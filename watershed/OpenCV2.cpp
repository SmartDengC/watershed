# include <opencv2/opencv.hpp>
# include <iostream>
# include "stdio.h"
# include "stdlib.h"

using namespace cv;
using namespace std;

// 因为是一个项目，不能有两个main，所以重命名madin即可使用说
int madin(int argc, char** argv) {
	/**
		输入图像----灰度----二值----距离变换----寻找种子
			   ----生成marker-----分水岭变换----输出图像---end
	*/
	for (int i = 303; i <= 484; i++) {
		string picPath = "E:/Data_Media/Data_Picture/seedPictures/IMG_1" + std::to_string(i) + ".JPG";
		Mat src = imread(picPath);
		if (src.empty()) {
			continue;
		}
		// namedWindow("input image", WINDOW_AUTOSIZE);
		// imshow("input image", src);

		// (1): gray 背景纯色化
		Mat gray, binary, shifted;
		// 纯化噪点，基于金字塔的meanShift，迭代收敛的方法边缘保留，一个空间，一个color距离
		pyrMeanShiftFiltering(src, shifted, 21, 51);

		// (2): binary
		cvtColor(shifted, gray, COLOR_BGR2GRAY);    // 变换成bgr2gray
		threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);    // 二值化

		// (3): distance tranform
		Mat dist;
		distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);  // L2 是两点之间的距离
		normalize(dist, dist, 0, 1, NORM_MINMAX); // 变换之后值比较小

		// (4): binary
		threshold(dist, dist, 0.4, 1, THRESH_BINARY);

		// (5): markers
		Mat dist_m;
		dist.convertTo(dist_m, CV_8U); // 变成8通道的
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

		// (8): 完成分水岭变换
		watershed(src, markers);
		Mat mark = Mat::zeros(markers.size(), CV_8UC1);
		markers.convertTo(mark, CV_8UC1);
		bitwise_not(mark, mark, Mat());


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
		Mat dst = Mat::zeros(markers.size(), CV_8UC3); // 返回指定的大小和类型的零数组。
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
		//计算轮廓矩 	
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
		string outPicPath = "E:\\Data_Media\\Data_Picture\\seedPictures\\output\\1" + std::to_string(i) + "_" + std::to_string(contours.size()) + ".jpg";
		imwrite(outPicPath, dst);
	}
	// imshow("final Result", dst);
	// printf("number of objects :%zd", contours.size());

	waitKey(0);
	return 0;
}