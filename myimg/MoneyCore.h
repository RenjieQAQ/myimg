#pragma once
/**
* @Copyright(c) 2016 华中科技大学—国家防伪中心 All rights reserved.
* @Author LiChengchen
* @Date 2017-03-11
* @Version 1.0.0
* @See
* @Description Money structure* -------------------------* @ Modified by Renjie
* @ Date 2018-10-17
* @ Description:
*	1.调整结构体内容
*   2.升级为类
*/

#ifndef MONEYCORE_H_
#define MONEYCORE_H_
#pragma pack(push)
#pragma pack(1)
//GZH
#define GZHHeight 		50
#define GZHWidth 		268
#define GZHqWidth 		67
//EDITION
#define EDITION_UNKNOWN			0
#define EDITION_RMB1YUAN		1
#define EDITION_RMB5T20			20
#define EDITION_RMB50YUAN		50
#define EDITION_RMB1999			1999
#define EDITION_RMB2005			2005
#define EDITION_RMB2015			2015
// Error Code
#define RMB_DIRECTION_ERROR		0x20
#define RMB_MONEY_ERROR			0x21
#define RMB_EDITION_ERROR		0x22
#define RMB_NUMBER_ERROR		0x23
#include <string.h>
#include <iostream>
using namespace std;
class MoneyResult {
public:
	char GZHresult[12];
	unsigned int isTrue;
	int money;	int edition;
	int direction;
	int grayValue;				//灰度值0~255
	int length;					//send up 36 Bytes
	int width;
	int index;
	int reserved;
	int lineNum;				//白光图像行数
	int angle;
	unsigned char refuseFlag;	//据识标志，0正常，1据识，没有正确识别，过斜或其他原因
	unsigned char noteType;		//币种信息
	int pos[11];
public:
	MoneyResult() {
		money = 0;
		direction = 0;
		edition = EDITION_UNKNOWN;
		grayValue = 0;
		angle = 0;
		isTrue = 0;
		length = 0;
		refuseFlag = 0;
		width = 0;
		reserved = 0;//baoliu
		noteType = 0;
		lineNum = 1000;	//白光图像行数(WidDpi = 1440)
		index = 0;
		memset(GZHresult, 0, 12);
		memset(pos, 0, 11);
	}
	MoneyResult(MoneyResult& record) {
		money = record.money;
		direction = record.direction;
		edition = record.edition;
		grayValue = record.grayValue;
		angle = record.angle;
		isTrue = record.isTrue;
		length = record.length;
		refuseFlag = record.refuseFlag;
		width = record.width;
		reserved = record.reserved;//baoliu
		noteType = record.noteType;
		lineNum = record.lineNum;	//白光图像行数(WidDpi = 1440)
		index = 0;
		memcpy(GZHresult, record.GZHresult, 12);
		memcpy(pos, record.pos, 11);
	}
	void test() {
		money = 100;
		direction = 1;
		edition = EDITION_RMB2005;
		grayValue = 0;
		angle = 0;
		isTrue = 12;
		length = 300;
		refuseFlag = 0;
		width = 100;
		reserved = 0;//baoliu
		noteType = 0;
		lineNum = 1000;	//白光图像行数(WidDpi = 1440)
		memcpy(GZHresult, "GZH0123456", 11);
		memset(pos, 0, 11);
	}
	void show() {
		cout << "* 记录:";
		cout << money << "元，";
		cout << edition << "，";
		cout << direction << "，";
		cout << length << "，";
		cout << width << "，";
		cout << "0x" << hex << isTrue << dec << "，";
		cout << GZHresult << endl;
	}
};
typedef struct EDITION {
	int positionX[26];
	int positionY[26];
	int charNums;
	int colsum[300];
	unsigned char thresh[4];
}EDITION;

//GZHRecord send to BIACAssist
class BillRecord {
public:
	MoneyResult moneyResult;		//
	unsigned char GZHImage[3350];	//134*25 Gray Image
	unsigned char FSNdata[12 * 32 * 4]; //32*12 Bytes
	unsigned char *correctedWhite;	//800*400 *2
	unsigned char *correctedIR;
public:
	BillRecord() :moneyResult() {
		memset(GZHImage, 0, 3350);
		memset(FSNdata, 0, 12 * 32 * 4);
		correctedWhite = NULL;
		correctedIR = NULL;
	}
	BillRecord(BillRecord& record) :moneyResult(record.moneyResult) {
		memcpy(GZHImage, record.GZHImage, 3350);
		memcpy(FSNdata, record.FSNdata, 12 * 32 * 4);
		correctedWhite = record.correctedWhite;
		correctedIR = record.correctedIR;
	}
	void test() {
		moneyResult.test();
		for (int i = 0; i < 25; i++)
			memset(GZHImage + i * 134, i * 10, 134);
	}
};

typedef struct ImgAddress {
	unsigned char* MiniImage;
	int* BWminiImage;
	unsigned char* BWCharImage;
	unsigned char* GrayCharImage;
}ImgAddress;

typedef enum RMB_MONEY {
	RMB_100 = 0,
	RMB_50,
	RMB_20,
	RMB_10,
	RMB_5,
	RMB_1
}RMB_MONEY;

typedef BillRecord BIACGZHRecord;
#pragma pack(pop)
#endif /* MONEYCORE_H_ */
