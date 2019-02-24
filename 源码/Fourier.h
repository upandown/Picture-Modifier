#ifndef FOURIER_H
#define FOURIER_H

#include "MYComplex.h"
#include <stdlib.h>
#include <math.h>


const double PI = 3.14159265358979;


extern void FFT( MYComplex * in, MYComplex*out, int n);

#endif

