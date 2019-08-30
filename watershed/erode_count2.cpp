#include<stdio.h>
#include<stdlib.h>
#include<opencv2/opencv.hpp>
#include<iostream>

/*
ʹ�������ռ�
*/

using namespace cv;
using namespace std;

int  mdain(int argc, char** argv)
{
	for (int i = 303; i <= 484; i++)
	{
		string picPath = "F:/Photo/IMG_1" + std::to_string(i) + ".JPG";
		Mat src = imread(picPath);
		if (src.empty()) {
			continue;
		}
		Mat gray, binary, shifted, morhImg, dist_m;
		//(1)ȥ���
		pyrMeanShiftFiltering(src, shifted, 30, 51);
		//(2):  change color to gray (��ɻ�ɫ)
		cvtColor(shifted, gray, COLOR_BGR2GRAY);
		//(3):  binary(��ֵ��)
		threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
		//(4):  structure_element(������̬ѧ�������õĽṹԪ��)
		Mat structure_element = getStructuringElement(MORPH_RECT, Size(5, 5));
		//(5):  Open morphology���ô���̬ѧ����е㣩
		morphologyEx(binary, binary, MORPH_OPEN, structure_element);
		//(6):  Distance conversion(����ת��)
		Mat dist;
		distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);
		normalize(dist, dist, 0, 1, NORM_MINMAX);
		//(7):  binary(��ֵ��)
		threshold(dist, dist, 0.4, 1, THRESH_BINARY);
		//(8)���8ͨ��
		dist.convertTo(dist_m, CV_8U);
		//(9):  erode(��ʴ��̬ѧ)
		threshold(dist_m, dist_m, 0.4, 1, THRESH_BINARY);
		erode(dist_m, morhImg, structure_element);
		//(10):  Find connectivity diagram(������ͨͼ)
		vector<vector<Point>> contours;
		findContours(morhImg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		// (11): create markers
		Mat markers = Mat::zeros(src.size(), CV_32SC1);
		for (size_t t = 0; t < contours.size(); t++) {
			drawContours(markers, contours, static_cast<int>(t), Scalar::all(double(static_cast<int>(t)) + 1), -1);
		}
		circle(markers, Point(5, 5), 3, Scalar(255), -1);


		// (12): generate random color �������ɫ��
		vector<Vec3b> colors;
		for (size_t i = 0; i < contours.size(); i++) {
			int r = theRNG().uniform(0, 255);
			int g = theRNG().uniform(0, 255);
			int b = theRNG().uniform(0, 255);
			colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
		}

		// (13): ��ɫ���
		Mat dst = Mat::zeros(markers.size(), CV_8UC3);
		int index = 0;
		// �����ɫ
		for (int row = 0; row < markers.rows; row++) {
			for (int col = 0; col < markers.cols; col++) {
				// index �൱�����ʱ����ֵ��0-255��
				index = markers.at<int>(row, col);
				if (index > 0 && index <= contours.size()) {
					dst.at<Vec3b>(row, col) = colors[double(index) - 1];  // �˴���doubleֻΪ����C26451
				}
				else {
					dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);  // �����ú�ɫ���
				}
			}
		}
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
		string outPicPath = "F:\\OutPhoto\\1" + std::to_string(i) + "_" + std::to_string(contours.size()) + ".jpg";
		imwrite(outPicPath, dst);
	}
	waitKey(0);
	return 0;
}