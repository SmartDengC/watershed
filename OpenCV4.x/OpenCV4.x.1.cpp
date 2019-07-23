//建立一个C++控制台程序，添加OpenCV4.x.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// #include "pch.h"
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
# include <string.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
# include <stdio.h>

# include <string.h>
# include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

using namespace cv;
using namespace std;


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;
Mat src;
Mat src_gray;
int thresh = 30;
int max_thresh = 255;

int min()
{
	src = imread("E:/Data_Media/Data_Picture/seedPictures/IMG_1312.JPG");
	cvtColor(src, src_gray, COLOR_BGR2GRAY);//灰度化 	
	GaussianBlur(src, src, Size(3, 3), 0.1, 0, BORDER_DEFAULT);// 高斯滤波
	blur(src_gray, src_gray, Size(3, 3)); //滤波 	
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", src);
	moveWindow("image", 20, 20);
	//定义Canny边缘检测图像 	
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//利用canny算法检测边缘 	
	Canny(src_gray, canny_output, thresh, thresh * 3, 3);
	namedWindow("canny", WINDOW_AUTOSIZE);
	imshow("canny", canny_output);
	moveWindow("canny", 550, 20);
	//查找轮廓 	
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//计算轮廓矩 	
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//计算轮廓的质心 	
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	//画轮廓及其质心并显示 	
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	// Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(255, 0, 0);
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 5, Scalar(0, 0, 255), -1, 8, 0);
		rectangle(drawing, boundingRect(contours.at(i)), Scalar(0, 255, 0));
		char tam[100];
		printf(tam, "(%0.0f,%0.0f)", mc[i].x, mc[i].y);
		putText(drawing, tam, Point(mc[i].x, mc[i].y), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 255), 1);
	}
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	moveWindow("Contours", 1100, 20);
	waitKey(0);
	src.release();
	src_gray.release();
	return 0;
}
#include "stdio.h"
#include "stdlib.h"

int mi(void)
{
	int num = 10;
	char str[100];
	std::to_string(num);
	printf("%s\n", str);
	system("pause");
	return 0;
}
