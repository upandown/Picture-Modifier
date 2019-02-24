#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#define PI 3.14159265358979323846

inline void checkCudaErrors(cudaError err) //cuda Error handle Function
{    
	if (cudaSuccess != err)
	{
		fprintf(stderr, "CUDA Runtime API error:%s.\n", cudaGetErrorString(err));
		return;
	}
}

//
// __device__ double abs(double a) {
//	 if (a > 0)
//		 return a;
//	 else
//		 return -a;
//}
//
// __device__ double pow(double a, int b) {
//	 double sum = a;
//	 for (int i = 1; i < b; i++)
//		 sum *= a;
//	 if (b == 0)
//		 return 1;
//	 else
//		 return sum;
// }
//
__device__ void getW_xy_a(double *n, double p) {
	int a = -0.5;
	int pn = (int)p;  //�����������
	double u = p - pn;  //���С������
	double np[4];
	np[0] = 1 + u;   //>1
	np[1] = u;        //<1
	np[2] = 1 - u;     //<1
	np[3] = 2 - u;   //>1
	n[0] = a * pow(abs(np[0]), 3) - 5 * a * pow(abs(np[0]), 2) + 8 * a * abs(np[0]) - 4 * a;
	n[1] = (a + 2) * pow(abs(np[1]), 3) - (a + 3) * pow(abs(np[1]), 2) + 1;
	n[2] = (a + 2) * pow(abs(np[2]), 3) - (a + 3) * pow(abs(np[2]), 2) + 1;
	n[3] = a * pow(abs(np[3]), 3) - 5 * a * pow(abs(np[3]), 2) + 8 * a * abs(np[3]) - 4 * a;
};


__global__ void extendPic(double *pixel, double *bigPic, double width, double height, double factor) {

	//int y = blockDim.y * blockIdx.y + threadIdx.y;
	//int x = blockDim.x * blockIdx.x + threadIdx.x;  //��������

	int y = blockDim.y * blockIdx.y + threadIdx.y;
	int x = blockDim.x * blockIdx.x + threadIdx.x;  //��������

	double bWidth = factor * width;
	double bHeight = factor * height;

	if (x >= 2 && x < bWidth - 4 && y >= 2  && y < bHeight - 4) {
		double bI = (double)y * 1 / factor; //�Ŵ�ǰ��λ�� y
		double bJ = (double)x * 1 / factor; // x

		double Wx[4], Wy[4];  //xy��Ȩ��
		getW_xy_a(Wx, bI);
		getW_xy_a(Wy, bJ);

		double element = 0;

		for (int a = 0; a < 4; a++) {
			for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
				/*element += (Vec3f)(dealPic.at<cv::Vec3b>((int)bI + a - 1, (int)bJ + b - 1)) * Wx[a] * Wy[b];*/
			/*	if ((bJ + (b - 1)) >= 0 && (bJ + (b - 1)) < width  && (bI + a - 1) >= 0 && (bI + a - 1) < height)*/
					element += pixel[(int)(bI + a - 1) * (int)width + (int)(bJ + (b - 1))] * Wx[a] * Wy[b];
			}
		}

		bigPic[(int)(y * bWidth + x)] = element;

	}
	/*else
		bigPic[int(y * bWidth + x)] = 0;*/

}

__global__ void rotatePic(double *pixel, double *rPic, double width,
	double height, double rWidth, double rHeight, double angle) {

	angle = angle / 180.0f * PI;
	double cosa = cos(angle);
	double sina = sin(angle);

	int y = blockDim.y * blockIdx.y + threadIdx.y;
	int x = blockDim.x * blockIdx.x + threadIdx.x;  //��������

	//����ӳ��Ѱ��ԭͼ�е�λ��
	int Rx = round(double(y - rHeight / 2.0f)*cosa - double(x - rWidth / 2.0f)*sina + height / 2.0f);
	int Ry = round(double(y - rHeight / 2.0f)*sina + double(x - rWidth / 2.0f)*cosa + width / 2.0f);


	double element = 0; //����ֵ
	double Wx[4], Wy[4];  //xy��Ȩ��

	getW_xy_a(Wx, Rx);
	getW_xy_a(Wy, Ry);
	//���������صĴ�С
	if (x >= 0 && x < rWidth  && y >= 0 && y < rHeight ) {
		for (int a = 0; a < 4; a++) {
			for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
				if (Rx + a - 1 < 0 || Rx + a - 1 >= height || Ry + b - 1 < 0 || Ry + b - 1 >= width)    //��ֹԽ�� ����ԭͼ��
					rPic[y * (int)rWidth + x] = 0;
				else
				{
					element += pixel[int((Rx + a - 1) * width) + ((int)Ry + b - 1)] * Wx[a] * Wy[b];
				}
			}
		}
		rPic[y * (int)rWidth + x] = element;
	}
}

__global__ void gayFilter(double *pixel, double* wight,  double Width, double Height, int filterSize) {
	int skipSize = filterSize / 2; //�����������


	int y = blockDim.y * blockIdx.y + threadIdx.y;
	int x = blockDim.x * blockIdx.x + threadIdx.x;  //��������

	if (x >= skipSize && x < Width - 1 - filterSize && y >= skipSize && y < Height - 1 - filterSize) { //��ֹԽ��
	
		double * filterArray = (double*)malloc(sizeof(double) * filterSize * filterSize);  //�洢������ڵ�����ֵ
		int c_cout = 0;
		for (int k = skipSize, count_i = 0; count_i < filterSize; count_i++) {
			// i -1 j -1    i - 0, j - 1 , i +1, j -1 
			// 0 1 2 3 4 5 p[5]
			//�����ص��������
			for (int p = skipSize, count_j = 0; count_j < filterSize; count_j++) {
				filterArray[c_cout] = pixel[ (y - k) * (int)Width + x - p];
				p--;
				c_cout++;
			}
			k--;
		}
		double value = 0;
		//��Ȩ���
		for (int i = 0; i < filterSize* filterSize; i++)
			value += filterArray[i] * wight[i];

		pixel[y * (int)Width + x] = value;
		free(filterArray);

	}

}


extern "C" void  BicubicInterpolation_host(double *pixel, double *bigPic, double Width, double Height, double factor) {
	double * oPic;
	double * bPic;
	//double * rPic;

	checkCudaErrors(cudaMalloc((void**)&oPic, sizeof(double) * Width * Height));
	checkCudaErrors(cudaMalloc((void**)&bPic, sizeof(double) * Width * Height * factor * factor));
	//checkCudaErrors(cudaMalloc((void**)&rPic, (sizeof(double) * Width * Height * factor * factor)));
	checkCudaErrors(cudaMemcpy(oPic, pixel, sizeof(double) * Width * Height, cudaMemcpyHostToDevice));

	int length = 12;
	dim3 dimGrid(Width * factor / length, Height * factor / length);
	/*dim3 dimGrid((Width * factor + dimBlock.x - 1) / dimBlock.x, (Height * factor + dimBlock.y -
		1) / dimBlock.y);*/
	dim3 dimBlock(length, length);
	extendPic << <dimGrid, dimBlock >> > (oPic, bPic, Width, Height, factor);

	checkCudaErrors(cudaMemcpy(bigPic, bPic, sizeof(double) * Width * Height * factor * factor, cudaMemcpyDeviceToHost));

	cudaFree(oPic);
	cudaFree(bPic);
	//cudaFree(rPic);
}

extern "C" void RotatePictureByBic_host(double *pixel, double *rPic, double Width, 
	double Height, double rWidth, double rHeight, double angle) {
	double * oPic;
	double * bPic;


	checkCudaErrors(cudaMalloc((void**)&oPic, sizeof(double) * Width * Height));  //ԭ��ͼƬ��С
	checkCudaErrors(cudaMalloc((void**)&bPic, sizeof(double) * rWidth * rHeight));//��תͼƬ��С

	checkCudaErrors(cudaMemcpy(oPic, pixel, sizeof(double) * Width * Height, cudaMemcpyHostToDevice));//��ֵ����cup��gpu
	int length = 32;
	dim3 dimGrid(length , length ); //�ڴ������С
	///*dim3 dimGrid((Width * factor + dimBlock.x - 1) / dimBlock.x, (Height * factor + dimBlock.y -
	//	1) / dimBlock.y);*/
	dim3 dimBlock(length,length );
	rotatePic << <dimGrid, dimBlock >> > (oPic, bPic, Width, Height, rWidth, rHeight,  angle);

	checkCudaErrors(cudaMemcpy(rPic, bPic, sizeof(double) * rWidth * rHeight, cudaMemcpyDeviceToHost));

	cudaFree(oPic);
	cudaFree(bPic);
}


extern "C" void GayssianFilter_host(double *pixel,double* wight, double Width, double Height, int filterSize) {
	double * oPic;
	double * Wight;
	double * filterArray;

	checkCudaErrors(cudaMalloc((void**)&oPic, sizeof(double) * Width * Height));  //ԭ��ͼƬ��С
	checkCudaErrors(cudaMalloc((void**)&Wight, sizeof(double) * filterSize * filterSize));//Ȩ�ؾ����С
	checkCudaErrors(cudaMalloc((void**)&filterArray, sizeof(double) * filterSize * filterSize));//���ھ����С

	checkCudaErrors(cudaMemcpy(oPic, pixel, sizeof(double) * Width * Height, cudaMemcpyHostToDevice));//��ֵ����cup��gpu
	checkCudaErrors(cudaMemcpy(Wight, wight, sizeof(double) * filterSize * filterSize, cudaMemcpyHostToDevice));//��ֵ����cup��gpu
	int length = 32;
	dim3 dimGrid(length, length); //�ڴ������С
	///*dim3 dimGrid((Width * factor + dimBlock.x - 1) / dimBlock.x, (Height * factor + dimBlock.y -
	//	1) / dimBlock.y);*/
	dim3 dimBlock(length, length);
	//
	gayFilter<<<dimGrid, dimBlock >> > (oPic, Wight,  Width, Height, filterSize);

	checkCudaErrors(cudaMemcpy(pixel , oPic, sizeof(double) * Width * Height, cudaMemcpyDeviceToHost));

	cudaFree(oPic);
	cudaFree(Wight);
	cudaFree(filterArray);
}