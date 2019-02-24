#include "Fourier.h"

void FFT(MYComplex *in, MYComplex*out, int n){
	int length = pow(2, n);

	int size = sizeof(MYComplex)*length;   //���鳤�� =  ָ��size * length 
	MYComplex* W = (MYComplex*)malloc(size / 2);  //wΪfft����ת����
	MYComplex* X1 = (MYComplex*)malloc(size);
	MYComplex* X2 = (MYComplex*)malloc(size);
	MYComplex* X = NULL;                  //�����õ���ʱ����

	int  bit;
	int  p;
	double angle = 0;

   //������ת����
	for (int i = 0; i < length / 2; ++i){
		W[i] = MYComplex(cos(angle), -sin(angle));
		angle += 2 * PI / length;;
	}

	for (int i = 0; i < length; ++i){   //��ֵ
		X1[i] = in[i];
	}

	//��λ���ã�����FFT�ĵ����㷨�ᵼ�½������λ���ң���Ҫ�ӽ����ʼ���Ƶ����Ӷ��õ�˳��Ľ��
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
