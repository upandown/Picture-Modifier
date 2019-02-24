#pragma once

#include <QtWidgets/QMainWindow>
#include <qdebug.h>
#include "ui_QtGuiApplication1.h"
#include<qfiledialog.h>
#include<opencv2\opencv.hpp>
#include <MThread.h>
#include <qmessagebox.h>
#include <qdatetime.h>
#include <omp.h>
#include <string>
using namespace cv;

class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT
		
	//connect(ui.button, SIGNAL(clicked))

public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGuiApplication1Class ui;
	QString filePath;
	Mat DealPicture;
	Mat tempPicture;
	MThread * mThread;


private slots:
	int OnButton1Clicked(bool checked);
	int OnHandleButtonClicked(bool checked);
	int OnSaveButtonClicked(bool checked);
	void OnApplyButtonClicked();
	void OnShowPicButtonClicked();
	void OnCurrentIndexChange(int);
	void OnCurrentIndexChange2(int);
	void OnClearButtonChange();
};
