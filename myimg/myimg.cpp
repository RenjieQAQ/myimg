/*
* myimg.cpp
*
*  Created on: Oct 10, 2018
*      Author: renjie
*/
#include "myimg.h"


/*
* ����ά�Ҷ�ͼ��opencv���󿽱���һά�Ҷ�ͼ
*/
void Mat3ToBuf(Mat& img, uint8_t * buf)
{
	int width = img.cols;
	int height = img.rows;
	for (int i = 0; i<width; i++)
	{
		for (int j = 0; j<height; j++)
		{
			buf[j*width + i] = img.at<uint8_t>(j, 3 * i);
		}
	}
}

void saveimg(const char *name, uint8_t *imgBuf, int width, int height)
{
	Mat img(height, width, CV_8UC1, imgBuf);
	imwrite(name, img);
}

void loadimg(const char *name, uint8_t *imgBuf) {
	Mat imgmat = imread(name);
	Mat3ToBuf(imgmat, imgBuf);
}


const char * markstr = "mark";

/*���ˮӡ*/
void MyImg::SetMark() {
	static const int roi_cols = 134;
	static const int roi_rows = 25;
	uint32_t *info = (uint32_t*)LineData(rows - 1);
	*info++ = *(uint32_t*)markstr;
	*info++ = subImgNum;
	*info++ = captureLine;
	MoneyResult * presult = (MoneyResult *)info;
	*presult = record.moneyResult;
	/*��ͼ*/
	Mat imgROI = (*this)(Rect(cols / 2, irt * (rows / subImgNum), roi_cols, roi_rows));
	Mat gzh(roi_rows, roi_cols, CV_8UC1, record.GZHImage);
	gzh.copyTo(imgROI);
	_mark = 1;
}

/*��ȡˮӡ*/
int MyImg::_LoadMark() {
	static const int roi_cols = 134;
	static const int roi_rows = 25;
	uint32_t *info = (uint32_t*)LineData(rows - 1);
	if (*info++ != *(uint32_t*)markstr)
		return -1;
	subImgNum = *info++;
	captureLine = *info++;
	MoneyResult * presult = (MoneyResult *)info;
	record.moneyResult = *presult;

	/*��ͼ*/
	Mat imgROI = (*this)(Rect(cols / 2, irt * (rows / subImgNum), roi_cols, roi_rows));
	Mat gzh(roi_rows, roi_cols, CV_8UC1, record.GZHImage);
	imgROI.copyTo(gzh);
	_mark = 1;
	return 0;
}


/*���ü�¼,���*/
void MyImg::setRecord(BillRecord& record) {
	this->record = record;
}

void MyImg::ShowImgInfo() {
	cout << "MyImg:" << rows << "x" << cols << endl;
	if (_mark == 1) {
		cout << "* ��ͼ�����:" << subImgNum << endl;
		cout << "* ����ʵ������:" << SignleImgHeight() << endl;
		record.moneyResult.show();
	}
	else {
		cout << "�޸�����Ϣ" << endl;
	}
}



/*
* ��opencv��ͼ���е���ͼ��
*/
void MyImg::load(const char* name) {
	Mat img = imread(name, CV_LOAD_IMAGE_GRAYSCALE);
	*this = img;
	_LoadMark();
}


void MyImg::save(const char *name) {
	SetMark();
	imwrite(name, *this);
}


MyImg::MyImg() :record() {
	subImgNum = 1;
	captureLine = 0;
	_mark = 0;
}

MyImg::MyImg(int wid, int sub_hei, int sub_num) :record() {
	newImage(wid, sub_hei, sub_num);
	captureLine = 0;
	_mark = 0;
}

MyImg::MyImg(const char* name, int subnum) :
	record() {
	load(name);
	subImgNum = subnum;
	captureLine = 0;
	_mark = 0;
}

/*�ӻ���Ŀ������캯��*/
MyImg::MyImg(Mat & img) :Mat(img), record() {
	subImgNum = 1;
	captureLine = 0;
	_mark = 0;
}

void MyImg::newImage(int wid, int sub_hei, int sub_num) {
	subImgNum = sub_num;
	Mat *img = dynamic_cast<Mat*>(this);
	if (img != NULL)
		*img = zeros(sub_hei*sub_num, wid, 0);
	else {
		cout << "new image error!" << endl;
		exit(-1);
	}
}


