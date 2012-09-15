#include "apu_lowpass.h"
#include <QtOpenGL>

#include <iostream>
using namespace std;

//
LowPassFilter::LowPassFilter( QWidget * parent ) 
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
	QLabel * title_label = new QLabel(tr("<b>Low-pass filter</b>"));
	params_layout->addWidget(title_label, 0, 0);
	params_layout->addWidget(m_bypass_button, 0, 1);
	params_layout->addWidget(new QLabel(tr("Cutoff frequency")), 2, 0);

	params_layout->addWidget(new QWidget, 4, 0);
	params_layout->setRowStretch(4, 1);
	
	m_cutoff_spin = new QSpinBox;
	m_cutoff_spin->setRange(20, 20000);
	
	params_layout->addWidget(m_cutoff_spin, 2, 1);
	
	connect(m_cutoff_spin, SIGNAL(valueChanged(int)), this, SLOT(setCutoff(int)) );
	
	m_afc = new AFCWidgetLowHigh;
	main_layout->addWidget(m_afc, 1);
	m_afc->setdBRange(-20, 0);
	connect(m_afc, SIGNAL(parametersChanged(int)), this, SLOT(parametersChanged(int)) );
	
	m_cutoff = 2000;
	
	m_cutoff_spin->setValue(m_cutoff);
	
	recalcCoeffs();
	reset();
}
//
QString LowPassFilter::name()
{
	return tr("Low-pass filter");
}


void LowPassFilter::recalcCoeffs()
{
	m_order = 2;
	
	double w0 = 2*M_PI*m_cutoff/m_freq;//TODO: prewarp
	double Q = sqrt(2)/2;
	double alpha = sin(w0)/2.0/Q;
	
	double norm_coef = 1+alpha;
	
	m_a_coeffs[0] = (1-cos(w0))/2;
	m_a_coeffs[1] = 1-cos(w0);
	m_a_coeffs[2] = (1-cos(w0))/2;
	
	m_b_coeffs[1] = -2*cos(w0);
	m_b_coeffs[2] = 1-alpha;
	
	m_a_coeffs[0] /= norm_coef;
	m_a_coeffs[1] /= norm_coef;
	m_a_coeffs[2] /= norm_coef;
	
	m_b_coeffs[1] /= -norm_coef;
	m_b_coeffs[2] /= -norm_coef;
	

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

void LowPassFilter::setCutoff(int cutoff)
{
	if(cutoff != m_cutoff){
		m_cutoff = cutoff;
		recalcCoeffs();
	}
}

void LowPassFilter::setFreq(long freq)
{
	if(freq != m_freq){
		m_freq = freq;
		m_afc->setFreqRange(20, freq/2);
		recalcCoeffs();
	}
}

void LowPassFilter::bypassSwitch()
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

void LowPassFilter::parametersChanged(int freq)
{
	m_cutoff = freq;
	m_cutoff_spin->setValue(m_cutoff);
	recalcCoeffs();
}

void LowPassFilter::writeSettings(QSettings & s)
{
	s.setValue("cutoff", m_cutoff);
}

void LowPassFilter::readSettings(QSettings & s)
{
	qDebug() << "LowPassFilter::readSettings";
	setCutoff(s.value("cutoff", 1000).toInt());

	m_cutoff_spin->setValue(m_cutoff);
}
