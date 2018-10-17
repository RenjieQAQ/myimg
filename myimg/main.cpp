#include <iostream>
using namespace std;
#include <opencv2/opencv.hpp>
using namespace cv;

#include "myimg.h"

const int image_width = 1440;
const int sub_image_height = 384;
const int sub_image_num = 5;
const int image_length = sub_image_height * sub_image_num;

int main() {
	Mat img = imread("bill.bmp");
	MyImg myimg = img;
	namedWindow("bill", WINDOW_NORMAL);
	imshow("bill", myimg);
	waitKey();


}