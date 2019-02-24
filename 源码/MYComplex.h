#ifndef MYCOMPLEX_H
#define MYCOMPLEX_H

#ifndef BOOLEAN_VAL
#define BOOLEAN_VAL
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

//这是自定义的复数类
class MYComplex{
public:
	MYComplex();
	MYComplex(double re, double im);
	//操作符重载
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
