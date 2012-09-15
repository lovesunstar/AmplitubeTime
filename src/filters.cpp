#include "filters.h"
#include <iostream>
#include <algorithm>

using namespace std;

void zeros2coeffs(Complex * zeros, var_type * coeffs, int size)//symmetric polynoms
{
	int * b = new int[size+1];
	memset(b, 0, sizeof(int)*(size+1));
	int i = 0;
	
	for(int i=0; i<size+1; ++i){
		coeffs[i] = 0.0;
	}
	
	while( !b[size] ){
		i = 0;
		while( b[i]) b[i++] = 0; // моделируем перенос в следующий разряд, возникающий при сложении
		b[i] = 1;
 
		int subset_size = 0;
		Complex summand = 1.0;
		for(i = 0; i < size; i++){
			if(b[i]){
				summand *= zeros[i];
				++subset_size;
			}
		}
		coeffs[size-subset_size] += summand.real();
	}

	delete [] b;

	for(int i=(size+1)%2; i<size; i+=2){
		coeffs[i] = -coeffs[i];
	}

	coeffs[size]=1.0;
}

void poly2filter(var_type * a, var_type * b, int size)
{
	var_type k0_numer=0.0;
	var_type k0_denom=0.0;
	for(int i=0; i<size+1; ++i){
		k0_numer += a[i];
		k0_denom += b[i];
	}
	
	var_type k0 = k0_denom/k0_numer;
	
	reverse(a, a+size+1);
	reverse(b, b+size+1);
	for(int i=0; i<size+1; ++i){
		a[i] *= k0;
	}
	for(int i=1; i<size+1; ++i){
		b[i] = -b[i];
	}
}

var_type warp_freq(var_type freq, var_type Fs)
{
	return Fs*tan(M_PI*freq/Fs)/M_PI;
}

Complex p2z(Complex p, var_type Fs)
{
	return (Complex(2*Fs, 0.0)+p)/(Complex(2*Fs, 0.0)-p);
}

void butterworth_lowpass(Complex * zeros, Complex * poles, int order, var_type Fs, var_type cutoff)
{
	cutoff = 2*Fs*tan(M_PI*cutoff/Fs);//warp analog freq in Hz to digital in rad/sec

	int num = order+1;
	
	var_type arg;
	for(int i=0; i<num; ++i){
		arg = M_PI*(2*i+num+1)/2/num;
		poles[i] = Complex(std::cos(arg), std::sin(arg));
		poles[i] *= cutoff;
		poles[i] = p2z(poles[i], Fs);
		
		zeros[i] = Complex(-1.0, 0.0);
	}
}

