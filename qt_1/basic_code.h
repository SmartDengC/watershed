# include <opencv2/opencv.hpp>
# include <iostream>
# include "stdio.h"
# include "stdlib.h"

using namespace cv;
using namespace std;

int opencv_to_all_1() {
	array<int, 15> seed_number;
	for (int i = 1; i <= 10; i++) {
		String picPath = "E:\\Data_Code\\qt\\qt_opencv\\src\\qt_1\\qt_1\\imgs\\original_img\\"+std::to_string(i) + ".JPG";
		Mat src = imread(picPath);
		// ���¾��Ǵ�����ͼƬ�Ĺ���
		if (src.empty()) {
			printf("none pic");
		}
		namedWindow("input image");
		// namedWindow("input image", WINDOW_AUTOSIZE);
		// imshow("input image", src);

		// cv::resize(src, src, Size(900, 900));
		// (1): gray ������ɫ��
		Mat gray, binary, shifted;
		// ������㣬���ڽ�������meanShift�����������ķ�����Ե������һ���ռ䣬һ��color����
		pyrMeanShiftFiltering(src, shifted, 21, 51);

		// (2): binary
		cvtColor(shifted, gray, COLOR_BGR2GRAY);    // �任��bgr2gray
		threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);    // ��ֵ��

		//(4):  structure_element(������̬ѧ�������õĽṹԪ��)
		Mat structure_element = getStructuringElement(MORPH_RECT, Size(5, 5));
		//(5):  Open morphology���ô���̬ѧ����е㣩
		morphologyEx(binary, binary, MORPH_OPEN, structure_element);
		// (3): distance tranform
		Mat dist;
		distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);  // L2 ������֮��ľ���
		normalize(dist, dist, 0, 1, NORM_MINMAX); // �任֮��ֵ�Ƚ�С

		// (4): binary
		threshold(dist, dist, 0.4, 1, THRESH_BINARY);

		// (5): markers
		Mat dist_m;
		dist.convertTo(dist_m, CV_8U); // ���8ͨ����
		
		//(9):  erode(��ʴ��̬ѧ)
		threshold(dist_m, dist_m, 0.4, 1, THRESH_BINARY);
		erode(dist_m, dist_m, structure_element);
		vector<vector<Point>> contours(11111);

		// ���Ҷ�ֵͼ��ı�Ե����, �൱������ͨͼ
		findContours(dist_m, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

		// (6): create markers
		Mat markers = Mat::zeros(src.size(), CV_32SC1);
		for (size_t t = 0; t < contours.size(); t++) {
			drawContours(markers, contours, static_cast<int>(t), Scalar::all(double(static_cast<int>(t)) + 1), -1);
		}
		circle(markers, Point(5, 5), 3, Scalar(255), -1);

		// (7): ��̬ѧ��������ɫͼ��Ŀ������ȥ�����ţ��ý������
		Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
		morphologyEx(src, src, MORPH_ERODE, k);

		// (8): ��ɷ�ˮ��任
		watershed(src, markers);
		Mat mark = Mat::zeros(markers.size(), CV_8UC1);
		markers.convertTo(mark, CV_8UC1);
		bitwise_not(mark, mark, Mat());


		// (9): generate random color ��ԭʼ��ɫ�ָ
		vector<Vec3b> colors;
		for (size_t i = 0; i < contours.size(); i++) {
			int r = theRNG().uniform(0, 255);
			int g = theRNG().uniform(0, 255);
			int b = theRNG().uniform(0, 255);
			// ����color����
			colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
		}

		// (10): ��ɫ�����������ʾ������������ʱ�򽫼�����ӽ�ȥ��
		Mat dst = Mat::zeros(markers.size(), CV_8UC3); // ����ָ���Ĵ�С�����͵������顣
		int index = 0;
		// �����ɫ
		for (int row = 0; row < markers.rows; row++) {
			for (int col = 0; col < markers.cols; col++) {
				// index �൱�����ʱ����ֵ��0-255��
				index = markers.at<int>(row, col);
				if (index > 0 && index <= contours.size()) {
					dst.at<Vec3b>(row, col) = colors[double(index) - 1];
				}
				else {
					dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);  // �����ú�ɫ���
				}
			}
		}

		// (11): ������ͨͼ��Ȼ�����������д
		//���������� 	
		vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			mu[i] = moments(contours[i], false);
		}
		//�������������� 	
		vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}
		//�������������Ĳ���ʾ 	
		for (int i = 0; i < contours.size(); i++) {
			string tam = std::to_string(i + 1);
			putText(dst, tam, Point(mc[i].x - 20, mc[i].y + 20), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 0), 2);
		}
		imshow("dst", dst);
		seed_number[i] = contours.size();
	}
	for (int i = 1; i <= 10; i++) {
		printf("\n��%d�У�%d��", i,seed_number[i]);
	}
	return 0;
}
int opencv_to_all() {
	array<int, 15> seed_number;
	for (int i = 1; i <= 10; i++) {
		// string picPath = "E:/Data_Media/Data_Picture/seedPictures/IMG_1" + std::to_string(i) + ".JPG";
		string picPath = "E:\\Data_Code\\qt\\qt_opencv\\src\\qt_1\\qt_1\\imgs\\original_img\\" + std::to_string(i) + ".JPG";
		Mat src = imread(picPath);
		if (src.empty()) {
			continue;
		}
		namedWindow("input image");
		// namedWindow("input image", WINDOW_AUTOSIZE);
		// imshow("input image", src);

		// (1): gray ������ɫ��
		Mat gray, binary, shifted;
		// ������㣬���ڽ�������meanShift�����������ķ�����Ե������һ���ռ䣬һ��color����
		pyrMeanShiftFiltering(src, shifted, 21, 51);

		// (2): binary
		cvtColor(shifted, gray, COLOR_BGR2GRAY);    // �任��bgr2gray
		threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);    // ��ֵ��

		// (3): distance tranform
		Mat dist;
		distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);  // L2 ������֮��ľ���
		normalize(dist, dist, 0, 1, NORM_MINMAX); // �任֮��ֵ�Ƚ�С

		// (4): binary
		threshold(dist, dist, 0.4, 1, THRESH_BINARY);

		// (5): markers
		Mat dist_m;
		dist.convertTo(dist_m, CV_8U); // ���8ͨ����
		vector<vector<Point>> contours(11111);
		// ���Ҷ�ֵͼ��ı�Ե����, �൱������ͨͼ
		findContours(dist_m, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

		// (6): create markers
		Mat markers = Mat::zeros(src.size(), CV_32SC1);
		for (size_t t = 0; t < contours.size(); t++) {
			drawContours(markers, contours, static_cast<int>(t), Scalar::all(double(static_cast<int>(t)) + 1), -1);
		}
		circle(markers, Point(5, 5), 3, Scalar(255), -1);

		// (7): ��̬ѧ��������ɫͼ��Ŀ������ȥ�����ţ��ý������
		Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
		morphologyEx(src, src, MORPH_ERODE, k);
		imshow("src", src);

		// (8): ��ɷ�ˮ��任
		watershed(src, markers);
		Mat mark = Mat::zeros(markers.size(), CV_8UC1);
		markers.convertTo(mark, CV_8UC1);
		bitwise_not(mark, mark, Mat());


		// (9): generate random color ��ԭʼ��ɫ�ָ
		vector<Vec3b> colors;
		for (size_t i = 0; i < contours.size(); i++) {
			int r = theRNG().uniform(0, 255);
			int g = theRNG().uniform(0, 255);
			int b = theRNG().uniform(0, 255);
			// ����color����
			colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
		}

		// (10): ��ɫ�����������ʾ������������ʱ�򽫼�����ӽ�ȥ��
		Mat dst = Mat::zeros(markers.size(), CV_8UC3); // ����ָ���Ĵ�С�����͵������顣
		int index = 0;
		// �����ɫ
		for (int row = 0; row < markers.rows; row++) {
			for (int col = 0; col < markers.cols; col++) {
				// index �൱�����ʱ����ֵ��0-255��
				index = markers.at<int>(row, col);
				if (index > 0 && index <= contours.size()) {
					dst.at<Vec3b>(row, col) = colors[double(index) - 1];
				}
				else {
					dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);  // �����ú�ɫ���
				}
			}
		}

		// (11): ������ͨͼ��Ȼ�����������д
		//���������� 	
		vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			mu[i] = moments(contours[i], false);
		}
		//�������������� 	
		vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}
		//�������������Ĳ���ʾ 	
		for (int i = 0; i < contours.size(); i++) {
			string tam = std::to_string(i + 1);
			putText(dst, tam, Point(mc[i].x - 20, mc[i].y + 20), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 0), 2);
		}
		seed_number[i] = contours.size();
	}
	for (int i = 1; i <= 10; i++) {
		printf("\n��%d�У�%d��", i,seed_number[i]);
	}
	return 0;
}



void opencv_to_one() {
	array<int, 15> seed_number;

	//string picPath = "E:\\Data_Code\\qt\\qt_opencv\\src\\qt_1\\qt_1\\imgs\\original_img\\6.JPG";
	string picPath = "E:\\Data_Code\\qt\\qt_opencv\\src\\qt_1\\qt_1\\imgs\\original_img\\10_1.jpg";
	Mat src = imread(picPath);
	if (src.empty()) {
		printf("none pic");
	}
	namedWindow("input image");
	// namedWindow("input image", WINDOW_AUTOSIZE);
	// imshow("input image", src);

	cv::resize(src, src, Size(900, 900));
	// (1): gray ������ɫ��
	Mat gray, binary, shifted;
	// ������㣬���ڽ�������meanShift�����������ķ�����Ե������һ���ռ䣬һ��color����
	pyrMeanShiftFiltering(src, shifted, 21, 51);

	// (2): binary
	cvtColor(shifted, gray, COLOR_BGR2GRAY);    // �任��bgr2gray
	threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);    // ��ֵ��

	//(4):  structure_element(������̬ѧ�������õĽṹԪ��)
	Mat structure_element = getStructuringElement(MORPH_RECT, Size(5, 5));
	//(5):  Open morphology���ô���̬ѧ����е㣩
	morphologyEx(binary, binary, MORPH_OPEN, structure_element);
	// (3): distance tranform
	Mat dist;
	distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);  // L2 ������֮��ľ���
	normalize(dist, dist, 0, 1, NORM_MINMAX); // �任֮��ֵ�Ƚ�С

	// (4): binary
	threshold(dist, dist, 0.4, 1, THRESH_BINARY);

	// (5): markers
	Mat dist_m;
	dist.convertTo(dist_m, CV_8U); // ���8ͨ����
	
	//(9):  erode(��ʴ��̬ѧ)
	threshold(dist_m, dist_m, 0.4, 1, THRESH_BINARY);
	erode(dist_m, dist_m, structure_element);
	vector<vector<Point>> contours(11111);

	// ���Ҷ�ֵͼ��ı�Ե����, �൱������ͨͼ
	findContours(dist_m, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// (6): create markers
	Mat markers = Mat::zeros(src.size(), CV_32SC1);
	for (size_t t = 0; t < contours.size(); t++) {
		drawContours(markers, contours, static_cast<int>(t), Scalar::all(double(static_cast<int>(t)) + 1), -1);
	}
	circle(markers, Point(5, 5), 3, Scalar(255), -1);

	// (7): ��̬ѧ��������ɫͼ��Ŀ������ȥ�����ţ��ý������
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(src, src, MORPH_ERODE, k);
	imshow("src", src);

	// (8): ��ɷ�ˮ��任
	watershed(src, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	imshow("mark_1", mark);
	bitwise_not(mark, mark, Mat());
	imshow("mark", mark);


	// (9): generate random color ��ԭʼ��ɫ�ָ
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++) {
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int b = theRNG().uniform(0, 255);
		// ����color����
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// (10): ��ɫ�����������ʾ������������ʱ�򽫼�����ӽ�ȥ��
	Mat dst = Mat::zeros(markers.size(), CV_8UC3); // ����ָ���Ĵ�С�����͵������顣
	int index = 0;
	// �����ɫ
	for (int row = 0; row < markers.rows; row++) {
		for (int col = 0; col < markers.cols; col++) {
			// index �൱�����ʱ����ֵ��0-255��
			index = markers.at<int>(row, col);
			if (index > 0 && index <= contours.size()) {
				dst.at<Vec3b>(row, col) = colors[double(index) - 1];
			}
			else {
				dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);  // �����ú�ɫ���
			}
		}
	}

	// (11): ������ͨͼ��Ȼ�����������д
	//���������� 	
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		mu[i] = moments(contours[i], false);
	}
	//�������������� 	
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	//�������������Ĳ���ʾ 	
	for (int i = 0; i < contours.size(); i++) {
		string tam = std::to_string(i + 1);
		putText(dst, tam, Point(mc[i].x - 20, mc[i].y + 20), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 0), 2);
	}
	imshow("dst", dst);
	waitKey(0);
}
