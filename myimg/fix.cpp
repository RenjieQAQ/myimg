#include "fix.h"
#define cis_filt_size 3

void FixData::setsize(int s) {
	cout << "FixData size=" << size << endl;
	if (size != 0) {
		delete[]d;
		delete[]k;
	}
	if (size != s) {
		size = s;
		cout << "set FixData size=" << size << endl;
		d = new uint8_t[size];
		k = new double[size];
	}
}

int ImgFix::NewFix(MyImg& white, MyImg& dark) {
	darkImg = dark;
	whiteImg = white;
	if (white.IsMarked())
		fix_cnt = white.SubImgNum();
	else {
		cout << "校正初始化失败" << endl;
		return -1;
	}
	if (data != NULL) {
		release();
	}
	data = new FixData[fix_cnt];
	for (int type_i = 0; type_i < fix_cnt; type_i++) {
		data[type_i].setsize(white.cols);
	}
	return 0;
}
void ImgFix::work() {
	uint8_t width = whiteImg.cols;
	cout << "work1" << endl;
	/*第一层为校正的图像个数*/
	for (int type_i = 0; type_i < fix_cnt; type_i++) {
		/*第一层为校正的图像宽度*/
		for (int point_i = 0; point_i < width; point_i++) {
			uint8_t vd = darkImg.data[point_i];
			data[type_i].d[point_i] = vd;
			uint8_t vp = aver_col(whiteImg.getDataByType((MyImg::ImgType)type_i), point_i, width, 15, 200);
			data[type_i].k[point_i] = fix(vp, vd, vmax);
			//cout<<data[type_i].k[point_i]<<" ";
		}
		cout << endl;
	}
}

staticStd getSStd(u8 * pImg) {
	staticStd sstd;
	staticStd sstd2;
	u32 offset = 0;
	sstd.g = aver(pImg + 0 * image_width, image_width, 5, cis_filt_size, 150, 710);
	sstd.b = aver(pImg + ((offset + 1) % 5) * 300 * image_width, image_width, 5, cis_filt_size, 150, 710);
	sstd.uv = aver(pImg + ((offset + 2) % 5) * 300 * image_width, image_width, 5, cis_filt_size, 150, 710);
	sstd.ir = aver(pImg + ((offset + 3) % 5) * 300 * image_width, image_width, 5, cis_filt_size, 150, 710);
	sstd.r = aver(pImg + ((offset + 4) % 5) * 300 * image_width, image_width, 5, cis_filt_size, 150, 710);

	sstd2.g = aver(pImg + 0 * image_width, image_width, 5, 720 + cis_filt_size, 150, 710);
	sstd2.b = aver(pImg + ((offset + 1) % 5) * 300 * image_width, image_width, 5, 720 + cis_filt_size, 150, 710);
	sstd2.uv = aver(pImg + ((offset + 2) % 5) * 300 * image_width, image_width, 5, 720 + cis_filt_size, 150, 710);
	sstd2.ir = aver(pImg + ((offset + 3) % 5) * 300 * image_width, image_width, 5, 720 + cis_filt_size, 150, 710);
	sstd2.r = aver(pImg + ((offset + 4) % 5) * 300 * image_width, image_width, 5, 720 + cis_filt_size, 150, 710);
	printf("uv=%d,%d\n", sstd.uv, sstd2.uv);
	printf("ir=%d,%d\n", sstd.ir, sstd2.ir);
	printf("r=%d,%d\n", sstd.r, sstd2.r);
	printf("g=%d,%d\n", sstd.g, sstd2.g);
	printf("b=%d,%d\n", sstd.b, sstd2.b);
	return sstd;
}

#include "mymath.h"
char colorWordTable[6][3] = { "uv","ir","r","g","b","wh" };
u8 vd_aver[1440];

void getSStd2(u8 * pImg, u8 * dark) {
	doubleNum left[5];
	doubleNum right[5];
	//u8 lvd[5],rvd[5];

	u8 lvd_aver, rvd_aver;
	doubleNum lvd_aver_min, rvd_aver_min;
	u8 lvd_min, rvd_min;

	u8 left_aver[5], right_aver[5];
	//g b uv ir r


	for (int i = 0; i<5; i++) {
		left[i] = findMaxWithIndex(pImg, image_width, 1968, i * 300, i * 300 + 150, 3, 720);
		right[i] = findMaxWithIndex(pImg, image_width, 1968, i * 300, i * 300 + 150, 720, 1437);
		left_aver[i] = getAver(pImg, image_width, 1968, i * 300 + 10, i * 300 + 150, 3, 720, 0);
		right_aver[i] = getAver(pImg, image_width, 1968, i * 300 + 10, i * 300 + 150, 720, 1437, 0);
	}
	for (int i = 0; i<5; i++) {
		//lvd[i]=dark[left[i].index];
		//rvd[i]=dark[right[i].index];
		//lvd[i]=getAver(dark,image_width,1968,5,150,left[i].index,0,COL);
		//rvd[i]=getAver(dark,image_width,1968,5,150,right[i].index,0,COL);
	}
	for (int i = 0; i<1440; i++) {
		vd_aver[i] = getAver(dark, image_width, 1968, 5, 150, i, 0, COL);
	}
	lvd_aver = getAver(dark, image_width, 1968, 10, 150, 3, 720, 0);
	rvd_aver = getAver(dark, image_width, 1968, 10, 150, 720, 1437, 0);
	lvd_min = getMin(dark, image_width, 1968, 10, 150, 3, 720, 0);
	rvd_min = getMin(dark, image_width, 1968, 10, 150, 720, 1437, 0);
	lvd_aver_min = findMinWithIndex(vd_aver + 1, 720 - 1);
	rvd_aver_min = findMinWithIndex(vd_aver + 720, 720);

	printf("\033[35;1m\n标准化测试数据\n\033[0m");
	printf("\033[35;1m-------------------------------------------------------\n\033[0m");

	printf("\033[36;1mVd\n\033[0m");
	printf("		平均值		最小值		一行最小值\n");
	printf("上管：		%d		%d		%d	(%d)\n", lvd_aver, lvd_min, lvd_aver_min.val, lvd_aver_min.index);
	printf("下管：		%d		%d		%d	(%d)\n", rvd_aver, rvd_min, rvd_aver_min.val, rvd_aver_min.index);

	printf("\033[36;1m各色光Vp平均值\n\033[0m		");
	for (int i = 1; i<5; i++) {
		printf("%s		", colorWordTable[i]);
	}
	printf("\n");
	printf("上管		%d		%d		%d		%d\n", left_aver[3], left_aver[4], left_aver[0], left_aver[1]);
	printf("下管		%d		%d		%d		%d\n", right_aver[3], right_aver[4], right_aver[0], right_aver[1]);

	printf("\033[36;1m各色光最大参考值\n\033[0m		");
	for (int i = 1; i<5; i++) {
		printf("%s		", colorWordTable[i]);
	}
	printf("\n");
	printf("上管		%d		%d		%d		%d\n", left[3].val, left[4].val, left[0].val, left[1].val);
	printf("下管		%d		%d		%d		%d\n", right[3].val, right[4].val, right[0].val, right[1].val);
	//	printf("上管		%d		%d		%d		%d\n",left[3].val-lvd[3],left[4].val-lvd[4],left[0].val-lvd[0],left[1].val-lvd[1]);
	//	printf("下管		%d		%d		%d		%d\n",right[3].val-rvd[3],right[4].val-rvd[4],right[0].val-rvd[0],right[1].val-rvd[1]);
	printf("位置");
	for (int i = 0; i<5; i++) {
		printf("%d,%d ", left[i].index % 1440, right[i].index % 1440);
	}
	printf("\n");
	printf("\033[35;1m-------------------------------------------------------\n\033[0m");

}

void set_cis_data(ab_cis_data& v, u8 * buf, uint32_t col1, uint32_t col2) {
	static u8 v_aver[1440];
	v.aver = getAver(buf, image_width, 1968, 10, 150, col1, col2, 0);
	for (uint32_t i = col1; i<col2; i++) {
		v_aver[i] = getAver(buf, image_width, 1968, 5, 150, i, 0, COL);
	}
	//	v.min=findMinWithIndex(v_aver+col1,col2-col1);
	//	v.max=findMaxWithIndex(v_aver+col1,col2-col1);
	v.min = findMinWithIndex(v_aver, 1440, 1, 0, 1, col1, col2);
	v.max = findMaxWithIndex(v_aver, 1440, 1, 0, 1, col1, col2);
}

uint8_t cal_v_cross(int max, uint8_t vp, uint8_t vd) {
	return (max*vd) / (max + vd + vp);
}
void set_v_cross(sigle_cis_data& data) {
	for (int i = 0; i<5; i++) {
		data.vc[i].aver = cal_v_cross(240, data.vp[i].aver, data.vd.aver);
		data.vc[i].max.val = cal_v_cross(240, data.vp[i].max.val, data.vd.max.val);
		data.vc[i].min.val = cal_v_cross(240, data.vp[i].min.val, data.vd.min.val);
	}
}



void set_ab_cis_data(ab_cis_data& v, u8 * buf, uint32_t col1, uint32_t col2) {
	v.aver = getAver(buf, image_width, 1968, 0, 1, col1, col2, ROW);
	v.min = findMinWithIndex(buf, 1440, 1, 0, 1, col1, col2);
	v.max = findMaxWithIndex(buf, 1440, 1, 0, 1, col1, col2);
	v.diff = v.max.val - v.min.val;
	v.mode = getMode(buf, col1, col2);
	v.median = getMedian(buf, col1, col2);
}

static u8 vd_buf[1440];
static u8 vp_buf[5][1440];
static u8 rel_buf[5][1440];
cis_data get_fix_data(u8 * white, u8 * dark, u8 * stdbuf) {
	cis_data  data;
	/*	set_cis_data(data.upper_cis.vd,dark,cis_filt_size,720-cis_filt_size);
	set_cis_data(data.lower_cis.vd,dark,720+cis_filt_size,1440-cis_filt_size);
	for(int i=0;i<4;i++){
	set_cis_data(data.upper_cis.vp[i],stdbuf+(i+3)%5*300*1440,cis_filt_size,720-cis_filt_size);
	set_cis_data(data.lower_cis.vp[i],stdbuf+(i+3)%5*300*1440,720+cis_filt_size,1440-cis_filt_size);
	}
	set_cis_data(data.upper_cis.vp[4],white+1200*1440,cis_filt_size,720-cis_filt_size);
	set_cis_data(data.lower_cis.vp[4],white+1200*1440,720+cis_filt_size,1440-cis_filt_size);

	set_v_cross(data.upper_cis);
	set_v_cross(data.lower_cis);*/

	for (int i = 0; i<1440; i++) {
		vd_buf[i] = getAver(dark, image_width, 1968, 5, 150, i, 0, COL);
		for (int k = 0; k<4; k++) {
			vp_buf[k][i] = getAver(stdbuf, image_width, 1968, (k + 3) % 5 * 300 + 5, (k + 3) % 5 * 300 + 150, i, 0, COL);
			if (vp_buf[k][i]>vd_buf[i])
				rel_buf[k][i] = vp_buf[k][i] - vd_buf[i];
			else
				rel_buf[k][i] = 0;
		}
		vp_buf[4][i] = getAver(white + 1210 * 1440, image_width, 1968, 5, 150, i, 0, COL);
		if (vp_buf[4][i]>vd_buf[i])
			rel_buf[4][i] = vp_buf[4][i] - vd_buf[i];
		else
			rel_buf[4][i] = 0;

	}

	set_ab_cis_data(data.upper_cis.vd, vd_buf, cis_filt_size, 720 - cis_filt_size);
	set_ab_cis_data(data.lower_cis.vd, vd_buf, 720 + cis_filt_size, 1440 - cis_filt_size);
	for (int i = 0; i<5; i++) {
		set_ab_cis_data(data.upper_cis.vp[i], vp_buf[i], cis_filt_size, 720 - cis_filt_size);
		set_ab_cis_data(data.lower_cis.vp[i], vp_buf[i], 720 + cis_filt_size, 1440 - cis_filt_size);

		set_ab_cis_data(data.upper_cis.rel_vp[i], rel_buf[i], cis_filt_size, 720 - cis_filt_size);
		set_ab_cis_data(data.lower_cis.rel_vp[i], rel_buf[i], 720 + cis_filt_size, 1440 - cis_filt_size);
	}





	return data;
	//print_fix_data(data);
}

cis_data get_std_data(u8 * white, u8 *stdbuf) {
	cis_data  data;
	for (int i = 0; i<1440; i++) {
		for (int k = 0; k<4; k++) {
			vp_buf[k][i] = getAver(stdbuf, image_width, 1968, (k + 3) % 5 * 300 + 5, (k + 3) % 5 * 300 + 150, i, 0, COL);
		}
		vp_buf[4][i] = getAver(white + 1210 * 1440, image_width, 1968, 5, 150, i, 0, COL);
	}

	for (int i = 0; i<5; i++) {
		set_ab_cis_data(data.upper_cis.vp[i], vp_buf[i], cis_filt_size, 720 - cis_filt_size);
		set_ab_cis_data(data.lower_cis.vp[i], vp_buf[i], 720 + cis_filt_size, 1440 - cis_filt_size);
	}
	return data;
}

char posWordTable[2][4] = { "上","下" };
enum {
	upper = 0,
	lower
};
void print_cis_data(ab_cis_data& data, int pos, int index_mask = 0) {
	if (index_mask == 0)
		printf("%s管：		%d		%d	(%d)		%d		%d		%d		%d	(%d)\n", posWordTable[pos],
			data.aver, data.max.val, data.max.index,//data.min.val,data.min.index,
			data.diff, data.mode, data.median, data.min.val, data.min.index);
	else if (index_mask == 1)
		printf("%s管：		%d		%d			%d		%d\n", posWordTable[pos], data.aver, data.max.val, data.min.val, data.diff);

}

void print_fix_data(cis_data& data, const char *title) {
	if (title == 0)
		printf("\033[35;1m\n测试数据\n\033[0m");
	else
		printf("\033[35;1m\n%s\n\033[0m", title);
	printf("\033[35;1m-------------------------------------------------------\n\033[0m");

	printf("\033[36;1mVd\033[0m");
	printf("		平均值		最大值			波动		众数		中位数		最小值\n");
	print_cis_data(data.upper_cis.vd, upper);
	print_cis_data(data.lower_cis.vd, lower);

	for (int i = 0; i<5; i++) {
		printf("\033[36;1mvp:%s\n\033[0m", colorWordTable[i + 1]);
		print_cis_data(data.upper_cis.vp[i], upper);
		print_cis_data(data.lower_cis.vp[i], lower);
	}
	/*	printf("\033[35;1m-------------------------------------------------------\n\033[0m");
	for(int i=0;i<5;i++){
	printf("\033[36;1mcross:%s\n\033[0m",colorWordTable[i+1]);
	print_cis_data(data.upper_cis.vc[i],upper,1);
	print_cis_data(data.lower_cis.vc[i],lower,1);
	}
	*/

	printf("\n");
	printf("\033[35;1m-------------------------------------------------------\n\033[0m");
}

void print_fix_rel_data(cis_data& data, const char *title) {
	//ab_cis_data rel_data;
	if (title == 0)
		printf("\033[35;1m\n测试数据\n\033[0m");
	else
		printf("\033[35;1m\n%s\n\033[0m", title);
	printf("\033[35;1m-------------------------------------------------------\n\033[0m");

	for (int i = 0; i<5; i++) {
		printf("\033[36;1mvp:%s\n\033[0m", colorWordTable[i + 1]);
		print_cis_data(data.upper_cis.rel_vp[i], upper);
		print_cis_data(data.lower_cis.rel_vp[i], lower);
	}

	printf("\n");
	printf("\033[35;1m-------------------------------------------------------\n\033[0m");
}




