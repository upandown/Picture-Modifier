#include "Fourier.h"

void FFT(MYComplex *in, MYComplex*out, int n){
	int length = pow(2, n);

	int size = sizeof(MYComplex)*length;   //数组长度 =  指针size * length 
	MYComplex* W = (MYComplex*)malloc(size / 2);  //w为fft的旋转因子
	MYComplex* X1 = (MYComplex*)malloc(size);
	MYComplex* X2 = (MYComplex*)malloc(size);
	MYComplex* X = NULL;                  //交换用的临时变量

	int  bit;
	int  p;
	double angle = 0;

   //计算旋转因子
	for (int i = 0; i < length / 2; ++i){
		W[i] = MYComplex(cos(angle), -sin(angle));
		angle += 2 * PI / length;;
	}

	for (int i = 0; i < length; ++i){   //赋值
		X1[i] = in[i];
	}

	//码位倒置，由于FFT的蝶形算法会导致结果的码位错乱，需要从结果向开始处推倒，从而得到顺序的结果
	for (int k = 0; k < n; ++k){
		for (int j = 0; j <(int)pow(2, k); ++j){
			bit = pow(2, (n - k));
			for (int i = 0; i < bit / 2; ++i){
				p = j * bit;
				X2[i + p] = X1[i + p] + X1[i + p + bit / 2];
				X2[i + p + bit / 2] = (X1[i + p] - X1[i + p + bit / 2])* W[i * (int)pow(2,k)];
			}
		}
		X = X1;
		X1 = X2;
		X2 = X;
	}

	for (int j = 0; j < length; ++j){
		p = 0;
		for (int i = 0; i < n; ++i){
			if (j& int(pow(2, i))){
				p += pow(2, (n - i - 1));
			}
		}
		out[j] = X1[p];
	}

	free(W);
	free(X1);
	free(X2);
}
