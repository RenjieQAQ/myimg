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
* 包含图像数据以及尺寸信息
* -提供得到图像信息的方法
*/
class MyImg :public Mat {
	uint32_t subImgNum;
	uint32_t captureLine;
	BillRecord record;
	/*水印标志，将尺寸信息加到图像*/
	int _mark;
public:
	/*默认构造函数为得到的是空图像*/
	MyImg();
	MyImg(int wid, int sub_hei, int sub_num);
	MyImg(const char* name, int subnum = 1);
	MyImg(Mat & img);//从基类的拷贝构造函数

	void newImage(int wid, int sub_hei, int sub_num);//分配内存
	enum ImgType { red = 0, green, blue, ir, irt, dark = -1 };

	/*返回图像信息*/
	inline void SetCaptureLine(int line) {
		captureLine = line;
	}

	inline uint8_t *getDataByType(ImgType type) {
		if (type == -1)
			return NULL;
		return data + rows / subImgNum * type * cols;
	}
	/*
	* 返回某行的图像地址
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
	/*设置记录*/
	void setRecord(BillRecord& record);
	void SetMark();
private:
	int _LoadMark();
};

void Mat3ToBuf(Mat& img, uint8_t * buf);
void saveimg(const char *name, uint8_t *imgBuf, int width, int height);
void loadimg(const char *name, uint8_t *imgBuf);


#endif /* BASE_MYIMG_H_ */

