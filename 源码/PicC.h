#pragma once

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include<opencv2\opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include <math.h>
#include <CL\cl.h>
#include <ctime>
#include <sys/timeb.h>
#include <vector>
#include "MYComplex.h"
#include "Fourier.h"
#define   PI  3.14159265358979323846

typedef struct {
	double real;
	double imag;
} COMPLEX;

typedef COMPLEX TYPE_FFT;

#define TYPE_FFT_E     double


using namespace cv;



 extern QImage cvMat_to_QImage(const cv::Mat &mat);
 extern QPixmap cvMatToQPixmap(const cv::Mat &inMat);
 extern cv::Mat QImage_to_cvMat(const QImage &image);


extern QPixmap  SaltNoiseDeal(Mat& dealPic, int SaltFactor = 4);
//椒盐噪声多线程分块处理


extern int  sort_Array(int * src, int  middle, int size, int filterSize, int filterMaxSize, int xy);

extern int  Middle_value(int * src, int middle, int size);


extern  QPixmap Median_filtering(Mat dealPic);
//产生随机分布的高斯随机数
extern double ProductGaussianNoise();
//产生高斯噪声
extern QPixmap GaussianNoise(Mat& dealPic, int GaussianFactor = 64);
//多线程
extern void SaltNoiseDeal(Mat& dealPic, int rowsStart, int blockRows, int cols, double SaltFactor);

extern  void Median_filtering(Mat& dealPic, int startRows, int blockRows);

extern void GaussianNoise(Mat& dealPic, int startRows, int blockRows, double GaussianFactor = 64);

extern void  BicubicInterpolation(Mat & dealpic, double scaleFactor);
extern  Mat getBiggerPic(Mat & dealPic,double scaleFactor);
extern  void  BicubicInterpolation(Mat &dealPic, double scaleFactor, int rowsStart, int blockRows, Mat& bigPic);




//extern void  BicubicInterpolation(Mat &dealPic, Mat &bigPic, double scaleFactor, int startRows, int blockRows);
extern Mat getRotatePic(Mat& dealPic, double angle);
extern  void  RotatePictureByBic(Mat & dealPic, double angle = 45);
extern void  RotatePictureByBic(Mat & dealPic, double angle, int rowsStart, int blockRows, Mat& rotatePic);

extern void FourierTransform(Mat &dealPic);
//平滑滤波
extern void SmoothingFilter(Mat &dealPic,  int startRows, int blockRows , int filterSize = 3);

//高斯滤波
extern void GayssianFilter(Mat &dealPic, int startRows, int blockRows, int filterSize = 3);

	//傅里叶变换


extern void wienerFilter(Mat &dealPic, int rowsStart, int  blockRows, int filterSize = 3);


extern "C" void  BicubicInterpolation_host(float *pixel, float *bigPic, float Width, float Height, float factor);
extern "C" void RotatePictureByBic_host(float *pixel, float *rPic, float Width, float Height, float rWidth, float rHeight, float angle);

extern "C" void GayssianFilter_host(float *pixel, float* wight, float Width, float Height, int filterSize);

extern void  BicubicInterpolation_GPU(Mat &dealPic, double scaleFactor);
extern void   RotatePictureByBic_GPU(Mat & dealPic, double angle, Mat& rotatePic);
extern void GayssianFilter_GPU(Mat &dealPic, int filterSize);

extern void GayssianFilterCL(double *pic, double * weight, double width, double height, int filterSize);
extern void GayssianFilter_CL(Mat &dealPic, int filterSize);

extern void BicubicInterpolationCL(double *pixel, double *bigPic, double width, double height, double factor);
extern void BicubicInterpolation_CL(Mat &dealPic, double  scaleFactor);

extern void   RotatePictureByBicCL(double *pixel, double *rPic, double width,
	double height, double rWidth, double rHeight, double angle);
extern void   RotatePictureByBic_CL(Mat & dealPic, double angle, Mat& rotatePic);