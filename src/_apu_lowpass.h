#ifndef APU_LOWPASS_H
#define APU_LOWPASS_H
//
#include "linear_filter.h"
#include "afc_widget.h"
//
class LowPassFilter : public LinearFilter
{
Q_OBJECT
	
	int m_cutoff;
	
	QIcon m_enabled_icon;
	QIcon m_disabled_icon;
	QPushButton * m_bypass_button;

	QSpinBox * m_cutoff_spin;
	QSpinBox * m_order_spin;

	AFCWidget * m_afc;
	
protected:
	void recalcCoeffs();

public:
	LowPassFilter(QWidget * parent=0);
	
	void setFreq(long freq);
	void writeSettings(QSettings & s);
	void readSettings(QSettings & s);
	
public slots:
	void setCutoff(int cutoff);

protected slots:
	void bypassSwitch();
};
#endif
