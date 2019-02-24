#pragma once
#include <qthread.h>

#include "PicC.h"


class MThread :
	public QThread
{
	Q_OBJECT
public:
	MThread();
	virtual void run();
	void stop();

	static int numOfThread;

	void Start(Mat& dealPic, int rowsStart, int blockRows, int cols, int select,float Factor = 4, Mat& conPic = Mat());
	// void  SaltNoiseDeal(Mat& dealPic, int rowsStart, int rowsEnd, int cols, int SaltFactor = 4);
signals:

	void send(QString msg);
private:

};

