#ifndef APU_H
#define APU_H
//
#include <QtGui>
//
class Apu : public QWidget
{
Q_OBJECT
protected:
	float m_freq;

public:
	Apu(QWidget * parent=0);
	
	virtual void setFreq(float freq);
	
	virtual void processData(short * data, long num_samples, long long start_time)=0;//start_time in msec
	virtual void reset()=0;

	virtual void writeSettings(QSettings & s);
	virtual void readSettings(QSettings & s);

	virtual QString name()=0;

signals:
	void changed();
};
#endif
