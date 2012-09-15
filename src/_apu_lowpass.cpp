#include "apu_lowpass.h"
#include "filters.h"
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
	params_layout->addWidget(new QLabel(tr("Filter order")), 3, 0);

	params_layout->addWidget(new QWidget, 4, 0);
	params_layout->setRowStretch(4, 1);
	
	m_cutoff_spin = new QSpinBox;
	m_cutoff_spin->setRange(20, 20000);
	m_order_spin = new QSpinBox;
	m_order_spin->setRange(1, FILTER_MAX_ORDER);
	
	params_layout->addWidget(m_cutoff_spin, 2, 1);
	params_layout->addWidget(m_order_spin, 3, 1);
	
	connect(m_cutoff_spin, SIGNAL(valueChanged(int)), this, SLOT(setCutoff(int)) );
	connect(m_order_spin, SIGNAL(valueChanged(int)), this, SLOT(setOrder(int)) );
	
	m_afc = new AFCWidget;
	main_layout->addWidget(m_afc, 1);
	m_afc->setdBRange(-60, 0);
	
	m_order = 2;
	m_cutoff = 2000;
	
	m_order_spin->setValue(m_order);
	m_cutoff_spin->setValue(m_cutoff);
	
	recalcCoeffs();
	reset();
}
//

void LowPassFilter::recalcCoeffs()
{
	Complex zeros[FILTER_MAX_ORDER+1];
	Complex poles[FILTER_MAX_ORDER+1];
	butterworth_lowpass(zeros, poles, m_order, m_freq, m_cutoff);
	zeros2coeffs(zeros, m_a_coeffs, m_order);
	zeros2coeffs(poles, m_b_coeffs, m_order);
	poly2filter(m_a_coeffs, m_b_coeffs, m_order);
	/*
	qDebug() << "order: " << m_order;
	qDebug() << "cutoff: " << m_cutoff;
	for(int i=0; i<m_order+1; ++i){
		cout << "a" << i << " " << m_a_coeffs[i] << endl;
	}
	for(int i=0; i<m_order+1; ++i){
		cout << "b" << i << " " << m_b_coeffs[i] << endl;
	}
	cout << endl;*/
        /*
	qDebug() << "a:";
	for(int i=0; i<m_order+1; ++i){
		qDebug() << m_a_coeffs[i];
	}
	qDebug() << "b:";
	for(int i=0; i<m_order+1; ++i){
		qDebug() << m_b_coeffs[i];
	}
	qDebug() << "|K(z)| f=1000Hz =" << amplitude(1000);
	qDebug() << "|K(z)| f=2000Hz =" << amplitude(2000);
	qDebug() << "|K(z)| f=3000Hz =" << amplitude(3000);
	qDebug() << "|K(z)| f=4000Hz =" << amplitude(4000);
	qDebug() << "\n";
        */
	m_afc->clear();
	for(int i=0; i<m_freq/2; i+=m_freq/200){
		m_afc->addPoint(float(i)*2/m_freq, 20*log10(amplitude(i)) );
	}
	m_afc->update();
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
		m_afc->setFreq(freq);
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

void LowPassFilter::writeSettings(QSettings & s)
{
	s.setValue("cutoff", m_cutoff);
	s.setValue("order", m_order);
}

void LowPassFilter::readSettings(QSettings & s)
{
	qDebug() << "LowPassFilter::readSettings";
	setCutoff(s.value("cutoff", 1000).toInt());
	setOrder(s.value("order", 2).toInt());

	m_order_spin->setValue(m_order);
	m_cutoff_spin->setValue(m_cutoff);
}
