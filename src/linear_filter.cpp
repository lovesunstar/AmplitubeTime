#include "linear_filter.h"
#include <cmath>
#include <iostream>
using namespace std;

//
LinearFilter::LinearFilter(  ) 
{
	m_a_coeffs = new double[FILTER_MAX_ORDER+1];
	m_b_coeffs = new double[FILTER_MAX_ORDER+1];
	m_in_history = new double[FILTER_MAX_ORDER+1];
	m_out_history = new double[FILTER_MAX_ORDER+1];
	
	m_history_iter = 0;
}
//

void LinearFilter::processData(short * data, long num_samples, long long start_time)
{
	if(m_bypass){
		return;
	}
	
	double out;
	for(int i=0; i<num_samples; ++i){
		out = 0;
		
		m_in_history[m_history_iter] = data[i];
		for(int j=0; j<m_order+1; ++j){
			out += m_in_history[(m_history_iter+m_order+1-j)%(m_order+1)] * m_a_coeffs[j];
		}
		for(int j=1; j<m_order+1; ++j){
			out += m_out_history[(m_history_iter+m_order+1-j)%(m_order+1)] * m_b_coeffs[j];
		}
		m_out_history[m_history_iter] = out;
		
		data[i] = (short)out;
		
		m_history_iter = (m_history_iter+1)%(m_order+1);
	}
	
}

void LinearFilter::setOrder(int order)
{
	if(order != m_order){
		m_order = order;
		recalcCoeffs();
	}
}

void LinearFilter::reset()
{
	memset(m_in_history, 0, (FILTER_MAX_ORDER+1)*sizeof(double));
	memset(m_out_history, 0, (FILTER_MAX_ORDER+1)*sizeof(double));
}

Complex LinearFilter::val(double freq)
{
	Complex numer;
	Complex denom;
	
	Complex cur_z(1.0, 0.0);
	Complex z = exp(Complex(0.0, 2*M_PI*freq/m_freq));
	
	numer += m_a_coeffs[0];
	
	for(int i=1; i<m_order+1; ++i){
		cur_z /= z;
		numer += m_a_coeffs[i]*cur_z;
		denom += m_b_coeffs[i]*cur_z;
	}

	return numer/(1.0-denom);
}

double LinearFilter::amplitude(double freq)
{
	return abs(val(freq));
}

double LinearFilter::phase(double freq)
{
	return arg(val(freq));
}

int LinearFilter::getOrder()
{
	return m_order;
}

double LinearFilter::numerCoef(int i)
{
	return m_a_coeffs[i];
}

double LinearFilter::denomCoef(int i)
{
	return m_b_coeffs[i];
}
