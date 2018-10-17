#include "mymath.h"
#include "stdio.h"
#include "string.h"

/*
* ��Ϊwidth����ĳ��һС�ε�ƽ��ֵ
* @rowstart����ʼ��
* @rows������
*/
int aver_col(u8 *buf, int col_no, int width, int rowstart, int rows) {
	long sum = 0;
	for (int i = rowstart; i<(rowstart + rows); i++) {
		sum += buf[i*width + col_no];
	}
	return sum / rows;
}

/*
* ��Ϊwidth����ĳ��һС�ε�ƽ��ֵ
*/
int aver_row(u8 *buf, int row_no, int width, int colstart, int cols) {
	long sum = 0;
	for (int i = colstart; i<(colstart + cols); i++) {
		sum += buf[row_no*width + i];
	}
	return sum / cols;
}

/*
* ��Ϊwidth����ĳС�����ƽ��ֵ
*/
int aver(u8 *buf, int width, int rowstart, int colstart, int rows, int cols) {
	long sum = 0;
	long size = rows * cols;
	for (int i = rowstart; i<(rowstart + rows); i++) {
		for (int j = colstart; j<(colstart + cols); j++) {
			sum += buf[i*width + j];
		}
	}
	return sum / size;
}

/*
* ����ƽ��ֵ
*/
int aver(u8 *buf, int size) {
	long sum = 0;
	for (int i = 0; i<size; i++) {
		sum += buf[i];
	}
	return sum / size;
}

u8 findMin(u8 *table, int length)
{
	u8 min = 0;
	for (int i = 0; i<length; i++)
	{
		if (table[i]<min)
			min = table[i];
	}
	return min;
}
u8 findMin(u8 *table, int colIndex, int width, int start_row, int rows)
{
	u8 min = 0;
	for (int i = start_row; i<(start_row + rows); i++)
	{
		if (table[i*width + colIndex]<min)
			min = table[i*width + colIndex];
	}
	return min;
}
u8 findMax(u8 *table, int length)
{
	u8 max = 0;
	for (int i = 0; i<length; i++)
	{
		if (table[i]>max)
			max = table[i];
	}
	return max;
}
u8 findMax(u8 *table, int colIndex, int width, int start_row, int rows)
{
	u8 max = 0;
	for (int i = start_row; i<(start_row + rows); i++)
	{
		if (table[i*width + colIndex]>max)
			max = table[i*width + colIndex];
	}
	return max;
}

/*
* ��һ�������У�ѡȡһС�������ֵ�����������ֵ�Ͷ�Ӧ��λ��
* row1<=r<row2
* col1<=c<col2
*/
doubleNum findMaxWithIndex(u8 *table, int width, int height, int row1, int row2, int col1, int col2) {
	doubleNum max;
	max.val = 0;
	for (int i = row1; i<row2; i++) {
		for (int j = col1; j<col2; j++) {
			if (*(table + i * width + j)>max.val) {
				max.val = *(table + i * width + j);
				max.index = i * width + j;
			}
		}
	}
	return max;
}

doubleNum findMaxWithIndex(u8 *table, int length) {
	doubleNum max;
	max.val = 0;
	max.index = 0;
	for (int i = 0; i<length; i++)
	{
		if (table[i]>max.val) {
			max.val = table[i];
			max.index = i;
		}
	}
	return max;
}

/*
* ��һ�������У�ѡȡһС������Сֵ����������Сֵ�Ͷ�Ӧ��λ��
* row1<=r<row2
* col1<=c<col2
*/
doubleNum findMinWithIndex(u8 *table, int width, int height, int row1, int row2, int col1, int col2) {
	doubleNum min;
	min.index = 0;
	min.val = 255;
	for (int i = row1; i<row2; i++) {
		for (int j = col1; j<col2; j++) {
			if (*(table + i * width + j)<min.val) {
				min.val = *(table + i * width + j);
				min.index = i * width + j;
			}
		}
	}
	return min;
}
doubleNum findMinWithIndex(u8 *table, int length) {
	doubleNum min;
	min.val = 255;
	min.index = 0;
	for (int i = 0; i<length; i++)
	{
		if (table[i]<min.val) {
			min.val = table[i];
			min.index = i;
		}
	}
	//	printf("%d,%d*************************\n",min.index,length);
	return min;
}
/*
* ��һ�������У���ĳ�С�ĳ�С�ĳ���ƽ��ֵ
* type
0:��
1:��
2:��
*/
u8 getAver(u8 *table, int width, int height, int row1, int row2, int col1, int col2, int type = 0) {
	u32 sum = 0;
	int row_end = row2;
	int col_end = col2;
	if (type == 1)//һ��
		row_end = row1 + 1;
	if (type == 2)
		col_end = col1 + 1;
	if (type == 3) {
		col_end = col1 + 1;
		//printf("%d,%d,%d,%d\n",row1,row_end,col1,col_end);
	}

	for (int i = row1; i<row_end; i++) {
		for (int j = col1; j<col_end; j++) {
			sum += table[width*i + j];
		}
	}
	//if(type==0)
	//	printf("\033[31;1m%d,%d\n\033[0m",sum,(row_end-row1)*(col_end-col1));
	return sum / ((row_end - row1)*(col_end - col1));
}

/*
* ��һ�������У���ĳ�С�ĳ�С�ĳ�����Сֵ
* type
0:��
1:��
2:��
*/
u8 getMin(u8 *table, int width, int height, int row1, int row2, int col1, int col2, int type = 0) {
	u8 min = 255;
	int row_end = row2;
	int col_end = col2;
	if (type == 1)//һ��
		row_end = row1 + 1;
	if (type == 2)
		col_end = col1 + 1;
	for (int i = row1; i<row_end; i++) {
		for (int j = col1; j<col_end; j++) {
			if (table[width*i + j]<min)
				min = table[width*i + j];
		}
	}
	return min;
}

/*
* ��һ�������У���ĳ�С�ĳ�С�ĳ������ֵ
* type
0:��
1:��
2:��
*/
u8 getMax(u8 *table, int width, int height, int row1, int row2, int col1, int col2, int type = 0) {
	u8 max = 0;
	int row_end = row2;
	int col_end = col2;
	if (type == 1)//һ��
		row_end = row1 + 1;
	if (type == 2)
		col_end = col1 + 1;
	for (int i = row1; i<row_end; i++) {
		for (int j = col1; j<col_end; j++) {
			if (table[width*i + j]>max)
				max = table[width*i + j];
		}
	}
	return max;
}

//����

u8 getMode(u8 *table, int start, int end) {
	int *cnt = new int[256];
	for (int i = start; i<end; i++) {
		cnt[table[i]]++;
	}
	u8 max = 0;
	u8 temp = 0;
	for (int i = 0; i<256; i++)
	{
		if (cnt[i]>max) {
			max = cnt[i];
			temp = i;
		}
	}
	delete cnt;
	return temp;
}

void bubbleSort(u8 * arr, int n) {
	for (int i = 0; i<n - 1; i++) {
		for (int j = 0; j<n - i - 1; j++) {
			if (arr[j]>arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}
u8 getMedian(u8 *table, int start, int end) {
	int temp = 0;
	int length = end - start;
	u8 *arr = new u8[length];
	memcpy(arr, table + start, length);
	bubbleSort(arr, length);
	temp = arr[length / 2];
	delete arr;
	return temp;
}

