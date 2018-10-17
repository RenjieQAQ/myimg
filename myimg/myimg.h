#pragma once
/*
* myimg.h
*
*  Created on: Oct 10, 2018
*      Author: renjie
*/

#ifndef BASE_MYIMG_H_
#define BASE_MYIMG_H_

#include <iostream>
using namespace std;
#include "stdint.h"
#include "opencv2/opencv.hpp"
#include "MoneyCore.h"
using namespace cv;

extern const int image_width;
extern const int sub_image_height;
extern const int sub_image_num;
extern const int image_length;

/*
* ����ͼ�������Լ��ߴ���Ϣ
* -�ṩ�õ�ͼ����Ϣ�ķ���
*/
class MyImg :public Mat {
	uint32_t subImgNum;
	uint32_t captureLine;
	BillRecord record;
	/*ˮӡ��־�����ߴ���Ϣ�ӵ�ͼ��*/
	int _mark;
public:
	/*Ĭ�Ϲ��캯��Ϊ�õ����ǿ�ͼ��*/
	MyImg();
	MyImg(int wid, int sub_hei, int sub_num);
	MyImg(const char* name, int subnum = 1);
	MyImg(Mat & img);//�ӻ���Ŀ������캯��

	void newImage(int wid, int sub_hei, int sub_num);//�����ڴ�
	enum ImgType { red = 0, green, blue, ir, irt, dark = -1 };

	/*����ͼ����Ϣ*/
	inline void SetCaptureLine(int line) {
		captureLine = line;
	}

	inline uint8_t *getDataByType(ImgType type) {
		if (type == -1)
			return NULL;
		return data + rows / subImgNum * type * cols;
	}
	/*
	* ����ĳ�е�ͼ���ַ
	*/
	inline uint8_t *LineData(int line)const {
		return data + line * cols;

	}
	inline size_t Width()const {
		return cols;
	}
	inline size_t SubImgNum()const {
		return subImgNum;
	}
	inline size_t CaptureLine()const {
		return captureLine;
	}
	inline size_t SignleImgHeight()const {
		return captureLine / subImgNum;
	}
	inline uint8_t *Data()const {
		return data;
	}
	inline bool IsMarked()const {
		return _mark;
	}
	void load(const char* name);
	void save(const char *name);
	void ShowImgInfo();
	inline void SetSubNum(int n) {
		subImgNum = n;
	}
	/*���ü�¼*/
	void setRecord(BillRecord& record);
	void SetMark();
private:
	int _LoadMark();
};

void Mat3ToBuf(Mat& img, uint8_t * buf);
void saveimg(const char *name, uint8_t *imgBuf, int width, int height);
void loadimg(const char *name, uint8_t *imgBuf);


#endif /* BASE_MYIMG_H_ */

