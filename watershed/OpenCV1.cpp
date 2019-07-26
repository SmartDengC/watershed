# include <opencv2/opencv.hpp>
# include <iostream>
# include "stdio.h"
# include "stdlib.h"

using namespace cv;
using namespace std;
/**
* @brief:���ڶԷ�ˮ��֮���ͼƬ���м�������ʾ��ͼ����
* @param arg1�������������contours
* @param arg2: ��ˮ�봦�����Mat����
* @return���������Mat����
*/
Mat CountSeed(vector<vector<Point>> contours, Mat dst);

/**
* @brief�������������ڷ�ˮ��ǰ���ͼ����
* @param arg1:none
* @param arg2:none
* @return none
*/
int main(int argc, char** argv) {
	Mat src = imread("F:\\watershed\\photo\\originPic\\IMG_1304.JPG");
	if (src.empty()) {
		printf("could not load image...\n");
		char c = getchar();
		return -1;
	}
	namedWindow("input image", WINDOW_AUTOSIZE);
	imshow("input image", src);

	// (1): gray ������ɫ��
	Mat gray, binary, shifted;
	// ���� ��㣬���ڽ�������meanShift�����������ķ�����Ե������һ���ռ䣬һ��color���� ������С�ĺͲ����Ĳ��
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

	// (10): ��ɫ�����������ʾ������������ʱ�򽫼������ӽ�ȥ��
	Mat dst = Mat::zeros(markers.size(), CV_8UC3); // ����ָ���Ĵ�С�����͵������顣
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
	dst = CountSeed(contours, dst);
	imshow("final Result", dst);
	printf("number of objects :%zd", contours.size());

	waitKey(0);
	return 0;
}

Mat CountSeed(vector<vector<Point>> contours, Mat dst) {
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
	vector<Vec4i> hierarchy;
	for (int i = 0; i < contours.size(); i++) {
		// Scalar color = Scalar(255, 0, 0);
		// drawContours(dst, contours, i, color, 2, 8, hierarchy, 0, Point());
		string tam = std::to_string(i + 1);
		putText(dst, tam, Point(mc[i].x - 20, mc[i].y + 20), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 0), 2);
	}
	return dst;
}