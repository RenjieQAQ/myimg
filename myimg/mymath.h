#pragma once
#ifndef __MYMATH_
#define __MYMATH_

#include <math.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int


int aver(u8 *buf, int size);
int aver_col(u8 *buf, int col_no, int width, int rowstart, int rows);
int aver_row(u8 *buf, int row_no, int width, int colstart, int cols);
int aver(u8 *buf, int width, int rowstart, int colstart, int rows, int cols);

u8 findMin(u8 *table, int length);
u8 findMax(u8 *table, int length);

u8 findMax(u8 *table, int colIndex, int width, int start_row, int rows);
u8 findMin(u8 *table, int colIndex, int width, int start_row, int rows);

typedef struct {
	int val;
	int index;
}doubleNum;

doubleNum findMaxWithIndex(u8 *table, int width, int height, int row1, int row2, int col1, int col2);
doubleNum findMinWithIndex(u8 *table, int length);
doubleNum findMaxWithIndex(u8 *table, int length);
doubleNum findMinWithIndex(u8 *table, int width, int height, int row1, int row2, int col1, int col2);

#define AVER_BLOCK 0
#define ROW 1
#define COL 2
u8 getAver(u8 *table, int width, int height, int row1, int row2, int col1, int col2, int type);
u8 getMax(u8 *table, int width, int height, int row1, int row2, int col1, int col2, int type);
u8 getMin(u8 *table, int width, int height, int row1, int row2, int col1, int col2, int type);

u8 getMode(u8 *table, int start, int end);
u8 getMedian(u8 *table, int start, int end);
#endif
