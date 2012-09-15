#ifndef APU_BANDPASS_H
#define APU_BANDPASS_H
//
#include "linear_filter.h"
#include "afc_widget_bpf.h"
//
class BandPassFilter : public LinearFilter
{
Q_OBJECT
	
	float m_f0;
	float m_Q;
	
	QIcon m_enabled_icon;
	QIcon m_disabled_icon;
	QPushButton * m_bypass_button;

	QSpinBox * m_f0_spin;
	QSpinBox * m_Q_spin;
	QSpinBox * m_gain_spin;

	AFCWidgetBandPass * m_afc;
	
protected:
	void recalcCoeffs();

public:
	BandPassFilter(QWidget * parent=0);
	
	void setFreq(int freq);
	void writeSettings(QSettings & s);
	void readSettings(QSettings & s);

	QString name();
	
public slots:
	void setCentralFreq(int f0);
	void setQ(int Q);

protected slots:
	void bypassSwitch();
	void parametersChanged(int freq);
};
#endif
