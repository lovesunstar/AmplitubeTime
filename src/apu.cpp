#include "apu.h"
//
Apu::Apu( QWidget * parent ) 
	: QWidget(parent)
{
	m_freq = 44100;
}
//

void Apu::setFreq(float freq)
{
	if(freq != m_freq){
		m_freq = freq;
		update();
	}
}

void Apu::writeSettings(QSettings & s)
{
	
}

void Apu::readSettings(QSettings & s)
{
	
}
