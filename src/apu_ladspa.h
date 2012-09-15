#ifndef APU_LADSPA_H
#define APU_LADSPA_H
//
#include <QtGui>
#include "apu.h"
#include "ladspa.h"
//
struct ladspa_plugin_data{
	LADSPA_Descriptor * desc;
	QString lib_path;
	int index;
};
//
class ApuLADSPA : public Apu
{
Q_OBJECT

	LADSPA_Descriptor * m_ladspa_desc;
	LADSPA_Handle m_ladspa_handle;

	unsigned m_inputs;
	unsigned m_outputs;
	unsigned long * m_input_ports;
	unsigned long * m_output_ports;
	float * m_input_controls;
	float * m_output_controls;

	QLineEdit ** m_param_val;
	QSlider ** m_param_slider;

	QString m_lib_path;
	int m_plugin_index;

	void instantiate();
	void deactivate();

public:
	static LADSPA_Descriptor * getDescriptor(QString fname, int index);

	ApuLADSPA(ladspa_plugin_data & plugin_data, QWidget * parent=0);
	~ApuLADSPA();
	
	void setFreq(float freq);
	
	void processData(short * data, long num_samples, long long start_time);//start_time in msec
	void reset();

	void writeSettings(QSettings & s);
	void readSettings(QSettings & s);

	QString name();

	QString getLibraryPath();
	int getPluginIndex();

private slots:
	void controlSliderChanged(int slider_val);
	void controlValueChanged(QString val);
};
#endif
