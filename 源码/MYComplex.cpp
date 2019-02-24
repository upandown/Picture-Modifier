#include "MYComplex.h"

MYComplex::MYComplex(){
	real = 0;
	imag = 0;
}

MYComplex::MYComplex(double re, double im){
	real = re;
	imag = im;
}

MYComplex MYComplex::operator+(double a){
	return MYComplex(real + a, imag);
}

MYComplex MYComplex::operator-(double a){
	return MYComplex(real - a, imag);
}

MYComplex MYComplex::operator*(double a){
	return MYComplex(real*a, imag*a);
}

MYComplex MYComplex::operator/(double a){
	return MYComplex(real / a, imag / a);
}

MYComplex MYComplex::operator=(double a){
	real = a;
	imag = 0;
	return *this;
}

MYComplex MYComplex::operator+=(double a){
	real += a;
	return *this;
}

MYComplex MYComplex::operator-=(double a){
	real -= a;
	return *this;
}

MYComplex MYComplex::operator*=(double a){
	real *= a;
	imag *= a;
	return *this;
}

MYComplex MYComplex::operator/=(double a){
	real /= 2;
	imag /= 2;
	return *this;
}

MYComplex MYComplex::operator+(MYComplex b){
	return MYComplex(real + b.real, imag + b.imag);
}

MYComplex MYComplex::operator-(MYComplex b){
	return MYComplex(real - b.real, imag - b.imag);
}

MYComplex MYComplex::operator*(MYComplex b){
	double re = real * b.real - imag * b.imag;
	double im = real * b.imag + imag * b.real;
	return MYComplex(re, im);
}

MYComplex MYComplex::operator/(MYComplex b){
	double x = b.real;
	double y = b.imag;
	double f = x * x + y * y;
	double re = (real*x + imag * y) / f;
	double im = (imag*x - real * y) / f;
	return MYComplex(re, im);
}

MYComplex MYComplex::operator=(MYComplex b){
	real = b.real;
	imag = b.imag;
	return *this;
}

MYComplex MYComplex::operator+=(MYComplex b){
	real += b.real;
	imag += b.imag;
	return *this;
}

MYComplex MYComplex::operator-=(MYComplex b){
	real -= b.real;
	imag -= b.imag;
	return *this;
}

MYComplex MYComplex::operator*=(MYComplex b){
	double re = real * b.real - imag * b.imag;
	double im = real * b.imag + imag * b.real;
	real = re;
	imag = im;
	return *this;
}

MYComplex MYComplex::operator/=(MYComplex b){
	double x = b.real;
	double y = b.imag;
	double f = x * x + y * y;
	double re = (real*x + imag * y) / f;
	double im = (imag*x - real * y) / f;
	real = re;
	imag = im;
	return *this;
}

BOOL MYComplex::operator==(MYComplex b){
	return ((real == b.real) && (imag == b.imag));
}

BOOL MYComplex::operator!=(MYComplex b){
	return ((real != b.real) || (imag != b.imag));
}
