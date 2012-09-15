#include "apu_peak.h"
#include "filters.h"
#include <QtOpenGL>

#include <iostream>
using namespace std;

//
PeakingFilter::PeakingFilter( QWidget * parent ) 
	: LinearFilter(),
	m_enabled_icon(":/images/on.png"),
	m_disabled_icon(":/images/off.png")
{
	m_bypass = false;
	
	QBoxLayout * main_layout = new QHBoxLayout;
	main_layout->setContentsMargins(0,0,0,0);
	setLayout(main_layout);
	
	m_bypass_button = new QPushButton(m_enabled_icon, "on");
	m_bypass_button->setCheckable(true);
	m_bypass_button->setChecked(true);
	connect(m_bypass_button, SIGNAL(clicked()), this, SLOT(bypassSwitch()) );
	
	QGridLayout * params_layout = new QGridLayout; 
	main_layout->addLayout(params_layout);
	main_layout->setStretchFactor(params_layout, 0);
	QLabel * title_label = new QLabel(tr("<b>Peaking filter</b>"));
	params_layout->addWidget(title_label, 0, 0);
	params_layout->addWidget(m_bypass_button, 0, 1);
	params_layout->addWidget(new QLabel(tr("Central frequency")), 2, 0);
	params_layout->addWidget(new QLabel(tr("Q")), 3, 0);
	params_layout->addWidget(new QLabel(tr("dB gain")), 4, 0);

	params_layout->addWidget(new QWidget, 5, 0);
	params_layout->setRowStretch(5, 1);
	
	m_f0_spin = new QSpinBox;
	m_f0_spin->setRange(20, 20000);
	m_Q_spin = new QSpinBox;
	m_Q_spin->setRange(1, 100);
	m_gain_spin = new QSpinBox;
	m_gain_spin->setRange(-60, 20);
	
	params_layout->addWidget(m_f0_spin, 2, 1);
	params_layout->addWidget(m_Q_spin, 3, 1);
	params_layout->addWidget(m_gain_spin, 4, 1);
	
	connect(m_f0_spin, SIGNAL(valueChanged(int)), this, SLOT(setCentralFreq(int)) );
	connect(m_Q_spin, SIGNAL(valueChanged(int)), this, SLOT(setQ(int)) );
	connect(m_gain_spin, SIGNAL(valueChanged(int)), this, SLOT(setdBGain(int)) );
	
	m_afc = new AFCWidgetPeak;
	main_layout->addWidget(m_afc, 1);
	m_afc->setdBRange(-60, 20);
	connect(m_afc, SIGNAL(parametersChanged(int, int)), this, SLOT(parametersChanged(int, int)) );
	
	m_f0 = 2000;
	m_dBgain = 0;
	m_Q = 20;
	
	m_f0_spin->setValue(m_f0);
	m_Q_spin->setValue(m_Q);
	m_gain_spin->setValue(m_dBgain);
	
	recalcCoeffs();
	reset();
}
//
QString PeakingFilter::name()
{
	return tr("Peaking filter");
}

void PeakingFilter::parametersChanged(int dBgain, int freq)
{
	m_dBgain = dBgain;
	m_f0 = freq;
	m_f0_spin->setValue(m_f0);
	m_gain_spin->setValue(m_dBgain);
	recalcCoeffs();
}

void PeakingFilter::recalcCoeffs()
{
	m_order = 2;
	
	double w0 = 2*M_PI*m_f0/m_freq;//TODO: prewarp
	double A = pow(10.0, m_dBgain/40.0);
	double alpha = sin(w0)/2.0/m_Q;
	
	double norm_coef = 1+alpha/A;
	
	m_a_coeffs[0] = 1+alpha*A;
	m_a_coeffs[1] = -2*cos(w0);
	m_a_coeffs[2] = 1-alpha*A;
	
	m_b_coeffs[1] = -2*cos(w0);
	m_b_coeffs[2] = 1-alpha/A;
	
	m_a_coeffs[0] /= norm_coef;
	m_a_coeffs[1] /= norm_coef;
	m_a_coeffs[2] /= norm_coef;
	
	m_b_coeffs[1] /= -norm_coef;
	m_b_coeffs[2] /= -norm_coef;
	
	/*
	for(int i=0; i<m_order+1; ++i){
		cout << "a" << i << " " << m_a_coeffs[i] << endl;
	}
	for(int i=0; i<m_order+1; ++i){
		cout << "b" << i << " " << m_b_coeffs[i] << endl;
	}
	cout << endl;
	*/

	m_afc->clear();
	int pts = 1000;
	for(int i=0; i<=pts; ++i){
		float freq = 19+pow(10.0, i*log10(20000-20)/pts);
		Complex val = this->val(freq);
		m_afc->addPointdB(freq, 20*log10(abs(val)) );
		m_afc->addPointRad(freq, arg(val) );
	}
	m_afc->update();

	emit changed();
}

void PeakingFilter::setCentralFreq(int f0)
{
	if(f0 != m_f0){
		m_f0 = f0;
		recalcCoeffs();
	}
}

void PeakingFilter::setFreq(long freq)
{
	if(freq != m_freq){
		m_freq = freq;
		m_afc->setFreqRange(20, freq/2);
		recalcCoeffs();
	}
}

void PeakingFilter::setQ(int Q)
{
	if(Q != m_Q){
		m_Q = Q;
		recalcCoeffs();
	}
}

void PeakingFilter::setdBGain(int gain)
{
	if(gain != m_dBgain){
		m_dBgain = gain;
		recalcCoeffs();
	}
}

void PeakingFilter::bypassSwitch()
{
	m_bypass = !m_bypass;
	
	if(m_bypass){
		m_bypass_button->setText("off");
		m_bypass_button->setIcon(m_disabled_icon);
	}
	else{
		m_bypass_button->setText("on");
		m_bypass_button->setIcon(m_enabled_icon);
	}
}


void PeakingFilter::writeSettings(QSettings & s)
{
	s.setValue("f0", m_f0);
	s.setValue("Q", m_Q);
	s.setValue("dBgain", m_dBgain);
}

void PeakingFilter::readSettings(QSettings & s)
{
	setCentralFreq(s.value("f0", 1000).toInt());
	setQ(s.value("Q", 10).toInt());
	setdBGain(s.value("dBgain", 0).toInt());

	m_f0_spin->setValue(m_f0);
	m_Q_spin->setValue(m_Q);
	m_gain_spin->setValue(m_dBgain);
}
