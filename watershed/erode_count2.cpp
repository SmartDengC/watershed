#include<stdio.h>
#include<stdlib.h>
#include<opencv2/opencv.hpp>
#include<iostream>

/*
使用命名空间
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
		//(1)去噪点
		pyrMeanShiftFiltering(src, shifted, 30, 51);
		//(2):  change color to gray (变成灰色)
		cvtColor(shifted, gray, COLOR_BGR2GRAY);
		//(3):  binary(二值化)
		threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
		//(4):  structure_element(进行形态学处理所用的结构元素)
		Mat structure_element = getStructuringElement(MORPH_RECT, Size(5, 5));
		//(5):  Open morphology（用打开心态学处理残点）
		morphologyEx(binary, binary, MORPH_OPEN, structure_element);
		//(6):  Distance conversion(距离转化)
		Mat dist;
		distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);
		normalize(dist, dist, 0, 1, NORM_MINMAX);
		//(7):  binary(二值化)
		threshold(dist, dist, 0.4, 1, THRESH_BINARY);
		//(8)变成8通道
		dist.convertTo(dist_m, CV_8U);
		//(9):  erode(腐蚀形态学)
		threshold(dist_m, dist_m, 0.4, 1, THRESH_BINARY);
		erode(dist_m, morhImg, structure_element);
		//(10):  Find connectivity diagram(查找连通图)
		vector<vector<Point>> contours;
		findContours(morhImg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		// (11): create markers
		Mat markers = Mat::zeros(src.size(), CV_32SC1);
		for (size_t t = 0; t < contours.size(); t++) {
			drawContours(markers, contours, static_cast<int>(t), Scalar::all(double(static_cast<int>(t)) + 1), -1);
		}
		circle(markers, Point(5, 5), 3, Scalar(255), -1);


		// (12): generate random color （随机颜色）
		vector<Vec3b> colors;
		for (size_t i = 0; i < contours.size(); i++) {
			int r = theRNG().uniform(0, 255);
			int g = theRNG().uniform(0, 255);
			int b = theRNG().uniform(0, 255);
			colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
		}

		// (13): 颜色填充
		Mat dst = Mat::zeros(markers.size(), CV_8UC3);
		int index = 0;
		// 填充颜色
		for (int row = 0; row < markers.rows; row++) {
			for (int col = 0; col < markers.cols; col++) {
				// index 相当与填充时的数值（0-255）
				index = markers.at<int>(row, col);
				if (index > 0 && index <= contours.size()) {
					dst.at<Vec3b>(row, col) = colors[double(index) - 1];  // 此处加double只为消除C26451
				}
				else {
					dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);  // 否者用黑色填充
				}
			}
		}
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
		string outPicPath = "F:\\OutPhoto\\1" + std::to_string(i) + "_" + std::to_string(contours.size()) + ".jpg";
		imwrite(outPicPath, dst);
	}
	waitKey(0);
	return 0;
}