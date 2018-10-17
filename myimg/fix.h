#pragma once
#ifndef __FIX_
#define __FIX_
#include <math.h>
#include <stdint.h>
#include "mymath.h"
#include "myimg.h"
#include <iostream>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

//ir r g b w


typedef struct {
	uint8_t aver;
	doubleNum max;
	doubleNum min;
	uint8_t diff;
	uint8_t mode;//众数
	uint8_t median;//中位数
}ab_cis_data;//抽象

typedef struct {
	ab_cis_data vd;
	ab_cis_data vp[5];
	ab_cis_data vc[5];
	ab_cis_data rel_vp[5];
}sigle_cis_data;

typedef struct {
	sigle_cis_data upper_cis;
	sigle_cis_data lower_cis;
}cis_data;

/**
* 存储校正系数
* v_out = k*(v_in - d)
*/
class FixData {
public:
	uint8_t * d;
	double  *k;
	int size;
	FixData() {
		d = NULL;
		k = NULL;
		size = 0;
	}
	FixData(int _size) :size(_size) {
		d = new uint8_t[size];
		k = new double[size];
	}
	~FixData() {
		delete[]d;
		delete[]k;
	}
	void setsize(int s);
	inline static uint8_t ToK(double d) {
		int temp = (d - 1.0)*32.0;
		return (temp>255) ? 255 : temp;
	}
	void Clear() {
		memset(d, 0, size);
		for (int point_i = 0; point_i<size; point_i++) {
			k[point_i] = 1.0;
		}
	}
};

class ImgFix
{
	MyImg darkImg;
	MyImg whiteImg;
	int fix_cnt;//校正图像个数
	uint16_t vmax;
public:
	FixData * data;
	ImgFix(int size) :vmax(240) {
		fix_cnt = size;
		data = NULL;
	}
	inline static double fix(uint8_t vp, uint8_t vd, uint8_t max) {
		return (double)max / (double)(vp - vd);
	}
	int NewFix(MyImg& white, MyImg& dark);
	void release() {
		delete[]data;
	}
	void work();
	int Size()const {
		return fix_cnt;
	}
	int Width()const {
		return whiteImg.cols;
	}
	void clear() {
		for (int type_i = 0; type_i < fix_cnt; type_i++) {
			data[type_i].Clear();
		}
	}

	void _shift(u8 * table, int n) {
		uint8_t width = whiteImg.cols;
		for (int i = 0; i<(width - n); i++) {
			table[width - 1 - i] = table[width - 1 - i - n];
		}
	}
};


typedef struct __staticStd {
	u8 r;
	u8 g;
	u8 b;
	u8 ir;
	u8 uv;
}staticStd;
staticStd getSStd(u8 * pImg);
void getSStd2(u8 * pImg, u8 * dark);
void print_fix_data(cis_data& data, const char *title);
cis_data get_fix_data(u8 * white, u8 * dark, u8 * stdbuf);
cis_data get_std_data(u8 * white, u8 *stdbuf);
void print_fix_rel_data(cis_data& data, const char *title);

#endif

