#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <complex>
#include <cmath>

typedef double var_type;
typedef std::complex<var_type> Complex;

//common functions
void zeros2coeffs(Complex * zeros, var_type * coeffs, int size);
void poly2filter(var_type * a, var_type * b, int size);
void p2z(Complex * p, Complex * z, int size, var_type Fs);
var_type warp_freq(var_type freq, var_type Fs);

//BUTTERWORTH
//zeros and poles in z-domain
//order is filter order
//Fs is sampling frequency
//cutoff is analog frequency in herzs
void butterworth_lowpass(Complex * zeros, Complex * poles, int order, var_type Fs, var_type cutoff);

#endif // __FILTERS_H__
