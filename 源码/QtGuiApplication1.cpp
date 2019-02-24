#include "QtGuiApplication1.h"
#include "PicC.h"





QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	OnCurrentIndexChange(0);
	//this->setObjectName("dialog");    //这句话一定要有，不然，整个界面上的控件背景都跟界面背景一样
	//this->setStyleSheet("QDialog#dialog{border-image:url(:/Recourse/Resources/background.jpg)}");
	connect(ui.openButton, SIGNAL(clicked(bool)), this, SLOT(OnButton1Clicked(bool)));
	connect(ui.dealButton, SIGNAL(clicked(bool)), this, SLOT(OnHandleButtonClicked(bool)));
	connect(ui.saveButton, SIGNAL(clicked(bool)), this, SLOT(OnSaveButtonClicked(bool)));
	connect(ui.applyButton, SIGNAL(clicked(bool)), this, SLOT(OnApplyButtonClicked()));
	connect(ui.showPicButton, SIGNAL(clicked(bool)), this, SLOT(OnShowPicButtonClicked()));
	connect(ui.dealBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnCurrentIndexChange(int)));
	connect(ui.processBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnCurrentIndexChange2(int)));
	connect(ui.clearButton, SIGNAL(clicked(bool)), this, SLOT(OnClearButtonChange()));
	//connect(ui.)
	//StartThread(mThread, 11);
}


QString str2qstr(const std::string str)
{
	return QString::fromLocal8Bit(str.data());
}

std::string qstr2str(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return std::string(cdata);
}



QString getHandle(int i) {
	switch (i)
	{
	case 0:
		return QString(str2qstr("椒盐噪声"));
	case 1:
		return QString(str2qstr("自适应中值滤波"));
	case 2:
		return QString(str2qstr("高斯噪声"));
	case 3:
		return QString(str2qstr("缩放"));
	case 4:
		return QString(str2qstr("旋转"));
	case 5:
		return QString(str2qstr("傅里叶变换"));
	case 6:
		return QString(str2qstr("平滑线性滤波"));
	case 7:
		return QString(str2qstr("高斯滤波"));
	case 8:
		return QString(str2qstr("维纳滤波"));
	default:
		return "error";
	}
}


void getInfo(Ui::QtGuiApplication1Class  ui, int i) {
	switch (i)
	{
	case 0:
		ui.informBower->setText(QString(str2qstr("椒盐噪声 参数区间为2-10最好")));
		break;
	case 1:
		ui.informBower->setText(QString(str2qstr("自适应中值滤波无参数选择，filter自动变化")));
		break;
	case 2:
		ui.informBower->setText(QString(str2qstr("高斯噪声 参数区间为32 - 512最好")));
		break;
	case 3:
		ui.informBower->setText(QString(str2qstr("放缩 参数格输入放缩大小 默认不填为1  区间0.5--3")));
		break;
	case 4:
		ui.informBower->setText(QString(str2qstr("旋转 参数格输入度数 默认为45")));
		break;
	case 5:
		ui.informBower->setText(QString(str2qstr("傅里叶变换无参数")));
		break;
	case 6:
		ui.informBower->setText(QString(str2qstr("无参数")));
		break;
	case 7:
		ui.informBower->setText(QString(str2qstr("无参数")));
		break;
	case 8:
		ui.informBower->setText(QString(str2qstr("无参数")));
		break;
	default:
		ui.informBower->setText(QString(str2qstr("无参数")));
		break;
	}
}



void StartThread(Ui::QtGuiApplication1Class  ui,  MThread* mThread, int numOfThread, Mat& picture, int select, int selectProcess, float factor = 0) {
	double startTime;
	double endTime;


	if (selectProcess == 0)
		mThread = new MThread[numOfThread]();  //初始化windows线程
	else
		mThread = new MThread();

	//int rows = picture.rows;
	int rows = picture.rows;
	int cols = picture.cols;
	//对图片进行分块
	int blockRows = rows / numOfThread;
	int * blockD = new int[numOfThread + 1];  //储存分块下标
	for (int i = 0, p = 0; i < rows; ) {   //将图片分为等距的几个分区
		blockD[p] = i;
		p++;
		i += blockRows;
		if (i >= rows) {
			blockD[p] = rows - 1;
			break;
		}
	}

	
	//windows多线程
	if (selectProcess == 0) {
		startTime = (double)clock();
		if (select != 3 && select != 4 && select != 0 && select != 2) {
			for (int i = 0; i < numOfThread; i++) {
				mThread[i].start();
				mThread[i].Start(picture, blockD[i], blockRows, cols, select, numOfThread); //平均信噪比  //当比例小于1时向上取整为1
			}
		}
		else if (select == 0 || select == 2) {
			for (int i = 0; i < numOfThread; i++) {
				mThread[i].start();
				mThread[i].Start(picture, blockD[i], blockRows, cols, select, factor); //平均信噪比  //当比例小于1时向上取整为1
			}
		}
		else if (select == 3) {
			Mat biggerPic = getBiggerPic(picture, factor);
			for (int i = 0; i < numOfThread; i++) {
				mThread[i].start();
				mThread[i].Start(picture, blockD[i], blockRows, cols, select, factor, biggerPic);  //处理缩放

			}
			picture = biggerPic;
		}
		else if (select == 4) {
			Mat rotatePic = getRotatePic(picture, factor);
			for (int i = 0; i < numOfThread; i++) {
				mThread[i].start();
				mThread[i].Start(picture, blockD[i], blockRows, cols, select, factor, rotatePic);  //处理缩放

			}
			picture = rotatePic;
		}
		endTime = (double)clock();
		ui.paramentPrintBower->append(getHandle(select) + " -- "  "windowsThread  numOfThread:  " + QString::number(numOfThread) + "  runing time:  " + QString::number(endTime - startTime) + " ms");
	}
	else if(selectProcess == 1) {
		startTime = (double)clock();
		omp_set_num_threads(numOfThread);  //设置openMP的线程数
        #pragma omp parallel for                //使用openMP
		for (int i = 0; i < 1; i++) {
			if (select != 3 && select != 4 && select != 0 && select != 2) {
				mThread[0].start();
				mThread[0].Start(picture, 0, picture.rows, cols, select, numOfThread); //平均信噪比  //当比例小于1时向上取整为1
			}
			else if (select == 0 || select == 2) {
		
					mThread[0].start();
					mThread[0].Start(picture, 0, picture.rows, cols, select, factor / numOfThread);
				
			}
			else if (select == 3) {
				Mat biggerPic = getBiggerPic(picture, factor);
				mThread[0].start();
				mThread[0].Start(picture, 0, picture.rows, cols, select, factor, biggerPic);  //处理缩放
				picture = biggerPic;

			}
			else if (select == 4) {
				Mat rotatePic = getRotatePic(picture, factor);
		
					mThread[0].start();
					mThread[0].Start(picture, 0, picture.rows, cols, select, factor, rotatePic);  //处理缩放

	
				picture = rotatePic;
			}
		}
		endTime = (double)clock();
		ui.paramentPrintBower->append(getHandle(select) + " -- "  "openMP  numOfThread:  " + QString::number(numOfThread) + "  runing time:  " + QString::number(endTime - startTime) + " ms");
	}
	//CUDA
	else if (selectProcess == 2) {
		startTime = (double)clock();
		if (select == 3) {
			mThread[0].start();
			mThread[0].Start(picture, 0, picture.rows, cols, 9, factor);  //处理缩放

		}
		else if (select == 4) {
			Mat rotatePic = getRotatePic(picture, factor);
			mThread[0].start();
			mThread[0].Start(picture, 0, picture.rows, cols, 11, factor, rotatePic);  //处理缩放
			picture = rotatePic;
		
		}
		else if (select == 7)
		{
			mThread[0].start();
			mThread[0].Start(picture, 0, picture.rows, cols, 13, factor);  //处理缩放
		
		}
		endTime = (double)clock();
		ui.paramentPrintBower->append(getHandle(select) + " -- "  "CUDA   runing time:  " + QString::number(endTime - startTime) + " ms");
	}
	//openCL
	else if (selectProcess == 3) {
		startTime = (double)clock();

		if (select == 3) {
			mThread[0].start();
			mThread[0].Start(picture, 0, picture.rows, cols, 10, factor);  //处理缩放
		}
		else if (select == 4) {
			Mat rotatePic = getRotatePic(picture, factor);
			mThread[0].start();
			mThread[0].Start(picture, 0, picture.rows, cols, 12, factor, rotatePic);  //处理缩放
			picture = rotatePic;
		}
		else if (select == 7)
		{
			mThread[0].start();
			mThread[0].Start(picture, 0, picture.rows, cols, 14, factor);  //处理缩放
		}

		endTime = (double)clock();
		ui.paramentPrintBower->append(getHandle(select) + " -- "  "openCL   runing time:  " + QString::number(endTime - startTime) + " ms");
	
	}
	
	/*
	for (int i = 0; i < numOfThread + 1; i++) {
		qDebug() << blockD[i] << "   ";
	}*/



}




int QtGuiApplication1::OnHandleButtonClicked(bool checked) {


	Mat picture = DealPicture.clone();  //读取图片
	QPixmap handlePic;
	float factor;
	if (ui.factorLine->text().isEmpty()) {
		if (ui.dealBox->currentIndex() == 3)
			factor = 1;
		else if (ui.dealBox->currentIndex() == 2)
			factor = 64;
		else if (ui.dealBox->currentIndex() == 4)
			factor = 45;
		else if (ui.dealBox->currentIndex() == 0)
			factor = 2;

	}

	else
		factor = ui.factorLine->text().toFloat();

	int numOfThread = ui.processSlider->value();  //线程数
	int kindOfThread = ui.processBox->currentIndex(); //使用什么线程

	bool loopDeal = ui.dealHTimesBox->isChecked();

	int loop = 1;  //处理次数
	if (loopDeal)   //处理100次
		loop = 100;
	/*double startTime;
	double endTime;*/
	for (int i = 0; i < loop && picture.rows != 0; i++) {
		switch (ui.dealBox->currentIndex())
		{
		case 0:  //椒盐噪声
			//handlePic = SaltNoiseDeal(picture);  //处理图片

			StartThread(ui, mThread, numOfThread, picture, 0, kindOfThread, factor);
			break;
		case 1:  //中值滤波
			//handlePic = Median_filtering(picture);
			StartThread(ui, mThread, numOfThread, picture, 1, kindOfThread);
			break;
		case 2: //高斯噪声
			//handlePic = GaussianNoise(picture);

			StartThread(ui, mThread, numOfThread, picture, 2, kindOfThread, factor);
			break;
		case 3:  //缩放
			//imshow("yuantu", picture);

			StartThread(ui, mThread, numOfThread, picture, 3, kindOfThread, factor);
			imshow("scale", picture);

			//handlePic = FourierTransform(picture);
			break;
		case 4:  //旋转

			StartThread(ui, mThread, numOfThread, picture, 4, kindOfThread, factor);
			imshow("rotate", picture);
			break;
		case 5:  //

			StartThread(ui, mThread, numOfThread, picture, 5, kindOfThread);
			break;
		case 6:

			StartThread(ui, mThread, numOfThread, picture, 6, kindOfThread);
			break;
		case 7:

			StartThread(ui, mThread, numOfThread, picture, 7, kindOfThread);
			break;
		case 8:

			StartThread(ui, mThread, numOfThread, picture, 8, kindOfThread);
			break;
		default:
			break;
		}

			//DealPicture = handlePic;
			//ui.showDealPic->setPixmap(handlePic);
			ui.showDealPic->setPixmap(cvMatToQPixmap(picture));
			tempPicture = picture.clone();//保存结果
		}
		//cvSaveImage("test", picture, "D:");
		//qDebug() << picture;
	
	return 0;
}


int QtGuiApplication1::OnButton1Clicked(bool checked) {
	filePath = QFileDialog::getOpenFileName(this, "open  WORN:cannot include chinese path");
	//ui.lineEdit->setText(filePath);  //打印文件路径、

	if (filePath.isEmpty())
	{
		//文件名为空，返回
		return 0;
	}
	else {
	  
		//QTextCodec *code = QTextCodeccodecForName("GB2312");//解决中文路径问题  
	   //string filename = code->fromUnicode(strFileName).data();
		QPixmap pixmap(filePath);  //加载图片
		DealPicture = imread(filePath.toStdString());  //读取图片
		ui.showChoosePic->setScaledContents(false);  //重置图片缩放设置
		ui.showDealPic->setScaledContents(false);
		if (pixmap.width() > ui.showChoosePic->width() || pixmap.height() > ui.showChoosePic->height()) {
			ui.showChoosePic->setScaledContents(true); //如果图片大于边框 则设置缩放
			ui.showDealPic->setScaledContents(true);
		}

		ui.showChoosePic->setPixmap(pixmap); //显示图片
	
	}

	return 0;
}

int QtGuiApplication1::OnSaveButtonClicked(bool checked) {
	/*
	Mat pic = QImage_to_cvMat(DealPicture.toImage());
	imwrite("F:\test", pic);*/
	if (ui.showDealPic->hasScaledContents()) {
		const QPixmap *savePic = ui.showDealPic->pixmap();

		QString dirPath;//创建文件夹路径
		QString filePath = QFileDialog::getExistingDirectory(this, "");//获取文件夹路径
		QString saveName = "dealPic";//保存图片名字
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		QString str = time.toString("yyyyMMdd_hhmmss"); //设置显示格式
		//QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		//QString str = time.toString("yyyyMMdd_hhmmss"); //设置显示格式
		if (filePath.isEmpty())
		{
			QMessageBox::information(this, "worn", "save fail");
		}
		else
		{
			dirPath = QString("%1/test%2").arg(filePath).arg(str);
			QDir *temp = new QDir;//声明文件对象
			temp->mkdir(dirPath);//创建路径
			QString savePath = QString("%1//%2.jpg").arg(dirPath).arg(saveName);
			savePic->save(savePath);
			QMessageBox::information(this, "message", "save successed");
		}
	}
	return 0;
}

void  QtGuiApplication1:: OnApplyButtonClicked() {
	if (ui.showDealPic->hasScaledContents()) {
		DealPicture = tempPicture.clone();
		ui.showChoosePic->setPixmap(cvMatToQPixmap(DealPicture));
	}
	}


void QtGuiApplication1::OnShowPicButtonClicked() {

	if (ui.showDealPic->hasScaledContents())
		imshow("DealPic", QImage_to_cvMat(ui.showDealPic->pixmap()->toImage()));
}

void  QtGuiApplication1::OnCurrentIndexChange(int) {
	ui.factorLine->clear();
	ui.factorLine->clearFocus();
	ui.dealHTimesBox->clearMask();
	ui.processSlider->setValue(1);
	ui.processBox->setCurrentIndex(0);
	int index = ui.dealBox->currentIndex();

	getInfo(ui, index);

	if (index != 0 && index != 2 && index != 3 && index != 4)
		ui.factorLine->setEnabled(false);
	else
		ui.factorLine->setEnabled(true);

	if (index != 1 && index != 6 && index != 7 && index != 8)
		ui.dealHTimesBox->setEnabled(false);
	else
		ui.dealHTimesBox->setEnabled(true);
	
	if (index != 3 && index != 4 && index != 7) {
		QModelIndex indexe = ui.processBox->model()->index(2, 0);
		QModelIndex indexe2 = ui.processBox->model()->index(3, 0);
		QVariant v(0);
		ui.processBox->model()->setData(indexe, v, Qt::UserRole - 1);
		ui.processBox->model()->setData(indexe2, v, Qt::UserRole - 1);
	}
	else {
		QModelIndex indexe = ui.processBox->model()->index(2, 0);
		QModelIndex indexe2 = ui.processBox->model()->index(3, 0);
		QVariant v(1 | 32);
		ui.processBox->model()->setData(indexe, v, Qt::UserRole - 1);
		ui.processBox->model()->setData(indexe2, v, Qt::UserRole - 1);
	}

	
};


void  QtGuiApplication1::OnCurrentIndexChange2(int) {

	ui.processSlider->setValue(1);
	int index = ui.dealBox->currentIndex();


	qDebug() << "dsadasdasd";
	ui.processSlider->setEnabled(true);

	if (index == 3 || index == 4 || index == 7) {
		int index2 = ui.processBox->currentIndex();
		if (index2 == 2 || index2 == 3) {
			ui.processSlider->setEnabled(false);
		}	
	}

};

void QtGuiApplication1::OnClearButtonChange() {
	ui.paramentPrintBower->clear();
}