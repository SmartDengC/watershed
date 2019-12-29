#include "qt_1.h"
# include <opencv2/opencv.hpp>
# include <opencv2/core.hpp>
# include <opencv2/imgcodecs.hpp>
# include <opencv2/imgproc.hpp>
# include "MatToQImage.h"
# include <QPainter>
# include <QObject>
# include "ConnectToAll.h"

# include <QLineEdit>
using namespace cv;
using namespace std;

Qt_1::Qt_1(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	Mat src = imread("up2_2.jpg");
	if (src.empty()) {
		printf("could not load image...\n");
		char c = getchar();
		cout << "The picture is empty!";
	}
	// jGnamedWindow("src");
	cv::resize(src, src, Size(300, 300));  
	// cv::resize(src, src, Size(1000, 1000));

	Mat gray, binary, shifted;
	pyrMeanShiftFiltering(src, shifted, 21, 51);

	cvtColor(shifted, gray, COLOR_BGR2GRAY);  
	threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

	first_button = new QPushButton(this);
	origin_label = new QLabel(this);
	shift_label = new QLabel(this);
	binary_label = new QLabel(this);
	gray_label = new QLabel(this);
	// 操作按钮
	origin_button = new QPushButton(this);
	shift_button = new QPushButton(this);
	gray_button = new QPushButton(this);
	binary_button = new QPushButton(this);

	first_button->setText(QStringLiteral("预处理"));
	origin_button->setText(QStringLiteral("原图像"));
	shift_button->setText(QStringLiteral("滤波图"));
	gray_button->setText(QStringLiteral("灰度图"));
	binary_button->setText(QStringLiteral("二值图"));
	/*
	origin_label->setText(QStringLiteral("原图"));
	binary_label->setText(QStringLiteral("二值化图像"));
	gray_label->setText(QStringLiteral("灰度图像"));
	*/

	first_button->move(15, 50);
	origin_label->move(15, 100);
	shift_label->move(345, 100);
	gray_label->move(675, 100);
	binary_label->move(1005, 100);

	origin_button->move(130, 450);
	shift_button->move(460, 450);
	gray_button->move(790, 450);
	binary_button->move(1120, 450);

	connect(first_button, &QPushButton::clicked,
		[=]() {
			QImage origin_image = cvMat2QImage(src);
			QImage shift_image = cvMat2QImage(shifted);
			QImage gray_image = cvMat2QImage(gray);
			QImage binary_image = cvMat2QImage(binary);


			origin_label->setPixmap(QPixmap::fromImage(origin_image));
			shift_label->setPixmap(QPixmap::fromImage(shift_image));
			gray_label->setPixmap(QPixmap::fromImage(gray_image));
			binary_label->setPixmap(QPixmap::fromImage(binary_image));

			origin_label->resize(src.cols, src.rows);
			shift_label->resize(src.cols, src.rows);
			gray_label->resize(src.cols, src.rows);
			binary_label->resize(src.cols, src.rows);


			origin_label->show();
			shift_label->show();
			gray_label->show();
			binary_label->show();


		}
	);

	// (3): distance tranform
	Mat dist;
	distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);  // L2 是两点之间的距离
	normalize(dist, dist, 0, 1, NORM_MINMAX); // 变换之后值比较小
	// imshow("distance", dist);

	// (4): binary
	Mat dist_b; 
	threshold(dist, dist_b, 0.4, 1, THRESH_BINARY);
	// imshow("dist_b", dist_b);

	// (5): markers
	Mat dist_m;
	dist_b.convertTo(dist_m, CV_8U); // 变成8通道的
	vector<vector<Point>> contours(11111);
	// 查找二值图像的边缘轮廓, 相当与找连通图
	findContours(dist_m, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// (6): create markers
	Mat markers = Mat::zeros(src.size(), CV_32SC1);
	for (size_t t = 0; t < contours.size(); t++) {
		drawContours(markers, contours, static_cast<int>(t), Scalar::all(double(static_cast<int>(t)) + 1), -1);
	}
	circle(markers, Point(5, 5), 3, Scalar(255), -1);

	// (7): 形态学操作，彩色图像，目的试试去掉干扰，让结果更好
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(src, src, MORPH_ERODE, k);

	// (8): 分水岭变换
	watershed(src, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark, Mat());
	// imshow("mark Result", mark);
	// (9): generate random color （原始白色分割）
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++) {
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int b = theRNG().uniform(0, 255);
		// 放在color里面
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// (10): 颜色填充与最终显示（可以在填充的时候将计数添加进去）
	Mat dst = Mat::zeros(markers.size(), CV_8UC3); 
	int index = 0;
	// 填充颜色
	for (int row = 0; row < markers.rows; row++) {
		for (int col = 0; col < markers.cols; col++) {
			// index 相当与填充时的数值（0-255）
			index = markers.at<int>(row, col);
			if (index > 0 && index <= contours.size()) {
				dst.at<Vec3b>(row, col) = colors[double(index) - 1];
			}
			else {
				dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);  // 否者用黑色填充
			}
		}
	}
	// (11): 查找连通图，然后进行数字填写
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		mu[i] = moments(contours[i], false);
	}
	//计算轮廓的质心 	
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	//画轮廓及其质心并显示 	
	for (int i = 0; i < contours.size(); i++) {
		string tam = std::to_string(i + 1);
		putText(dst, tam, Point(mc[i].x - 20, mc[i].y + 20), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 0), 2);
	}

	// imshow("final Result", dst);

	second_button = new QPushButton(this);
	origin2_label = new QLabel(this);
	count2_label = new QLineEdit(this);
	distance_label = new QLabel(this);
	binary2_label = new QLabel(this);
	watershed_label = new QLabel(this);
	watershed_number_label = new QLabel(this);
	// 独立操作
	origin2_button = new QPushButton(this);
	distance_button = new QPushButton(this);
	binary2_button = new QPushButton(this);
	watershed_button = new QPushButton(this);
	watershed_number_button = new QPushButton(this);

	second_button->setText(QStringLiteral("第二步处理"));
	/*
	origin2_label->setText(QStringLiteral("原图"));
	count2_label->setText(QStringLiteral("种子总数："));
	distance_label->setText(QStringLiteral("距离变换图像"));
	binary2_label->setText(QStringLiteral("二值化图像"));
	watershed_label->setText(QStringLiteral("分水岭图像"));
	watershed_number_label->setText(QStringLiteral("分水岭彩色图像"));
	*/
	// 独立操作
	origin2_button->setText(QStringLiteral("原图像"));
	distance_button->setText(QStringLiteral("距离变换"));
	binary2_button->setText(QStringLiteral("二值图"));
	watershed_button->setText(QStringLiteral("分水岭图"));
	watershed_number_button->setText(QStringLiteral("分水岭彩图"));


	second_button->move(15, 550);
	count2_label->move(200, 555);
	origin2_label->move(15, 600);
	distance_label->move(345, 600);
	binary2_label->move(675, 600);
	watershed_label->move(1005, 600);
	watershed_number_label->move(1335, 600);

	//独立操作
	origin2_button->move(130, 950);
	distance_button->move(460, 950);
	binary2_button->move(790, 950);
	watershed_button->move(1120, 950);
	watershed_number_button->move(1450, 950);


	connect(second_button, &QPushButton::clicked,
		[=]() {
			QImage origin2_image = cvMat2QImage(src);
			QImage distance_image = cvMat2QImage(dist);
			// QImage distance_image = QImage((const unsigned char*)dist.data, dist.cols, dist.rows, QImage::Format_RGB888);
			QImage binary_image = cvMat2QImage(dist_b);
			// QImage binary_image = QImage((const unsigned char*)dist_b.data, dist_b.cols, dist_b.rows, QImage::Format_RGB888);
			QImage watershed_image = cvMat2QImage(mark);
			QImage watershed_number_image = cvMat2QImage(dst);

			origin2_label->setPixmap(QPixmap::fromImage(origin2_image));
			distance_label->setPixmap(QPixmap::fromImage(distance_image));
			binary2_label->setPixmap(QPixmap::fromImage(binary_image));
			watershed_label->setPixmap(QPixmap::fromImage(watershed_image));
			watershed_number_label->setPixmap(QPixmap::fromImage(watershed_number_image));

			origin2_label->resize(src.cols, src.rows);
			distance_label->resize(QSize(distance_image.width(), distance_image.height()));
			binary2_label->resize(src.cols, src.rows);
			watershed_label->resize(src.cols, src.rows);
			watershed_number_label->resize(src.cols, src.rows);

			count2_label->setText(QStringLiteral("种子总数：") + QString::number(contours.size()));

			origin2_label->show();
			distance_label->show();
			binary2_label->show();
			watershed_label->show();
			watershed_number_label->show();

		}
	);
	connect(origin_button, SIGNAL(clicked()), this, SLOT(ConnectToEach(src, origin_label)));

}

