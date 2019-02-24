#include "MThread.h"

int MThread::numOfThread = 0;

MThread::MThread()
{
}


void MThread::run() {


};


void MThread::stop() {
	numOfThread--;
	this->quit();

};

void MThread::Start(Mat& dealPic, int rowsStart,int blockRows, int cols,int select, float Factor, Mat& conPic ) {
	
	

	switch (select)
	{
	case 0:
		SaltNoiseDeal(dealPic, rowsStart, blockRows, cols,Factor);  //椒盐噪声处理
		break;
	case 1:
		Median_filtering(dealPic, rowsStart, blockRows);  //自适应中值滤波
		break;
	case 2:
		GaussianNoise(dealPic, rowsStart, blockRows ,Factor);//, (int)ceil(SaltFactor * 16 / (float)numOfThread)
		break;
	case 3:
		BicubicInterpolation(dealPic, Factor, rowsStart, blockRows, conPic);
		//BicubicInterpolation(dealPic, cP, 2, rowsStart, blockRows);
		break;
	case 4:
		RotatePictureByBic(dealPic, Factor, rowsStart, blockRows,conPic);
		//RotatePictureByBic(dealPic,45);
		break;
	case 5:
		FourierTransform(dealPic);
		break;
	case 6:
		SmoothingFilter(dealPic, rowsStart, blockRows);
		break;
	case 7:
		GayssianFilter(dealPic, rowsStart, blockRows);
		break;
	case 8:
		wienerFilter(dealPic, rowsStart, blockRows);
		break;
	case 9:
		BicubicInterpolation_GPU(dealPic, Factor);
		break;
	case 10:
		BicubicInterpolation_CL(dealPic, Factor);
		break;
	case 11:
		RotatePictureByBic_GPU(dealPic, Factor, conPic);
		break;
	case 12:
		RotatePictureByBic_CL(dealPic, Factor, conPic);
		break;
	case 13:
		GayssianFilter_GPU(dealPic, 3);
		break;
	case 14:
		GayssianFilter_CL(dealPic, 3);
		break;
	default:
		break;
	}
	

	
	qDebug() <<"num:" <<numOfThread++;
	this->stop();
}

