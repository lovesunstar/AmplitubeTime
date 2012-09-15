#ifndef LINEAR_FILTER_H
#define LINEAR_FILTER_H
//
#include <QWidget>
#include "apu.h"
#include <complex>

typedef std::complex<double> Complex;

#define FILTER_MAX_ORDER 10
//
class LinearFilter : public Apu
{
Q_OBJECT
protected:

	double * m_in_history;
	double * m_out_history;
	double * m_a_coeffs;
	double * m_b_coeffs;
	
	int m_history_iter;
	int m_order;
	
	bool m_bypass;
	
	virtual void recalcCoeffs()=0;

public:
	LinearFilter();
	
	void processData(short * data, long num_samples, long long start_time);
	double amplitude(double freq);
	double phase(double freq);
	Complex val(double freq);
	void reset();

	int getOrder();
	double numerCoef(int i);
	double denomCoef(int i);
	
public slots:
	void setOrder(int order);
};
#endif
