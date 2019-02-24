#ifndef MYCOMPLEX_H
#define MYCOMPLEX_H

#ifndef BOOLEAN_VAL
#define BOOLEAN_VAL
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

//�����Զ���ĸ�����
class MYComplex{
public:
	MYComplex();
	MYComplex(double re, double im);
	//����������
	MYComplex operator=(double a);
	MYComplex operator-=(double a);
	MYComplex operator*=(double a);
	MYComplex operator+=(double a);
	MYComplex operator/=(double a);
	MYComplex operator/(double a);
	MYComplex operator+(double a);
	MYComplex operator-(double a);
	MYComplex operator*(double a);



	MYComplex operator=(MYComplex b);
	MYComplex operator+(MYComplex b);
	MYComplex operator-(MYComplex b);
	MYComplex operator*(MYComplex b);
	MYComplex operator/(MYComplex b);
	MYComplex operator+=(MYComplex b);
	MYComplex operator-=(MYComplex b);
	MYComplex operator*=(MYComplex b);
	MYComplex operator/=(MYComplex b);

	BOOL operator==(MYComplex b);
	BOOL operator!=(MYComplex b);

	double real;
	double imag;
};

#endif
