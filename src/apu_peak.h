#ifndef APU_PEAK_H
#define APU_PEAK_H
//
#include "linear_filter.h"
#include "afc_widget_peak.h"
//
class PeakingFilter : public LinearFilter
{
Q_OBJECT
	
	int m_f0;
	float m_Q;
	float m_dBgain;
	
	QIcon m_enabled_icon;
	QIcon m_disabled_icon;
	QPushButton * m_bypass_button;

	QSpinBox * m_f0_spin;
	QSpinBox * m_Q_spin;
	QSpinBox * m_gain_spin;

	AFCWidgetPeak * m_afc;
	
protected:
	void recalcCoeffs();

public:
	PeakingFilter(QWidget * parent=0);
	
	void setFreq(long freq);
	void writeSettings(QSettings & s);
	void readSettings(QSettings & s);

	QString name();
	
public slots:
	void setCentralFreq(int f0);
	void setQ(int Q);
	void setdBGain(int gain);

protected slots:
	void bypassSwitch();
	void parametersChanged(int dBgain, int freq);
};
#endif
