#include "PicC.h"


//��Matת����QImageģʽ
cv::Mat QImage_to_cvMat(const QImage &image) {
	switch (image.format())
	{
		// 8-bit, 4 channel
	case QImage::Format_RGB32:
	{
		cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
		return mat;
	}

	// 8-bit, 3 channel
	case QImage::Format_RGB888:
	{
		//����Qimage ��mat RGB��һ�� ��Ҫת��  QImage��rgb���ж�Mat��bgr����
		QImage swapped = image.rgbSwapped();
		return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
	}

	// 8-bit, 1 channel
	case QImage::Format_Indexed8:
	{
		cv::Mat  mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());

		return mat;
	}

	default:
		qDebug("Image format error: depth=%d , %d format\n", image.depth(), image.format());
		break;
	}

	return cv::Mat();
}
//��QImageת����Matģʽ
QImage cvMat_to_QImage(const cv::Mat &mat) {
	switch (mat.type())
	{
		// 8-bit, 4 channel
	case CV_8UC4:
	{
		QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
		return image;
	}

	// 8-bit, 3 channel
	case CV_8UC3:
	{
		QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}

	// 8-bit, 1 channel
	case CV_8UC1:
	{
		static QVector<QRgb>  sColorTable;
		// only create our color table once
		if (sColorTable.isEmpty())
		{
			for (int i = 0; i < 256; ++i)
				sColorTable.push_back(qRgb(i, i, i));
		}
		QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
		image.setColorTable(sColorTable);
		return image;
	}

	default:
		qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
		break;
	}
	return QImage();
}
//��Mat��ΪQPixmapģʽ
QPixmap cvMatToQPixmap(const cv::Mat &inMat)
{
	return QPixmap::fromImage(cvMat_to_QImage(inMat));
}


//���н�����������ĺ���
static QPixmap  SaltNoiseDeal(Mat& dealPic, int SaltFactor) {
	SaltFactor *= 10000;
	for (int p = 0; p < SaltFactor; p++) {
		int i = rand() % dealPic.rows;  //������ɶ�ά����
		int j = rand() % dealPic.cols;
		int noisePoint = (rand() % 2 == 0) ? 0 : 255; //�������0��255������ֵ

		if (dealPic.channels() == 1) {        //�ǵ�ͨ����ֱ�Ӹ�ֵ
			dealPic.at<cv::Vec3b>(i, j) = noisePoint;
		}
		else {
			dealPic.at<cv::Vec3b>(i, j)[0] = noisePoint;
			dealPic.at<cv::Vec3b>(i, j)[1] = noisePoint;
			dealPic.at<cv::Vec3b>(i, j)[2] = noisePoint;
		}

	}
	return cvMatToQPixmap(dealPic);  //���ش����ͼƬ
	//qDebug() << filePath;
	//imwrite(filePath.toStdString(), dealPic);


}


void SaltNoiseDeal(Mat& dealPic, int rowsStart, int blockRows, int cols, double SaltFactor) {
	//SaltFactor *= 10000;
	double numOfThread = dealPic.rows/ (double)blockRows ;
	qDebug() << "numOfThread: " << numOfThread;
	SaltFactor = dealPic.rows * dealPic.cols / numOfThread /10 *  SaltFactor;
	//qDebug() << SaltFactor;
	for (int p = 0; p < SaltFactor; p++) {
		int i = rand() % blockRows + rowsStart;  //������ɶ�ά����
		int j = rand() % cols;
		int noisePoint = (rand() % 2 == 0) ? 0 : 255; //�������0��255������ֵ

		if (dealPic.channels() == 1) {        //�ǵ�ͨ����ֱ�Ӹ�ֵ
			dealPic.at<cv::Vec3b>(i, j) = noisePoint;
		}
		else {
			dealPic.at<cv::Vec3b>(i, j)[0] = noisePoint;
			dealPic.at<cv::Vec3b>(i, j)[1] = noisePoint;
			dealPic.at<cv::Vec3b>(i, j)[2] = noisePoint;
		}

	}
	//return cvMatToQPixmap(dealPic);  //���ش����ͼƬ
	//qDebug() << filePath;
	//imwrite(filePath.toStdString(), dealPic);


}


int Middle_value(int * src, int middle, int size) {


	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (src[i] < src[j])    //���ǰһ��Ԫ��С�ں�һ��Ԫ��
			{
				int temp;        //��ʱ����
				temp = src[i];
				src[i] = src[j]; //���Ԫ�ص�ǰһ��λ��
				src[j] = temp;   //С��Ԫ�ص���һ��λ��
			}
		}
	}
	// �����Ԫ��
	return src[middle];
}


int  sort_Array(int * src, int  middle, int size, int filterSize, int filterMaxSize, int xy) {


	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (src[i] < src[j])    //���ǰһ��Ԫ��С�ں�һ��Ԫ��
			{
				int temp;        //��ʱ����
				temp = src[i];
				src[i] = src[j]; //���Ԫ�ص�ǰһ��λ��
				src[j] = temp;   //С��Ԫ�ص���һ��λ��
			}
		}
	}
	// �����Ԫ��
	int A1 = src[middle] - src[size - 1];  //
	int A2 = src[middle] - src[0];
	if (A1 > 0 && A2 < 0) {
		int	B1 = xy - src[size - 1];    //��ֵ��������  �����жϴ���ĵ��ǲ�������
		int B2 = xy - src[0];
		if (B1 > 0 && B2 < 0)        //����ĵ㲻������
			return xy;
		else
			return src[middle];

	}
	else {
		if (filterSize + 2 > filterMaxSize)   //��ֵ������ ���󴰿�
			return src[middle];
		else
			return -1; //��Ҫ���󴰿ڳߴ�
	}

}



//����Ӧ��ֵ�˲�

 QPixmap Median_filtering(Mat dealPic) {
	int filterSize = 3;   //����˴�С
	int filterMaxSize = 11;   //��������С
	int skipSize = filterSize / 2; //�����������

	qDebug() << "ͨ����" << dealPic.channels() << endl;

	qDebug() << "duotongdaochuli";
	//������߽��ϵ����ؽ��д����ֹԽ��
		//for (int i = skipSize, int j = skipSize, int count = 0; count < elementCount; count++) {
	for (int channel = 0; channel < dealPic.channels(); channel++) {
		for (int i = skipSize; i < dealPic.rows - 1 - filterSize; i++) {

			for (int j = skipSize; j < dealPic.cols - 1 - filterSize; j++) {

			rehandle:
				int c_cout = 0;

				int skipSize2 = filterSize / 2; //�����������
				int * filterArray = new int[filterSize * filterSize];  //�����������˴�С������


				for (int k = skipSize2, count_i = 0; count_i < filterSize; count_i++) {
					// i -1 j -1    i - 0, j - 1 , i +1, j -1 
					// 0 1 2 3 4 5 p[5]
					//�����ص��������
					for (int p = skipSize2, count_j = 0; count_j < filterSize; count_j++) {
						filterArray[c_cout] = dealPic.at<cv::Vec3b>(i - p, j - k)[channel];

						p--;
						c_cout++;
					}
					k--;
				}
				int middle_value = filterSize * filterSize / 2;  //��ȡ�������������ֵ

				int backValue = sort_Array(filterArray, middle_value, filterSize * filterSize, filterSize, filterMaxSize, dealPic.at<cv::Vec3b>(i, j)[channel]);
				if (backValue >= 0) {

					dealPic.at<cv::Vec3b>(i, j)[channel] = backValue;  //���¸�ֵ=�˲�
					delete[]filterArray;   //�ͷ��ڴ�
					filterSize = 3;      //��ԭ���ڴ�С
				}
				else {   //��Ҫ���󴰿ڳߴ�
					filterSize += 2;   //���󴰿ڳߴ�
					if (i - filterSize / 2 < 0 || i + filterSize / 2 > dealPic.rows - 1 || j - filterSize / 2 < 0 || j + filterSize / 2 > dealPic.cols - 1);//��ֹԽ��
					{   filterSize -= 2;  //Խ��Ͱ�3*3����
					dealPic.at<cv::Vec3b>(i, j)[channel] = Middle_value(filterArray, middle_value, filterSize * filterSize);
					filterSize = 3;      //��ԭ���ڴ�С
					delete[]filterArray;   //�ͷ��ڴ�
					continue;
					}

					delete[]filterArray;   //�ͷ��ڴ�
					goto rehandle;
				}


			}

		}



	}
	return cvMatToQPixmap(dealPic);
}

 //����Ӧ��ֵ�˲����߳�

 void Median_filtering(Mat& dealPic , int startRows , int blockRows) {
	 int filterSize = 3;   //����˴�С
	 int filterMaxSize = 11;   //��������С
	 int skipSize = filterSize / 2; //�����������

	/* qDebug() << "ͨ����" << dealPic.channels() << endl;

	 qDebug() << "duotongdaochuli";*/
	 //������߽��ϵ����ؽ��д����ֹԽ��
		 //for (int i = skipSize, int j = skipSize, int count = 0; count < elementCount; count++) {
	 for (int channel = 0; channel < dealPic.channels(); channel++) {
		 int i = 0;
		 startRows == 0 ? i = skipSize : i = startRows;
		 int p = 0;
		 (startRows + blockRows) >= dealPic.rows ? (p = dealPic.rows - 1 - filterSize) : (p = startRows + blockRows);
		 for (; i < p; i++) {

			 for (int j = skipSize; j < dealPic.cols - 1 - filterSize; j++) {

			 rehandle:
				 int c_cout = 0;

				 int skipSize2 = filterSize / 2; //�����������
				 int * filterArray = new int[filterSize * filterSize];  //�����������˴�С������


				 for (int k = skipSize2, count_i = 0; count_i < filterSize; count_i++) {
					 // i -1 j -1    i - 0, j - 1 , i +1, j -1 
					 // 0 1 2 3 4 5 p[5]
					 //�����ص��������
					 for (int p = skipSize2, count_j = 0; count_j < filterSize; count_j++) {
						 filterArray[c_cout] = dealPic.at<cv::Vec3b>(i - p, j - k)[channel];

						 p--;
						 c_cout++;
					 }
					 k--;
				 }
				 int middle_value = filterSize * filterSize / 2;  //��ȡ�������������ֵ

				 int backValue = sort_Array(filterArray, middle_value, filterSize * filterSize, filterSize, filterMaxSize, dealPic.at<cv::Vec3b>(i, j)[channel]);
				 if (backValue >= 0) {

					 dealPic.at<cv::Vec3b>(i, j)[channel] = backValue;  //���¸�ֵ=�˲�
					 delete[]filterArray;   //�ͷ��ڴ�
					 filterSize = 3;      //��ԭ���ڴ�С
				 }
				 else {   //��Ҫ���󴰿ڳߴ�
					 filterSize += 2;   //���󴰿ڳߴ�
					 if (i - filterSize / 2 < 0 || i + filterSize / 2 > dealPic.rows - 1 || j - filterSize / 2 < 0 || j + filterSize / 2 > dealPic.cols - 1);//��ֹԽ��
					 {   filterSize -= 2;  //Խ��Ͱ�3*3����
					 dealPic.at<cv::Vec3b>(i, j)[channel] = Middle_value(filterArray, middle_value, filterSize * filterSize);
					 filterSize = 3;      //��ԭ���ڴ�С
					 delete[]filterArray;   //�ͷ��ڴ�
					 continue;
					 }

					 delete[]filterArray;   //�ͷ��ڴ�
					 goto rehandle;
				 }


			 }

		 }



	 }
	
 }

//��˹����
 double ProductGaussianNoise()
 {
	 double pi = 3.1415926;      //�����

	 static bool flag = false;
	 static double rand1, rand2;

	 if (flag)
	 {
		 flag = false;
		 return sqrt(rand1) * sin(rand2);
	 }

	 flag = true;
	 //�����������
	 rand1 = rand() / ((double)RAND_MAX);
	 if (rand1 < 1e-100) rand1 = 1e-100;


	 rand1 = -2 * log(rand1);  //�����������
	 rand2 = (rand() / ((double)RAND_MAX)) * 2 * pi;

	 return sqrt(rand1) * cos(rand2);
 }

 QPixmap GaussianNoise(Mat& dealPic, int GaussianFactor)
 {

	 int channels = dealPic.channels();  //ͨ����
	 int rows = dealPic.rows;       //����
	 int cols = dealPic.cols * channels;   //�� * ͨ���� = ʵ��һ�е�������


	 uchar* pic;
	 for (int i = 0; i < rows; ++i) {
		 pic = dealPic.ptr<uchar>(i);     //��ȡͼƬ�ĵ�i��
		 for (int j = 0; j < cols; ++j) {
			 double noise = pic[j] + ProductGaussianNoise() * GaussianFactor;
			 (noise < 0) ? (noise = 0) : ((noise > 255) ? noise = 255 : noise = noise);
			 pic[j] = (uchar)noise;
		 }
	 }
	 return cvMatToQPixmap(dealPic);  //���ش����ͼƬ

 }


 //���̵߳ĸ�˹����
 void GaussianNoise(Mat& dealPic, int startRows, int blockRows,double GaussianFactor )
 {

	 int channels = dealPic.channels();  //ͨ����
	// int rows = dealPic.rows;       //����
	 int cols = dealPic.cols * channels;   //�� * ͨ���� = ʵ��һ�е�������
	

	 uchar* pic;
	 for (int i = startRows; i <  startRows +  blockRows; ++i) {
		 pic = dealPic.ptr<uchar>(i);     //��ȡͼƬ�ĵ�i��
		 for (int j = 0; j < cols; ++j) {
			 double noise = pic[j] + ProductGaussianNoise() * GaussianFactor;
			 (noise < 0) ? (noise = 0) : ((noise > 255) ? noise = 255 : noise = noise);
			 pic[j] = (uchar)noise;
		 }
	 }
	
 }

 //���̵߳����ײ�ֵ�Ŵ�

//��Bicubic����
 //�ĸ�����Ϊ 1+u u 1-u 2-u
 void getW_xy(double *n, double p) {
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
 }

 //���߳�ģ��
 void  BicubicInterpolation(Mat &dealPic, double scaleFactor) {
	 double BRows = dealPic.rows * scaleFactor;  //������ͼƬ�ĳ���
	 double BCols = dealPic.cols * scaleFactor;

	 Mat bigPic = Mat(BRows, BCols, CV_8UC3); //���ɷŴ��ͼƬ
	 
	 for (int i = 2; i < BRows - 4; i++) {  //������ȡ4*4����  ��ֹԽ�����Դ�2��ʼ
		 for (int j = 2; j < BCols - 4; j++) {
			 double bI = i * (dealPic.rows / BRows); //�Ŵ�ǰ��λ��
			 double bJ = j * (dealPic.cols / BCols);

			 Vec3f element = { 0,0,0 }; //����ֵ
			 double Wx[4], Wy[4];  //xy��Ȩ��


			 getW_xy(Wx, bI);
			 getW_xy(Wy, bJ);
			 //���������صĴ�С
			 for (int a = 0; a < 4; a++) {
				 for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
					 element += (Vec3f)(dealPic.at<cv::Vec3b>((int)bI + a - 1, (int)bJ + b - 1)) * Wx[a] * Wy[b];
				 }
			 }
			 bigPic.at<cv::Vec3b>(i, j) = element;
		 }
	 }
	 dealPic = bigPic;
	 qDebug() << "extendImage";
	 //namedWindow("BiggerPic", 0);
	 //imshow("BiggerPic", bigPic);
	// dealPic = bigPic;
	// waitKey(0);
 
 };

 Mat getBiggerPic(Mat & dealPic, double scaleFactor) {
 
	 double BRows = dealPic.rows * scaleFactor;  //������ͼƬ�ĳ���
	 double BCols = dealPic.cols * scaleFactor;

	 Mat bigPic = Mat(BRows, BCols, CV_8UC3); //���ɷŴ��ͼƬ
	 return bigPic;
 }

 void  BicubicInterpolation(Mat &dealPic, double scaleFactor , int rowsStart, int blockRows, Mat& bigPic) {
	 //double BRows = dealPic.rows * scaleFactor;  //������ͼƬ�ĳ���
	 double BRows = dealPic.rows * scaleFactor;
	 double BCols = dealPic.cols * scaleFactor;

	 //Mat bigPic = Mat(BRows, BCols, CV_8UC3); //���ɷŴ��ͼƬ

	 int bottom = (blockRows + rowsStart) * scaleFactor;
	 int top = rowsStart * scaleFactor;
	 if (rowsStart == 0)
		 top = 2;
	 if (blockRows + rowsStart == dealPic.rows)
		 bottom -= 4;

	 for (int i = top; i < bottom; i++) {  //������ȡ4*4����  ��ֹԽ�����Դ�2��ʼ
		 for (int j = 2; j < BCols - 4; j++) {
			 double bI = i * (dealPic.rows / BRows); //�Ŵ�ǰ��λ��
			 double bJ = j * (dealPic.cols / BCols);

			 Vec3f element = { 0,0,0 }; //����ֵ
			 double Wx[4], Wy[4];  //xy��Ȩ��


			 getW_xy(Wx, bI);
			 getW_xy(Wy, bJ);
			 //���������صĴ�С
			 for (int a = 0; a < 4; a++) {
				 for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
					 element += (Vec3f)(dealPic.at<cv::Vec3b>((int)bI + a - 1, (int)bJ + b - 1)) * Wx[a] * Wy[b];
				 }
			 }
			 bigPic.at<cv::Vec3b>(i, j) = element;
		 }
	 }
	 /*if (rowsStart + blockRows == dealPic.rows)
		 dealPic = bigPic;*/

	 //dealPic = bigPic;
	 //qDebug() << "extendImage";
	 //namedWindow("BiggerPic", 0);
	 //imshow("BiggerPic", bigPic);
	// dealPic = bigPic;
	// waitKey(0);

 };
 /*
 //���߳�
 void  BicubicInterpolation(Mat &dealPic, Mat &bigPic , double scaleFactor , int startRows , int blockRows ) {
	 double BRows = blockRows * scaleFactor;  //������ͼƬ�ĳ���
	 double BCols = dealPic.cols * scaleFactor;

	 //Mat bigPic = Mat(BRows, BCols, CV_8UC3); //���ɷŴ��ͼƬ

	 int i = 0;
	 startRows == 0 ? i = 2 : i = startRows * scaleFactor;
	 int p = 0;
	 (startRows + blockRows) >= dealPic.rows ? (p = dealPic.rows * scaleFactor - 4) : (p = (startRows + blockRows) * scaleFactor);


	 for (; i < p; i++) {  //������ȡ4*4����  ��ֹԽ�����Դ�2��ʼ
		 for (int j = 2; j < BCols - 4; j++) {
			 double bI = i * (dealPic.rows / BRows); //�Ŵ���λ��
			 double bJ = j * (dealPic.cols / BCols);

			 Vec3f element = { 0,0,0 }; //����ֵ
			 double Wx[4], Wy[4];  //xy��Ȩ��


			 getW_xy(Wx, bI);
			 getW_xy(Wy, bJ);
			 //���������صĴ�С
			 for (int a = 0; a < 4; a++) {
				 for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
					 element += (Vec3f)(dealPic.at<cv::Vec3b>((int)bI + a - 1, (int)bJ + b - 1)) * Wx[a] * Wy[b];
				 }
			 }
			 bigPic.at<cv::Vec3b>(i, j) = element;
		 }
	 }
	 qDebug() << "extendImage";
	 //namedWindow("BiggerPic", 0);
	 //imshow("BiggerPic", bigPic);
	 // dealPic = bigPic;
	 //waitKey(0);

 };*/
 //ͼƬ��ת

 //ͼ����ת 
 //ǰ��ӳ��
 //x2 = x1cosa + y1sina
 //y2 = -x1sina + y1cosa
 //����ӳ��  x1 = x2cosa - y2sina   y1= x2sina + y2cosa

 Mat getRotatePic(Mat& dealPic, double angle) {
	 int opic1[2] = { -dealPic.rows / 2, dealPic.cols / 2 };  //����ĸ�����  ������ת�󶥵��λ��ȷ��ͼƬ��С//right up
	 int opic2[2] = { -dealPic.rows / 2, -dealPic.cols / 2 };//left up
	 int opic3[2] = { dealPic.rows / 2, dealPic.cols / 2 };//right down
	 int opic4[2] = { dealPic.rows / 2, -dealPic.cols / 2 };//left down

	 angle = angle / 180.0f * PI;
	 double cosa = cos(angle);
	 double sina = sin(angle);

	 int dpic1[2] = { opic1[0] * cosa + opic1[1] * sina , -opic1[0] * sina + opic1[1] * cosa };
	 int dpic2[2] = { opic2[0] * cosa + opic2[1] * sina , -opic2[0] * sina + opic2[1] * cosa };
	 int dpic3[2] = { opic3[0] * cosa + opic3[1] * sina , -opic3[0] * sina + opic3[1] * cosa };
	 int dpic4[2] = { opic4[0] * cosa + opic4[1] * sina , -opic4[0] * sina + opic4[1] * cosa };

	 //���㳤��
	 int width = (int)max(abs(dpic1[0] - dpic4[0]), abs(dpic2[0] - dpic3[0])) + 2; //��ֹԽ��
	 int height = (int)max(abs(dpic1[1] - dpic4[1]), abs(dpic2[1] - dpic3[1])) + 2;

	 Mat rotatePic = Mat::zeros(width, height, dealPic.type());  //������ͼƬ
 
	 return rotatePic;
 }
 //���߳�ģ��
 void  RotatePictureByBic(Mat & dealPic, double angle, int rowsStart, int blockRows, Mat& rotatePic) {


	 //int opic1[2] = { -dealPic.rows / 2, dealPic.cols / 2 };  //����ĸ�����  ������ת�󶥵��λ��ȷ��ͼƬ��С//right up
	 //int opic2[2] = { -dealPic.rows / 2, -dealPic.cols / 2 };//left up
	 //int opic3[2] = { dealPic.rows / 2, dealPic.cols / 2 };//right down
	 //int opic4[2] = { dealPic.rows / 2, -dealPic.cols / 2 };//left down

	 angle = angle / 180.0f * PI;
	 double cosa = cos(angle);
	 double sina = sin(angle);

	 //int dpic1[2] = { opic1[0] * cosa + opic1[1] * sina , -opic1[0] * sina + opic1[1] * cosa };
	 //int dpic2[2] = { opic2[0] * cosa + opic2[1] * sina , -opic2[0] * sina + opic2[1] * cosa };
	 //int dpic3[2] = { opic3[0] * cosa + opic3[1] * sina , -opic3[0] * sina + opic3[1] * cosa };
	 //int dpic4[2] = { opic4[0] * cosa + opic4[1] * sina , -opic4[0] * sina + opic4[1] * cosa };

	 //���㳤��
	 int width = rotatePic.cols;
	 int height = rotatePic.rows;

	 int srows = ((double)rowsStart / dealPic.rows) * height;
	 int sblock = ((double)blockRows / dealPic.rows) * height;
	

	 //Mat rotatePic = Mat::zeros(width, height, dealPic.type());  //������ͼƬ
	 //qDebug() << rotatePic.channels();
	 //��ֵ����
	 for (int i = srows; i < srows + sblock; i++) {  
		 for (int j = 0; j < width; j++) {
			 //����ӳ��Ѱ��ԭͼ�е�λ��
			 int Rx = round(double(i - height / 2.0f)*cosa - double(j - width / 2.0f)*sina + dealPic.rows / 2.0f);
			 int Ry = round(double(i - height / 2.0f)*sina + double(j - width / 2.0f)*cosa + dealPic.cols / 2.0f);


			 Vec3f element = { 0,0,0 }; //����ֵ
			 double Wx[4], Wy[4];  //xy��Ȩ��

			 getW_xy(Wx, Rx);
			 getW_xy(Wy, Ry);
			 //���������صĴ�С

			 for (int a = 0; a < 4; a++) {
				 for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
					 if (Rx + a - 1 < 0 || Rx + a - 1 >= dealPic.rows || Ry + b - 1 < 0 || Ry + b - 1 >= dealPic.cols)    //��ֹԽ�� ����ԭͼ��
						 rotatePic.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
					 else
					 {
						 element += (Vec3f)(dealPic.at<cv::Vec3b>((int)Rx + a - 1, (int)Ry + b - 1)) * Wx[a] * Wy[b];
					 }
				 }
			 }
			 rotatePic.at<cv::Vec3b>(i, j) = (cv::Vec3b)element;
		 }
	 }
	 //dealPic = rotatePic;
	// imshow("output", dealPic);
 }



	 //���߳�ģ��
 void  RotatePictureByBic(Mat & dealPic, double angle) {
	

	 int opic1[2] = { -dealPic.rows / 2, dealPic.cols / 2 };  //����ĸ�����  ������ת�󶥵��λ��ȷ��ͼƬ��С//right up
	 int opic2[2] = { -dealPic.rows / 2, -dealPic.cols / 2 };//left up
	 int opic3[2] = { dealPic.rows / 2, dealPic.cols / 2 };//right down
	 int opic4[2] = { dealPic.rows / 2, -dealPic.cols / 2 };//left down
	
	 angle = angle / 180.0f * PI;  
	 double cosa = cos(angle);
	 double sina = sin(angle);

	 int dpic1[2] = { opic1[0] * cosa + opic1[1] * sina , -opic1[0] * sina + opic1[1] * cosa };
	 int dpic2[2] = { opic2[0] * cosa + opic2[1] * sina , -opic2[0] * sina + opic2[1] * cosa };
	 int dpic3[2] = { opic3[0] * cosa + opic3[1] * sina , -opic3[0] * sina + opic3[1] * cosa };
	 int dpic4[2] = { opic4[0] * cosa + opic4[1] * sina , -opic4[0] * sina + opic4[1] * cosa };

	 //���㳤��
	 int width = (int)max(abs(dpic1[0] - dpic4[0]), abs(dpic2[0] - dpic3[0])) + 2; //��ֹԽ��
	 int height = (int)max(abs(dpic1[1] - dpic4[1]), abs(dpic2[1] - dpic3[1])) + 2;

	 Mat rotatePic = Mat::zeros(width, height, dealPic.type());  //������ͼƬ
	 qDebug() << rotatePic.channels();
	 //��ֵ����
	 for (int i = 2; i < height  - 2; i++) {  //������ȡ4*4����  ��ֹԽ�����Դ�2��ʼ
		 for (int j = 2; j < width  - 2; j++) {
			 //����ӳ��Ѱ��ԭͼ�е�λ��
			 int Rx = round(double(i - height / 2.0f)*cosa - double(j - width / 2.0f)*sina + dealPic.rows / 2.0f);
			 int Ry = round(double(i - height / 2.0f)*sina + double(j - width / 2.0f)*cosa + dealPic.cols / 2.0f);
			

			 Vec3f element = { 0,0,0 }; //����ֵ
			 double Wx[4], Wy[4];  //xy��Ȩ��

			 getW_xy(Wx, Rx);
			 getW_xy(Wy, Ry);
			 //���������صĴ�С
			 		
 				 for (int a = 0; a < 4; a++) {
					 for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
						 if (Rx+a-1 < 0 || Rx+a-1 >= dealPic.rows || Ry+b-1 < 0 || Ry+b-1 >= dealPic.cols)    //��ֹԽ�� ����ԭͼ��
							 rotatePic.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
						 else
						 {
							 element += (Vec3f)(dealPic.at<cv::Vec3b>((int)Rx + a - 1, (int)Ry + b - 1)) * Wx[a] * Wy[b];
						 }						 
					 }
				 }
				 rotatePic.at<cv::Vec3b>(i, j) = (cv::Vec3b)element;
		 }
	 }
	 dealPic = rotatePic;
	 imshow("output", dealPic);
 }

 //�����length����С2���ݴη��Ĵ���
 int extend(int length)
 {
	 int i = 0;
	 int j = 1;
	
	 while (j < length)
	 {
		 j = j * 2;
		 i++;
	 }

	 return i;
 }


 void MYdft(Mat &in, Mat &out, int m, int n) {

	 //��ÿһ�н��и���Ҷ�任
	 MYComplex * test = new MYComplex[in.cols];  //�洢ʵ��������ֵ
	 MYComplex * mytest = new MYComplex[in.cols]; //�洢����Ҷ�任���ֵ
	 for (int i = 0; i < in.rows; i++) {
		 for (int j = 0; j < in.cols; j++) {
			 test[j].real = in.at<Vec2f>(i, j)[0];
			 test[j].imag = in.at<Vec2f>(i, j)[1];
		 }
		 FFT(test, mytest, n);

		 for (int k = 0; k < in.cols; k++) {
			 in.at<Vec2f>(i, k)[0] = mytest[k].real;
			 in.at<Vec2f>(i, k)[1] = mytest[k].imag;
		 }

	 }
	 delete[]test;
	 delete[]mytest;
	 //��ÿһ�н��и���Ҷ�任
	 MYComplex * testh = new MYComplex[in.rows];
	 MYComplex * mytesth = new MYComplex[in.rows];
	 for (int i = 0; i < in.cols; i++) {

		 for (int j = 0; j < in.rows; j++) {
			 testh[j] = in.at<Vec2f>(j, i)[0];
			 testh[j] = in.at<Vec2f>(j, i)[1];
		 }
		 FFT(testh, mytesth, m);

		 for (int k = 0; k < in.rows; k++) {
			 in.at<Vec2f>(k, i)[0] = mytesth[k].real;
			 in.at<Vec2f>(k, i)[1] = mytesth[k].imag;
		 }
	 }
	 delete[]testh;
	 delete[]mytesth;

 }




 void FourierTransform(Mat &dealPic) {
 
	 //Mat dealPic = imread("F:\\test\\q.jpg", IMREAD_GRAYSCALE);      
	// Mat dealPic = imread("F:\\test\\dealPic.jpg");
	 Mat grayPic;
	 cvtColor(dealPic, grayPic, CV_BGR2GRAY);



	 Mat extendPic;                 //�����ͼ�� Ҫ����߳�Ϊ2���ݴη�

	 int M = extend(grayPic.rows);  //����ݴη�
	 int N = extend(grayPic.cols);
	 int m = pow(2, M);//  ��������ı߳�
	 int n = pow(2, N);
	 // cout << m << "ddd" << n;
	 //��ԭͼ�������� �����extendPic�� ��0���
	 copyMakeBorder(grayPic, extendPic, 0, m - grayPic.rows, 0, n - grayPic.cols, BORDER_CONSTANT, Scalar::all(0));


	 Mat complexPic;  //������ں�ͼ��
	 Mat context[] = { Mat_<double>(extendPic), Mat::zeros(extendPic.size(),CV_32F) }; //�洢ͼƬ��ʵ����

	 merge(context, 2, complexPic);     //��context�ںϺϲ���һ��2ͨ������complexPic��0ͨ��Ϊʵ������1Ϊ������
	 //���и���Ҷ�任
	 MYdft(complexPic, complexPic, M, N);

	 split(complexPic, context);  //��complexPic��2ͨ�� ��ʵ�����鲿����

	 magnitude(context[0], context[1], context[0]);     //�����ά������ֵ����ʵ�����鲿�������

	 Mat resultPic = context[0];   //��ֵʵ����

	 resultPic += Scalar::all(1);
	 log(resultPic, resultPic);                //���ж���������

	 //����������л��У����Ƶ�׽��вü�
	 resultPic = resultPic(Rect(0, 0, resultPic.cols&-2, resultPic.rows&-2));

	 //��ԭ������ͼ������
	 int cx = resultPic.cols / 2;
	 int cy = resultPic.rows / 2;

	 Mat q1(resultPic, Rect(0, 0, cx, cy));       //����
	 Mat q2(resultPic, Rect(cx, 0, cx, cy));      //����
	 Mat q3(resultPic, Rect(0, cy, cx, cy));      //����
	 Mat q4(resultPic, Rect(cx, cy, cx, cy));     //����

	 //�任���ϽǺ����½�����
	 Mat swap;    //���ڽ�������ʱ����

	 q1.copyTo(swap);
	 q4.copyTo(q1);
	 swap.copyTo(q4);

	 //�任���ϽǺ����½�����
	 q2.copyTo(swap);
	 q3.copyTo(q2);
	 swap.copyTo(q3);

	 //��һ�������������Ϊ0-1֮��ĸ�����
	 normalize(resultPic, resultPic, 0, 1, CV_MINMAX);

	// imshow("����ͼ��", grayPic);
	 imshow("Ƶ��ͼ", resultPic);
	 waitKey(0);

 }
 //�����ֵ
 int avgValue(int * src, int size) {          
	 int sum = 0;
	 for (int i = 0; i < size; i++)
	 {
		 sum += src[i];
	 }
	 // �����Ԫ��
	 return sum / size;
 }

 //ƽ�������˲�
 void SmoothingFilter(Mat &dealPic,int startRows, int blockRows , int filterSize) {
	 //int filterSize = 3;   //����˴�С
	 int skipSize = filterSize / 2; //�����������
	// qDebug() << "ͨ����" << dealPic.channels() << endl;

	 //������߽��ϵ����ؽ��д����ֹԽ��

		 int i = 0;
		 startRows == 0 ? i = skipSize : i = startRows;
		 int p = 0;
		 (startRows + blockRows) >= dealPic.rows ? (p = dealPic.rows - 1 - filterSize) : (p = startRows + blockRows);
		 for (; i < p; i++) {

			 for (int j = skipSize; j < dealPic.cols - 1 - filterSize; j++) {

				 int c_cout = 0;

				 int skipSize2 = filterSize / 2; //�����������
				 int * filterArray = new int[filterSize * filterSize];  //�����������˴�С������
				 int * filterArray1 = new int[filterSize * filterSize];
				 int * filterArray2 = new int[filterSize * filterSize];

				 for (int k = skipSize2, count_i = 0; count_i < filterSize; count_i++) {
					 // i -1 j -1    i - 0, j - 1 , i +1, j -1 
					 // 0 1 2 3 4 5 p[5]
					 //�����ص��������
					 for (int p = skipSize2, count_j = 0; count_j < filterSize; count_j++) {
						 filterArray[c_cout] = dealPic.at<cv::Vec3b>(i - p, j - k)[0];
						 filterArray1[c_cout] = dealPic.at<cv::Vec3b>(i - p, j - k)[1];
						 filterArray2[c_cout] = dealPic.at<cv::Vec3b>(i - p, j - k)[2];
						 p--;
						 c_cout++;
					 }
					 k--;
				 }
				// int middle_value = filterSize * filterSize / 2;  //��ȡ�������������ֵ
				 int avg = avgValue(filterArray, filterSize * filterSize);  //��ȡ�������ľ�ֵ
				 int avg1 = avgValue(filterArray1, filterSize * filterSize);
				 int avg2 = avgValue(filterArray2, filterSize * filterSize);
				
			
				 dealPic.at<cv::Vec3b>(i, j)[0] = avg;
				 dealPic.at<cv::Vec3b>(i, j)[1] = avg1;
				 dealPic.at<cv::Vec3b>(i, j)[2] = avg2;
					
				     delete[]filterArray;
					 delete[]filterArray1;   //�ͷ��ڴ�
					 delete[]filterArray2;
			 }

		 

	 }
 
 }

 //�������ĸ�˹Ȩ��
double** GayssianWightArray(int size ,double sigma = 1) {
	 
	 double  **array = new double*[size];
	 for (int i = 0; i < size; i++)        //�����洢������
		 array[i] = new double[size];

	 double sum = 0;

	 for (int i = 0; i < size; i++) {
		 for (int j = 0; j < size; j++) {
			 array[i][j] = (1 / (2 * PI * pow(sigma, 2)) * exp(-((pow(i - size, 2) + pow(j - size, 2)) / (2 * pow(sigma, 2)))));//�Ե�һ����Ϊԭ��
			 sum += array[i][j];
		 }
	 }
	 for (int i = 0; i < size; i++) {
		 for (int j = 0; j < size; j++) {
			 array[i][j] /= sum;
		 }
	 }
	 return array;
 }
//�ø�˹Ȩ�ؾ��������ֵ
int  GetGayssianValue(int * src, double ** wight, int size) {
	double value = 0;
	double * arr = new double[size * size];
	for (int i = 0, count = 0; i < size ; i++) {  //����ά�����Ϊһά�ô���
		for (int j = 0; j < size; j++) {
			arr[count] = wight[i][j];
			count++;
		}
	}
	for (int i = 0; i < size* size; i++)
		value += src[i] * arr[i];
	delete[]arr;

	return (int)value;
}

//��˹�˲�
void GayssianFilter(Mat &dealPic, int startRows, int blockRows, int filterSize) {

	int skipSize = filterSize / 2; //�����������
   // qDebug() << "ͨ����" << dealPic.channels() << endl;
	double ** wight = GayssianWightArray(filterSize); //��ȡ�����˹Ȩ�ؾ���
	//������߽��ϵ����ؽ��д����ֹԽ��

	int i = 0;
	startRows == 0 ? i = skipSize : i = startRows;
	int p = 0;
	(startRows + blockRows) >= dealPic.rows ? (p = dealPic.rows - 1  - filterSize) : (p = startRows + blockRows);
	for (; i < p; i++) {

		for (int j = skipSize; j < dealPic.cols - 1 - filterSize; j++) {

			int c_cout = 0;

			int skipSize2 = filterSize / 2; //�����������
			int * filterArray = new int[filterSize * filterSize];  //�����������˴�С������
			int * filterArray1 = new int[filterSize * filterSize];
			int * filterArray2 = new int[filterSize * filterSize];

			for (int k = skipSize2, count_i = 0; count_i < filterSize; count_i++) {
				// i -1 j -1    i - 0, j - 1 , i +1, j -1 
				// 0 1 2 3 4 5 p[5]
				//�����ص��������
				for (int p = skipSize2, count_j = 0; count_j < filterSize; count_j++) {
					filterArray[c_cout] = dealPic.at<cv::Vec3b>(i - p, j - k)[0];
					filterArray1[c_cout] = dealPic.at<cv::Vec3b>(i - p, j - k)[1];
					filterArray2[c_cout] = dealPic.at<cv::Vec3b>(i - p, j - k)[2];
					p--;
					c_cout++;
				}
				k--;
			}
			// int middle_value = filterSize * filterSize / 2;  //��ȡ�������������ֵ


			dealPic.at<cv::Vec3b>(i, j)[0] = GetGayssianValue(filterArray, wight, filterSize); //��ֵ
			dealPic.at<cv::Vec3b>(i, j)[1] = GetGayssianValue(filterArray1, wight, filterSize);
			dealPic.at<cv::Vec3b>(i, j)[2] = GetGayssianValue(filterArray2, wight, filterSize);

			delete[]filterArray;
			delete[]filterArray1;   //�ͷ��ڴ�
			delete[]filterArray2;
		}
	
	}

}

//������С������  ��������ֵ
double cirNoise(Mat & pic, int sizeOfFilter, double **miu, double **sigma, int rowsStart, int  blockRows, int channels = 3) {
	//int height = pic.rows;
	int height = blockRows;
	int width = pic.cols;
	int skipSize = sizeOfFilter / 2; //��Խ��

	double noise = 0;
	//sigma ��niu ����
	//double ** miu = new double*[height * width];
	//for (int i = 0; i < height * width; i++) {
	//	miu[i] = new double[channels];
	//}
	//double ** sigma = new double*[height * width];
	//for (int i = 0; i < height * width; i++) {
	//	sigma[i] = new double[channels];
	//}
	////�����������ʼ��
	//for (int i = 0; i < height * width; i++) {
	//	for (int j = 0; j < channels; j++) {
	//		miu[i][j] = 0;

	//	}
	//}
	//
	//for (int i = 0; i < height * width; i++) {
	//	for (int j = 0; j < channels; j++) {
	//		sigma[i][j] = 0;
	//	}
	//}

	//�������ص�����
	double ** k = new double*[height + 2];
	for (int i = 0; i < height + 2; i++) {
		k[i] = new double[width];
	}
	//��ÿһ��ͨ�����д���
	for (int ch = 0; ch < channels; ch++) {

		int bottom = rowsStart + blockRows;
		int top = rowsStart;
		if (bottom != pic.rows) {            //���Դ�����½�  ����û�е�ԭͼ��ı߽磬������һ��=����߽�
			for (int j = 0; j < width; j++) {
				k[height + 1][j] = (double)pic.at<cv::Vec3b>(height + rowsStart, j)[ch];
			}
		}
		if (top != 0) {
			for (int j = 0; j < width; j++) { //���Դ�����Ͻ죬����ͬ��
				k[0][j] = (double)pic.at<cv::Vec3b>(rowsStart - 1, j)[ch];
			}
		}
		for (int i = 1; i < height + 1; i++) {
			for (int j = 0; j < width; j++) {
				k[i][j] = (double)pic.at<cv::Vec3b>(i - 1 + rowsStart, j)[ch];//ǰ��I����Ҫ������
			}
		}//���������ϴ�����̺߳�����һ�������ŵĺ���
		
		//qDebug() << "rowsStart:  " << rowsStart;
		//��ÿһ�����ؽ��о������ƽ������
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				//cout << k[i][j] << "d" << endl;
				for (int m = 0; m < sizeOfFilter; m++) {
					for (int n = 0; n < sizeOfFilter; n++) {
						//int bottom = rowsStart + blockRows;
						if (bottom != pic.rows) bottom += 1;  //���û��Խԭͼ�� �Ϳ��Դ���߽�

						//int top = rowsStart;
						if (top != 0) top = top - 1;  //���û��Խԭͼ�� �Ϳ��Դ���߽�

						if ((i - skipSize + m + rowsStart ) >= top  && (i - skipSize + m + rowsStart ) < bottom  && (j - skipSize + n) < pic.cols && (j - skipSize + n) >= 0) { //��Խ��
						//	try {
								miu[i* width + j][ch] += k[i - skipSize + m + 1][j - skipSize + n];
								
								// / pow(sizeOfFilter, 2);
							//}
							//catch (Exception e) {
							//	qDebug() << "i :  " << i - skipSize + m << "  j:  " << j - skipSize + n;
							//}
							sigma[i* width + j][ch] += k[i - skipSize + m + 1][j - skipSize + n] * k[i - skipSize + m + 1][j - skipSize + n];// / pow(sizeOfFilter, 2);
						}
					}
				}
				miu[i* width + j][ch] /= pow(sizeOfFilter, 2);          //�� = 1 / MN * sum(a(i,j))
				sigma[i* width + j][ch] /= pow(sizeOfFilter, 2);        // ��^2 = 1 / a(i , j)^2 - ��
				sigma[i * width + j][ch] -= pow(miu[i* width + j][ch], 2);
				noise += sigma[i * width + j][ch];
			}
		}
	}

	delete[]k;
	noise = noise / (width * height * channels);
	//cout << "my: " << noise;
	return noise;
}

void wienerFilter(Mat &dealPic, int rowsStart,int  blockRows, int filterSize) {

	int width = dealPic.cols;
	//int width = blockRows;
	//int height = dealPic.rows;
	int height = blockRows;//����ͼƬ�Ĵ�С
	int channel = dealPic.channels();
	double ** sigma;
	double ** miu;
	double * deta;

	double ** pic = new double*[height * width];

	//sigma ��niu ����
	miu = new double*[height * width];
	for (int i = 0; i < height * width; i++) {
		miu[i] = new double[channel];
	}
	sigma = new double*[height * width];
	for (int i = 0; i < height * width; i++) {
		sigma[i] = new double[channel];
	}
	//�����������ʼ��
	for (int i = 0; i < height * width; i++) {
		for (int j = 0; j < channel; j++) {
			miu[i][j] = 0;
		}
	}

	for (int i = 0; i < height * width; i++) {
		for (int j = 0; j < channel; j++) {
			sigma[i][j] = 0;
		}
	}


	deta = new double[height * width];
	for (int i = 0; i < height * width; i++) {
		deta[i] = 0;
	}
	/*for (int i = 0; i < height * width; i++) {
		deta[i] = new double[channel];
	}*/


	for (int i = 0; i < height * width; i++) {
		for (int j = 0; j < channel; j++)
			pic[i] = new double[channel];
	}
	for (int i = 0; i < height; i++) {    //��ֵ
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < channel; k++)
				pic[i * width + j][k] = dealPic.at<cv::Vec3b>(i + rowsStart, j)[k]; //��ʼ����+i
		}
	}

	//double noise = calculateNoiseVariance(dealPic, filterSize, miu, sigma);
	double noise = cirNoise(dealPic, filterSize, miu, sigma, rowsStart, blockRows);
	qDebug() << "noise:  " <<noise;
	/*for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << miu[i][j] << "aaaaaaaa" << endl;
			cout << sigma[i][j] << endl;
		}
	}*/

	for (int k = 0; k < channel; k++) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				deta[i * width + j] = pic[i * width + j][k] - miu[i * width + j][k];//(a(i,j) - u)
				pic[i * width + j][k] = sigma[i * width + j][k] - noise;
				if (pic[i * width + j][k] < 0);
					pic[i * width + j][k] = 0;
				sigma[i * width + j][k] = max(sigma[i * width + j][k], noise);  //b(i ,j) = ��+ (��^2 - v^2) /��^2 * (a(i,j) - u)
				deta[i * width + j] = deta[i * width + j] * pic[i * width + j][k] / sigma[i * width + j][k] + miu[i * width + j][k];
				dealPic.at<Vec3b>(i + rowsStart, j)[k] = deta[i * width + j];
			}
		}

	}
	
	//qDebug() << "rowsStart:  " << rowsStart;
	//for (int i = 0; i < height; i++) {
	//	for (int j = 0; j < width; j++) {
	//		//out.ptr<uchar>(i)[j] =dstbuffer[i * width + j];
	//		for(int k = 0; k < channel; k++)
	//		dealPic.at<Vec3b>(i, j)[k] = deta[i * width + j];
	//	}
	//}
	//cout << "end" << endl;
	delete[] deta;
	delete[] sigma;
	delete[] miu;
	delete[] pic;

}




void  BicubicInterpolation_GPU(Mat &dealPic, double scaleFactor) {
	int width = dealPic.cols;
	int height = dealPic.rows;

	float BRows = dealPic.rows * scaleFactor;  //������ͼƬ�ĳ���
	float BCols = dealPic.cols * scaleFactor;

	Mat BigPic = Mat((int)BRows + 1, (int)BCols + 1, CV_8UC3); //���ɷŴ��ͼƬ +1 ��ֹԽ��

	float *bigPicR = (float*)malloc(sizeof(float)* BRows * BCols);
	float *bigPicG = (float*)malloc(sizeof(float)* BRows * BCols);
	float *bigPicB = (float*)malloc(sizeof(float)* BRows * BCols);

	float *picR = (float*)malloc(sizeof(float)*height * width);
	float *picG = (float*)malloc(sizeof(float)*height * width);
	float *picB = (float*)malloc(sizeof(float)*height * width);
	//��ͼƬ��Ϊһά����
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			picR[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[0];
			picG[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[1];
			picB[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[2];
		}
	}
	/*for (int i = 0; i < BRows; i++) {
		for (int j = 0; j < BCols * 3; j++) {
			std::cout << pic[i * width + j] << std::endl;
		}
	}*/

	//����GPU����
	BicubicInterpolation_host(picR, bigPicR, width, height, scaleFactor);
	BicubicInterpolation_host(picG, bigPicG, width, height, scaleFactor);
	BicubicInterpolation_host(picB, bigPicB, width, height, scaleFactor);
	for (int i = 0; i < BRows; i++) {
		for (int j = 0; j < BCols; j++) {
			BigPic.at<cv::Vec3b>(i, j)[0] = bigPicR[int(i * BCols + j)];
			BigPic.at<cv::Vec3b>(i, j)[1] = bigPicG[int(i * BCols + j)];
			BigPic.at<cv::Vec3b>(i, j)[2] = bigPicB[int(i * BCols + j)];
		}
	}
	/*
	for (int i = 0; i < BRows; i++) {
		for (int j = 0; j < BCols * 3; j++) {
			std::cout << bigPic[i * (int)BCols * 3 + j] << std::endl;
		}
	}
*/
	free(bigPicR);
	free(bigPicG);
	free(bigPicB);
	free(picR);
	free(picG);
	free(picB);
	dealPic = BigPic;
};



void   RotatePictureByBic_GPU(Mat & dealPic, double angle, Mat& rotatePic) {


	int width = dealPic.cols;
	int height = dealPic.rows;

	int  rRows = rotatePic.rows; //������ͼƬ�ĳ���
	int  rCols = rotatePic.cols;



	float *rPicR = (float*)malloc(sizeof(float)* rRows * rCols);
	float *rPicG = (float*)malloc(sizeof(float)* rRows * rCols);
	float *rPicB = (float*)malloc(sizeof(float)* rRows * rCols);

	float *picR = (float*)malloc(sizeof(float)*height * width);
	float *picG = (float*)malloc(sizeof(float)*height * width);
	float *picB = (float*)malloc(sizeof(float)*height * width);
	//��ͼƬ��Ϊһά����
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			picR[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[0];
			picG[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[1];
			picB[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[2];
		}
	}

	//
	RotatePictureByBic_host(picR, rPicR, width, height, rCols, rRows, angle);
	RotatePictureByBic_host(picG, rPicG, width, height, rCols, rRows, angle);
	RotatePictureByBic_host(picB, rPicB, width, height, rCols, rRows, angle);
	for (int i = 0; i < rRows; i++) {
		for (int j = 0; j < rCols; j++) {
			rotatePic.at<cv::Vec3b>(i, j)[0] = rPicR[i * rCols + j];
			rotatePic.at<cv::Vec3b>(i, j)[1] = rPicG[i * rCols + j];
			rotatePic.at<cv::Vec3b>(i, j)[2] = rPicB[i * rCols + j];
		}
	}


	free(rPicR);
	free(rPicG);
	free(rPicB);
	free(picR);
	free(picG);
	free(picB);
	dealPic = rotatePic;


}

void  GayssianWightArray(float *array, int size, float sigma = 1) {

	//float  **array = new float*[size];
	//for (int i = 0; i < size; i++)        //�����洢������
	//	array[i] = new float[size];
/*
	float *array = new float[size * size];
*/

	float sum = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			array[i * size + j] = (1 / (2 * PI * pow(sigma, 2)) * exp(-((pow(i - size, 2) + pow(j - size, 2)) / (2 * pow(sigma, 2)))));//�Ե�һ����Ϊԭ��
			sum += array[i * size + j];
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			array[i * size + j] /= sum;
		}
	}
}

void  GayssianWightArray(double *array, int size, float sigma = 1) {

	//float  **array = new float*[size];
	//for (int i = 0; i < size; i++)        //�����洢������
	//	array[i] = new float[size];
/*
	float *array = new float[size * size];
*/

	double sum = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			array[i * size + j] = (1 / (2 * PI * pow(sigma, 2)) * exp(-((pow(i - size, 2) + pow(j - size, 2)) / (2 * pow(sigma, 2)))));//�Ե�һ����Ϊԭ��
			sum += array[i * size + j];
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			array[i * size + j] /= sum;
		}
	}
}

void GayssianFilter_GPU(Mat &dealPic, int filterSize) {

	int width = dealPic.cols;
	int height = dealPic.rows;


	float *picR = (float*)malloc(sizeof(float)*height * width);
	float *picG = (float*)malloc(sizeof(float)*height * width);
	float *picB = (float*)malloc(sizeof(float)*height * width);



	//��ͼƬ��Ϊһά����
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			picR[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[0];
			picG[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[1];
			picB[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[2];
		}
	}


	float * wight = new float[filterSize * filterSize];
	GayssianWightArray(wight, filterSize); //��ȡ�����˹Ȩ�ؾ��� һά

	//for (int i = 0; i < filterSize * filterSize; i++)
	//	std::cout << wight[i] << std::endl;

	//GayssianFilter_host
	GayssianFilter_host(picR, wight, width, height, filterSize);
	GayssianFilter_host(picG, wight, width, height, filterSize);
	GayssianFilter_host(picB, wight, width, height, filterSize);
	//�������ΪͼƬ



	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dealPic.at<cv::Vec3b>(i, j)[0] = picR[i * width + j];
			dealPic.at<cv::Vec3b>(i, j)[1] = picG[i * width + j];
			dealPic.at<cv::Vec3b>(i, j)[2] = picB[i * width + j];
		}
	}


	free(picR);
	free(picG);
	free(picB);
}

char * LoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength)
{
	FILE* pFileStream = NULL;
	size_t szSourceLength;

	// open the OpenCL source code file  
	pFileStream = fopen(cFilename, "rb");
	if (pFileStream == NULL)
	{
		return NULL;
	}

	size_t szPreambleLength = strlen(cPreamble);

	// get the length of the source code  
	fseek(pFileStream, 0, SEEK_END);
	szSourceLength = ftell(pFileStream);
	fseek(pFileStream, 0, SEEK_SET);

	// allocate a buffer for the source code string and read it in  
	char* cSourceString = (char *)malloc(szSourceLength + szPreambleLength + 1);
	memcpy(cSourceString, cPreamble, szPreambleLength);
	if (fread((cSourceString)+szPreambleLength, szSourceLength, 1, pFileStream) != 1)
	{
		fclose(pFileStream);
		free(cSourceString);
		return 0;
	}

	// close the file and return the total length of the combined (preamble + source) string  
	fclose(pFileStream);
	if (szFinalLength != 0)
	{
		*szFinalLength = szSourceLength + szPreambleLength;
	}
	cSourceString[szSourceLength + szPreambleLength] = '\0';

	return cSourceString;
}

size_t RoundUp(int groupSize, int globalSize)
{
	int r = globalSize % groupSize;
	if (r == 0)
	{
		return globalSize;
	}
	else
	{
		return globalSize + groupSize - r;
	}
}

void GayssianFilterCL(double *pic, double * weight, double width, double height, int filterSize) {


	//step 1:get platform;
	cl_int ret;														//errcode;
	cl_uint num_platforms;											//���ڱ����ȡ����platforms����;
	ret = clGetPlatformIDs(0, NULL, &num_platforms);
	if ((CL_SUCCESS != ret) || (num_platforms < 1))
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting platform number : %d"), ret);
		AfxMessageBox(errMsg);*/
		qDebug() << "Error getting platform number " ;
		return;
	}
	cl_platform_id platform_id = NULL;
	ret = clGetPlatformIDs(1, &platform_id, NULL);					//��ȡ��һ��platform��id;
	if (CL_SUCCESS != ret)
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting platform id:%d "), ret);
		AfxMessageBox(errMsg);*/
		qDebug() << "Error getting platform id " ;
		return;
	}

	//step 2:get device ;
	cl_uint num_devices;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
	if ((CL_SUCCESS != ret) || (num_devices < 1))
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error getting GPU device number:%d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error getting GPU device number " ;
		return;
	}
	cl_device_id device_id = NULL;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (CL_SUCCESS != ret)
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting GPU device id: %d "), ret);
		AfxMessageBox(errMsg);*/
		qDebug() << "Error getting GPU device id " ;
		return;
	}

	//step 3:create context;
	cl_context_properties props[] =
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0
	};
	cl_context context = NULL;
	context = clCreateContext(props, 1, &device_id, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == context))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error createing context: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error createing context " ;
		return;
	}

	//step 4:create command queue;						//һ��device�ж��queue��queue֮�䲢��ִ��
	cl_command_queue command_queue = NULL;
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if ((CL_SUCCESS != ret) || (NULL == command_queue))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error createing command queue: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error createing command queue " ;
		return;
	}

	//step 5:create memory object;						//�������ͣ�buffer����ͼ�����ͣ�iamge��

	cl_mem mem_obj = NULL;
	cl_mem mem_weight = NULL;


	//create opencl memory object using host ptr
//	mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, BUF_SIZE, host_buffer, &ret);
	mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, width * height * sizeof(double), pic, &ret);
	mem_weight = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, filterSize * filterSize * sizeof(double), weight, &ret);
	if ((CL_SUCCESS != ret) || (NULL == mem_obj))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating memory object: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error creating memory object " ;
		return;
	}

	//step 6:create program;
	size_t szKernelLength = 0;
	//	const char* oclSourceFile = "add_vector.cl";
	const char* oclSourceFile = "GayssianFilter.cl";
	const char* kernelSource = LoadProgSource(oclSourceFile, "", &szKernelLength);
	if (kernelSource == NULL)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error loading source file: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error loading source file " ;
		return;
	}

	//create program
	cl_program program = NULL;
	program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == program))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating program: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error creating program  " << ret ;
		return;
	}

	//build program 
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		size_t len;
		char buffer[8 * 1024];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		qDebug() << "Error build program " << ret << "  " << buffer ;
		/*CString errMsg = (CString)buffer;
		AfxMessageBox(errMsg);
		return;*/
	}

	//step 7:create kernel;
	cl_kernel kernel = NULL;
	//	kernel = clCreateKernel(program, "test", &ret);
	kernel = clCreateKernel(program, "GayssianFilterCL", &ret);

	if ((CL_SUCCESS != ret) || (NULL == kernel))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating kernel: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error creating kernel " ;
		return;
	}

	//step 8:set kernel arguement;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)& mem_obj);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error setting kernel arguement 0: %d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error setting kernel arguement 0 " ;
		return;
	}
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)& mem_weight);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 1: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error setting kernel arguement 1 " ;
		return;
	}
	ret = clSetKernelArg(kernel, 2, sizeof(double), (void*)& width);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error setting kernel arguement 2: %d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error setting kernel arguement 2 " ;
		return;
	}
	ret = clSetKernelArg(kernel, 3, sizeof(double), (void*)& height);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 3: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error setting kernel arguement 3 " ;
		return;
	}
	ret = clSetKernelArg(kernel, 4, sizeof(int), (void*)& filterSize);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 3: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error setting kernel arguement 4 " ;
		return;
	}
	//step 9:set work group size;  							//<---->dimBlock\dimGrid
	cl_uint work_dim = 2;
	size_t local_work_size[2] = { 32, 32 };
	size_t global_work_size[2] = { RoundUp(local_work_size[0], width),
		RoundUp(local_work_size[1], height) };		//let opencl device determine how to break work items into work groups;

	//step 10:run kernel;				//put kernel and work-item arugement into queue and excute;
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error enqueue NDRange: %d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error enqueue NDRange " ;
		return;
	}

	//step 11:get result;
	int *device_buffer = (int *)clEnqueueMapBuffer(command_queue, mem_obj, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, width * height * sizeof(double), 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == device_buffer))
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error map buffer: %d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error map buffer: " ;
		return;
	}

	memcpy(pic, device_buffer, width * height * sizeof(double));

	//step 12:release all resource;
	if (NULL != kernel)
		clReleaseKernel(kernel);
	if (NULL != program)
		clReleaseProgram(program);
	if (NULL != mem_obj)
		clReleaseMemObject(mem_obj);
	if (NULL != command_queue)
		clReleaseCommandQueue(command_queue);
	if (NULL != context)
		clReleaseContext(context);
	if (NULL != mem_weight)
		clReleaseMemObject(mem_weight);
	//	if (NULL != host_buffer)
	//		free(host_buffer);

}


void GayssianFilter_CL(Mat &dealPic, int filterSize) {
	filterSize = 3;

	int width = dealPic.cols;
	int height = dealPic.rows;


	double *picR = (double*)malloc(sizeof(double)*height * width);
	double *picG = (double*)malloc(sizeof(double)*height * width);
	double *picB = (double*)malloc(sizeof(double)*height * width);



	//��ͼƬ��Ϊһά����
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			picR[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[0];
			picG[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[1];
			picB[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[2];
		}
	}


	double * wight = new double[filterSize * filterSize];
	GayssianWightArray(wight, filterSize); //��ȡ�����˹Ȩ�ؾ��� һά

	//for (int i = 0; i < filterSize * filterSize; i++)
	//	qDebug() << wight[i] ;

	//GayssianFilter_host
	GayssianFilterCL(picR, wight, width, height, filterSize);
	GayssianFilterCL(picG, wight, width, height, filterSize);
	GayssianFilterCL(picB, wight, width, height, filterSize);
	//�������ΪͼƬ



	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dealPic.at<cv::Vec3b>(i, j)[0] = picR[i * width + j];
			dealPic.at<cv::Vec3b>(i, j)[1] = picG[i * width + j];
			dealPic.at<cv::Vec3b>(i, j)[2] = picB[i * width + j];
		}
	}

	free(picR);
	free(picG);
	free(picB);

}


void BicubicInterpolationCL(double *pic, double *bigPic, double width, double height, double factor) {

	//step 1:get platform;
	cl_int ret;														//errcode;
	cl_uint num_platforms;											//���ڱ����ȡ����platforms����;
	ret = clGetPlatformIDs(0, NULL, &num_platforms);
	if ((CL_SUCCESS != ret) || (num_platforms < 1))
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting platform number : %d"), ret);
		AfxMessageBox(errMsg);*/
		qDebug() << "Error getting platform number " ;
		return;
	}
	cl_platform_id platform_id = NULL;
	ret = clGetPlatformIDs(1, &platform_id, NULL);					//��ȡ��һ��platform��id;
	if (CL_SUCCESS != ret)
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting platform id:%d "), ret);
		AfxMessageBox(errMsg);*/
		qDebug() << "Error getting platform id " ;
		return;
	}

	//step 2:get device ;
	cl_uint num_devices;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
	if ((CL_SUCCESS != ret) || (num_devices < 1))
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error getting GPU device number:%d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error getting GPU device number " ;
		return;
	}
	cl_device_id device_id = NULL;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (CL_SUCCESS != ret)
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting GPU device id: %d "), ret);
		AfxMessageBox(errMsg);*/
		qDebug() << "Error getting GPU device id " ;
		return;
	}

	//step 3:create context;
	cl_context_properties props[] =
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0
	};
	cl_context context = NULL;
	context = clCreateContext(props, 1, &device_id, NULL, NULL, &ret);
	
	if ((CL_SUCCESS != ret) || (NULL == context))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error createing context: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error createing context " ;
		return;
	}

	//step 4:create command queue;						//һ��device�ж��queue��queue֮�䲢��ִ��
	cl_command_queue command_queue = NULL;
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if ((CL_SUCCESS != ret) || (NULL == command_queue))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error createing command queue: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error createing command queue " ;
		return;
	}

	//step 5:create memory object;						//�������ͣ�buffer����ͼ�����ͣ�iamge��

	cl_mem mem_obj = NULL;

	cl_mem mem_objout = NULL;

	//create opencl memory object using host ptr
//	mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, BUF_SIZE, host_buffer, &ret);
	mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, width * height * sizeof(double), pic, &ret);
	mem_objout = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, factor * factor * width * height * sizeof(double), bigPic, &ret);
	if ((CL_SUCCESS != ret) || (NULL == mem_obj))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating memory object: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error creating memory object " ;
		return;
	}

	//step 6:create program;
	size_t szKernelLength = 0;
	//	const char* oclSourceFile = "add_vector.cl";
	const char* oclSourceFile = "BicubicInterpolation.cl";
	const char* kernelSource = LoadProgSource(oclSourceFile, "", &szKernelLength);
	if (kernelSource == NULL)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error loading source file: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error loading source file " ;
		return;
	}

	//create program
	cl_program program = NULL;
	program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == program))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating program: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error creating program  " << ret ;
		return;
	}

	//build program 
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		size_t len;
		char buffer[8 * 1024];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		qDebug() << "Error build program " << ret << "  " << buffer ;
		/*CString errMsg = (CString)buffer;
		AfxMessageBox(errMsg);
		return;*/
	}

	//step 7:create kernel;
	cl_kernel kernel = NULL;
	//	kernel = clCreateKernel(program, "test", &ret);
	kernel = clCreateKernel(program, "extendPic", &ret);

	if ((CL_SUCCESS != ret) || (NULL == kernel))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating kernel: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error creating kernel " ;
		return;
	}

	//step 8:set kernel arguement;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)& mem_obj);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error setting kernel arguement 0: %d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error setting kernel arguement 0 " ;
		return;
	}
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)& mem_objout);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 1: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error setting kernel arguement 1 " ;
		return;
	}
	ret = clSetKernelArg(kernel, 2, sizeof(double), (void*)& width);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error setting kernel arguement 2: %d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error setting kernel arguement 2 " ;
		return;
	}
	ret = clSetKernelArg(kernel, 3, sizeof(double), (void*)& height);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 3: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error setting kernel arguement 3 " ;
		return;
	}
	ret = clSetKernelArg(kernel, 4, sizeof(double), (void*)& factor);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 3: %d "), ret);
		//AfxMessageBox(errMsg);
		qDebug() << "Error setting kernel arguement 4 " ;
		return;
	}
	//step 9:set work group size;  							//<---->dimBlock\dimGrid
	cl_uint work_dim = 2;
	size_t local_work_size[2] = { 32, 32 };
	size_t global_work_size[2] = { RoundUp(local_work_size[0], width * factor),
		RoundUp(local_work_size[1], height * factor) };		//let opencl device determine how to break work items into work groups;

	//step 10:run kernel;				//put kernel and work-item arugement into queue and excute;
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error enqueue NDRange: %d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error enqueue NDRange " ;
		return;
	}

	//step 11:get result;
	int *device_buffer = (int *)clEnqueueMapBuffer(command_queue, mem_objout, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, factor * factor * width * height * sizeof(double), 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == device_buffer))
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error map buffer: %d "), ret);
			AfxMessageBox(errMsg);*/
		qDebug() << "Error map buffer: " ;
		return;
	}

	memcpy(bigPic, device_buffer, factor * factor * width * height * sizeof(double));

	//step 12:release all resource;
	if (NULL != kernel)
		clReleaseKernel(kernel);
	if (NULL != program)
		clReleaseProgram(program);
	if (NULL != mem_obj)
		clReleaseMemObject(mem_obj);
	if (NULL != mem_objout)
		clReleaseMemObject(mem_objout);
	if (NULL != command_queue)
		clReleaseCommandQueue(command_queue);
	if (NULL != context)
		clReleaseContext(context);
	//	if (NULL != host_buffer)
	//		free(host_buffer);



}

void BicubicInterpolation_CL(Mat &dealPic, double  scaleFactor) {

	int width = dealPic.cols;
	int height = dealPic.rows;

	double BRows = dealPic.rows * scaleFactor;  //������ͼƬ�ĳ���
	double BCols = dealPic.cols * scaleFactor;

	Mat BigPic = Mat((int)BRows + 1, (int)BCols + 1, CV_8UC3); //���ɷŴ��ͼƬ +1 ��ֹԽ��

	double *bigPicR = (double*)malloc(sizeof(double)* BRows * BCols);
	double *bigPicG = (double*)malloc(sizeof(double)* BRows * BCols);
	double *bigPicB = (double*)malloc(sizeof(double)* BRows * BCols);

	double *picR = (double*)malloc(sizeof(double)*height * width);
	double *picG = (double*)malloc(sizeof(double)*height * width);
	double *picB = (double*)malloc(sizeof(double)*height * width);
	//��ͼƬ��Ϊһά����
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			picR[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[0];
			picG[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[1];
			picB[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[2];
		}
	}
	/*for (int i = 0; i < BRows; i++) {
		for (int j = 0; j < BCols * 3; j++) {
			qDebug() << pic[i * width + j] ;
		}
	}*/

	//����GPU����
	BicubicInterpolationCL(picR, bigPicR, width, height, scaleFactor);
	BicubicInterpolationCL(picG, bigPicG, width, height, scaleFactor);
	BicubicInterpolationCL(picB, bigPicB, width, height, scaleFactor);
	for (int i = 0; i < BRows; i++) {
		for (int j = 0; j < BCols; j++) {
			BigPic.at<cv::Vec3b>(i, j)[0] = bigPicR[int(i * BCols + j)];
			BigPic.at<cv::Vec3b>(i, j)[1] = bigPicG[int(i * BCols + j)];
			BigPic.at<cv::Vec3b>(i, j)[2] = bigPicB[int(i * BCols + j)];
		}
	}
	/*
	for (int i = 0; i < BRows; i++) {
		for (int j = 0; j < BCols * 3; j++) {
			qDebug() << bigPic[i * (int)BCols * 3 + j] ;
		}
	}
*/
	free(bigPicR);
	free(bigPicG);
	free(bigPicB);
	free(picR);
	free(picG);
	free(picB);
	dealPic = BigPic;

}
void   RotatePictureByBicCL(double *pic, double *rPic, double width,
	double height, double rWidth, double rHeight, double angle) {

	//step 1:get platform;
	cl_int ret;														//errcode;
	cl_uint num_platforms;											//���ڱ����ȡ����platforms����;
	ret = clGetPlatformIDs(0, NULL, &num_platforms);
	if ((CL_SUCCESS != ret) || (num_platforms < 1))
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting platform number : %d"), ret);
		AfxMessageBox(errMsg);*/
		std::cout << "Error getting platform number " << std::endl;
		return;
	}
	cl_platform_id platform_id = NULL;
	ret = clGetPlatformIDs(1, &platform_id, NULL);					//��ȡ��һ��platform��id;
	if (CL_SUCCESS != ret)
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting platform id:%d "), ret);
		AfxMessageBox(errMsg);*/
		std::cout << "Error getting platform id " << std::endl;
		return;
	}

	//step 2:get device ;
	cl_uint num_devices;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
	if ((CL_SUCCESS != ret) || (num_devices < 1))
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error getting GPU device number:%d "), ret);
			AfxMessageBox(errMsg);*/
		std::cout << "Error getting GPU device number " << std::endl;
		return;
	}
	cl_device_id device_id = NULL;
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (CL_SUCCESS != ret)
	{
		/*CString errMsg;
		errMsg.Format(_T("Error getting GPU device id: %d "), ret);
		AfxMessageBox(errMsg);*/
		std::cout << "Error getting GPU device id " << std::endl;
		return;
	}

	//step 3:create context;
	cl_context_properties props[] =
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0
	};
	cl_context context = NULL;
	context = clCreateContext(props, 1, &device_id, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == context))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error createing context: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error createing context " << std::endl;
		return;
	}

	//step 4:create command queue;						//һ��device�ж��queue��queue֮�䲢��ִ��
	cl_command_queue command_queue = NULL;
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if ((CL_SUCCESS != ret) || (NULL == command_queue))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error createing command queue: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error createing command queue " << std::endl;
		return;
	}

	//step 5:create memory object;						//�������ͣ�buffer����ͼ�����ͣ�iamge��

	cl_mem mem_obj = NULL;

	cl_mem mem_objout = NULL;

	//create opencl memory object using host ptr
//	mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, BUF_SIZE, host_buffer, &ret);
	mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, width * height * sizeof(double), pic, &ret);
	mem_objout = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, rWidth * rHeight * sizeof(double), rPic, &ret);
	if ((CL_SUCCESS != ret) || (NULL == mem_obj))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating memory object: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error creating memory object " << std::endl;
		return;
	}

	//step 6:create program;
	size_t szKernelLength = 0;
	//	const char* oclSourceFile = "add_vector.cl";
	const char* oclSourceFile = "RotatePictureByBic.cl";
	const char* kernelSource = LoadProgSource(oclSourceFile, "", &szKernelLength);
	if (kernelSource == NULL)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error loading source file: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error loading source file " << std::endl;
		return;
	}

	//create program
	cl_program program = NULL;
	program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == program))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating program: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error creating program  " << ret << std::endl;
		return;
	}

	//build program 
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		size_t len;
		char buffer[8 * 1024];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		std::cout << "Error build program " << ret << "  " << buffer << std::endl;
		/*CString errMsg = (CString)buffer;
		AfxMessageBox(errMsg);
		return;*/
	}

	//step 7:create kernel;
	cl_kernel kernel = NULL;
	//	kernel = clCreateKernel(program, "test", &ret);
	kernel = clCreateKernel(program, "rotatePic", &ret);

	if ((CL_SUCCESS != ret) || (NULL == kernel))
	{
		//CString errMsg;
		//errMsg.Format(_T("Error creating kernel: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error creating kernel " << std::endl;
		return;
	}

	//step 8:set kernel arguement;
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)& mem_obj);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error setting kernel arguement 0: %d "), ret);
			AfxMessageBox(errMsg);*/
		std::cout << "Error setting kernel arguement 0 " << std::endl;
		return;
	}
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)& mem_objout);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 1: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error setting kernel arguement 1 " << std::endl;
		return;
	}
	ret = clSetKernelArg(kernel, 2, sizeof(double), (void*)& width);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error setting kernel arguement 2: %d "), ret);
			AfxMessageBox(errMsg);*/
		std::cout << "Error setting kernel arguement 2 " << std::endl;
		return;
	}
	ret = clSetKernelArg(kernel, 3, sizeof(double), (void*)& height);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 3: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error setting kernel arguement 3 " << std::endl;
		return;
	}
	ret = clSetKernelArg(kernel, 4, sizeof(double), (void*)& rWidth);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 3: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error setting kernel arguement 4 " << std::endl;
		return;
	}
	ret = clSetKernelArg(kernel, 5, sizeof(double), (void*)& rHeight);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 3: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error setting kernel arguement 5 " << std::endl;
		return;
	}
	ret = clSetKernelArg(kernel, 6, sizeof(double), (void*)& angle);
	if (CL_SUCCESS != ret)
	{
		//CString errMsg;
		//errMsg.Format(_T("Error setting kernel arguement 3: %d "), ret);
		//AfxMessageBox(errMsg);
		std::cout << "Error setting kernel arguement 6 " << std::endl;
		return;
	}
	//step 9:set work group size;  							//<---->dimBlock\dimGrid
	cl_uint work_dim = 2;
	size_t local_work_size[2] = { 32, 32 };
	size_t global_work_size[2] = { RoundUp(local_work_size[0], rWidth),
		RoundUp(local_work_size[1], rHeight) };		//let opencl device determine how to break work items into work groups;

	//step 10:run kernel;				//put kernel and work-item arugement into queue and excute;
	ret = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (CL_SUCCESS != ret)
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error enqueue NDRange: %d "), ret);
			AfxMessageBox(errMsg);*/
		std::cout << "Error enqueue NDRange " << std::endl;
		return;
	}

	//step 11:get result;
	int *device_buffer = (int *)clEnqueueMapBuffer(command_queue, mem_objout, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, rWidth * rHeight * sizeof(double), 0, NULL, NULL, &ret);
	if ((CL_SUCCESS != ret) || (NULL == device_buffer))
	{
		/*	CString errMsg;
			errMsg.Format(_T("Error map buffer: %d "), ret);
			AfxMessageBox(errMsg);*/
		std::cout << "Error map buffer: " << std::endl;
		return;
	}

	memcpy(rPic, device_buffer, rWidth * rHeight * sizeof(double));

	//step 12:release all resource;
	if (NULL != kernel)
		clReleaseKernel(kernel);
	if (NULL != program)
		clReleaseProgram(program);
	if (NULL != mem_obj)
		clReleaseMemObject(mem_obj);
	if (NULL != command_queue)
		clReleaseCommandQueue(command_queue);
	if (NULL != context)
		clReleaseContext(context);
	//	if (NULL != host_buffer)
	//		free(host_buffer);
	if (NULL != mem_objout)
		clReleaseMemObject(mem_objout);




}
void  RotatePictureByBic_CL(Mat & dealPic, double angle, Mat& rotatePic) {

	int width = dealPic.cols;
	int height = dealPic.rows;

	int  rRows = rotatePic.rows; //������ͼƬ�ĳ���
	int  rCols = rotatePic.cols;



	double *rPicR = (double*)malloc(sizeof(double)* rRows * rCols);
	double *rPicG = (double*)malloc(sizeof(double)* rRows * rCols);
	double *rPicB = (double*)malloc(sizeof(double)* rRows * rCols);

	double *picR = (double*)malloc(sizeof(double)*height * width);
	double *picG = (double*)malloc(sizeof(double)*height * width);
	double *picB = (double*)malloc(sizeof(double)*height * width);
	//��ͼƬ��Ϊһά����
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			picR[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[0];
			picG[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[1];
			picB[i * width + j] = dealPic.at<cv::Vec3b>(i, j)[2];
		}
	}

	//
	RotatePictureByBicCL(picR, rPicR, width, height, rCols, rRows, angle);
	RotatePictureByBicCL(picG, rPicG, width, height, rCols, rRows, angle);
	RotatePictureByBicCL(picB, rPicB, width, height, rCols, rRows, angle);
	for (int i = 0; i < rRows; i++) {
		for (int j = 0; j < rCols; j++) {
			rotatePic.at<cv::Vec3b>(i, j)[0] = rPicR[i * rCols + j];
			rotatePic.at<cv::Vec3b>(i, j)[1] = rPicG[i * rCols + j];
			rotatePic.at<cv::Vec3b>(i, j)[2] = rPicB[i * rCols + j];
		}
	}


	free(rPicR);
	free(rPicG);
	free(rPicB);
	free(picR);
	free(picG);
	free(picB);
	dealPic = rotatePic;

}