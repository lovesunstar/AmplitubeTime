#include "apu_ladspa.h"
//
#define SLIDER_MAX 1000000

LADSPA_Descriptor * ApuLADSPA::getDescriptor(QString fname, int index)
{
	QLibrary lib (fname);
	if(!lib.load()){
		qDebug() << "error:" << fname << "library cannot be loaded";
		return 0;
	}

	LADSPA_Descriptor_Function ladspa_desc_func = (LADSPA_Descriptor_Function)lib.resolve("ladspa_descriptor");
	if(!ladspa_desc_func) return 0;
	LADSPA_Descriptor * ladspa_desc = (LADSPA_Descriptor*)ladspa_desc_func(index);

	return ladspa_desc;
}

ApuLADSPA::ApuLADSPA(ladspa_plugin_data & plugin_data, QWidget * parent)
	: Apu(parent)
{
	m_lib_path = plugin_data.lib_path;
	m_plugin_index = plugin_data.index;
	m_ladspa_desc = plugin_data.desc;
	m_ladspa_handle = 0;

	QBoxLayout * main_layout = new QVBoxLayout;
	main_layout->setContentsMargins(0,0,0,0);
	setLayout(main_layout);
	main_layout->addWidget(new QLabel(QString("<b>")+m_ladspa_desc->Name+"</b> by "+m_ladspa_desc->Maker+" ("+m_ladspa_desc->Copyright+")"));
	QGridLayout * params_layout = new QGridLayout;
	main_layout->addLayout(params_layout);

	setFreq(44100);

	m_input_ports = new unsigned long [m_ladspa_desc->PortCount];
	m_output_ports = new unsigned long [m_ladspa_desc->PortCount];
	m_input_controls = new float [m_ladspa_desc->PortCount];
	m_output_controls = new float [m_ladspa_desc->PortCount];

	m_param_val = new QLineEdit*[m_ladspa_desc->PortCount];
	m_param_slider = new QSlider*[m_ladspa_desc->PortCount];

	m_inputs = 0;
	m_outputs = 0;
	int controls = 0;

	for(int p=0; p<m_ladspa_desc->PortCount; p++) {
		LADSPA_PortDescriptor d = m_ladspa_desc->PortDescriptors[p];
		
		if (LADSPA_IS_PORT_AUDIO(d)) {
			if (LADSPA_IS_PORT_INPUT(d)) {
				m_input_ports[m_inputs] = p;
				//qDebug() << "input_port" << p;
				m_inputs++;
			}
			else
			if (LADSPA_IS_PORT_OUTPUT(d)) {
				m_output_ports[m_outputs] = p;
				//qDebug() << "output_port" << p;
				m_outputs++;
			}
		}

		if (LADSPA_IS_PORT_CONTROL(d) && LADSPA_IS_PORT_INPUT(d)) {
			//numInputControls++;

			float val = float(1.0);
			LADSPA_PortRangeHint hint = m_ladspa_desc->PortRangeHints[p];

			if (LADSPA_IS_HINT_BOUNDED_BELOW(hint.HintDescriptor) && val < hint.LowerBound)
				val = hint.LowerBound;

			if (LADSPA_IS_HINT_BOUNDED_ABOVE(hint.HintDescriptor) && val > hint.UpperBound)
				val = hint.UpperBound;

			if (LADSPA_IS_HINT_DEFAULT_MINIMUM(hint.HintDescriptor))
				val = hint.LowerBound;

			if (LADSPA_IS_HINT_DEFAULT_LOW(hint.HintDescriptor))
				val = hint.LowerBound * 0.75f + hint.UpperBound * 0.25f;

			if (LADSPA_IS_HINT_DEFAULT_MIDDLE(hint.HintDescriptor))
				val = hint.LowerBound * 0.5f + hint.UpperBound * 0.5f;

			if (LADSPA_IS_HINT_DEFAULT_HIGH(hint.HintDescriptor))
				val = hint.LowerBound * 0.25f + hint.UpperBound * 0.75f;

			if (LADSPA_IS_HINT_DEFAULT_MAXIMUM(hint.HintDescriptor))
				val = hint.UpperBound;

			if (LADSPA_IS_HINT_SAMPLE_RATE(hint.HintDescriptor))
				val *= 44100;

			if (LADSPA_IS_HINT_DEFAULT_0(hint.HintDescriptor))
				val = 0.0f;

			if (LADSPA_IS_HINT_DEFAULT_1(hint.HintDescriptor))
				val = 1.0f;

			if (LADSPA_IS_HINT_DEFAULT_100(hint.HintDescriptor))
				val = 100.0f;

			if (LADSPA_IS_HINT_DEFAULT_440(hint.HintDescriptor))
				val = 440.0f;

			m_input_controls[p] = val;

			QLabel * param_name = new QLabel(m_ladspa_desc->PortNames[p]);
			m_param_val[p] = new QLineEdit(QString::number(val));
			m_param_slider[p] = new QSlider(Qt::Horizontal);

			m_param_val[p]->setValidator(new QDoubleValidator((double)hint.LowerBound, (double)hint.UpperBound, 4, this));
			m_param_slider[p]->setRange(0, SLIDER_MAX);
			int slider_val = int((val-hint.LowerBound)*SLIDER_MAX/(hint.UpperBound-hint.LowerBound));
			m_param_slider[p]->setValue(slider_val);

			m_param_val[p]->setProperty("port", QVariant(p));
			m_param_slider[p]->setProperty("port", QVariant(p));

			connect(m_param_val[p], SIGNAL(textChanged(QString)), this, SLOT(controlValueChanged(QString)) );
			connect(m_param_slider[p], SIGNAL(valueChanged(int)), this, SLOT(controlSliderChanged(int)) );

			params_layout->addWidget(param_name, controls, 0);
			params_layout->addWidget(m_param_val[p], controls, 1);
			params_layout->addWidget(m_param_slider[p], controls, 2);

			++controls;
		}
	}

	params_layout->addWidget(new QWidget, controls, 0);
	params_layout->setRowStretch(controls, 1);

	instantiate();
}
//
ApuLADSPA::~ApuLADSPA()
{
	delete [] m_input_ports;
	delete [] m_output_ports;
	delete [] m_input_controls;
	delete [] m_output_controls;
	delete [] m_param_val;
	delete [] m_param_slider;
}

void ApuLADSPA::controlSliderChanged(int slider_val)
{
	int port = sender()->property("port").toInt();
	LADSPA_PortRangeHint hint = m_ladspa_desc->PortRangeHints[port];
	float range = hint.UpperBound-hint.LowerBound;
	m_input_controls[port] = hint.LowerBound+float(slider_val)*range/SLIDER_MAX;

	m_param_val[port]->setText( QString::number(m_input_controls[port]) );
}

void ApuLADSPA::controlValueChanged(QString val)
{
	int port = sender()->property("port").toInt();
	LADSPA_PortRangeHint hint = m_ladspa_desc->PortRangeHints[port];
	float range = hint.UpperBound-hint.LowerBound;
	m_input_controls[port] = val.toFloat();

	int slider_val = int((m_input_controls[port]-hint.LowerBound)*SLIDER_MAX/range);
	m_param_slider[port]->setValue(slider_val);
}

void ApuLADSPA::instantiate()
{
	if(m_ladspa_handle){
		if(m_ladspa_desc->deactivate){
			m_ladspa_desc->deactivate(m_ladspa_handle);
		}
		if(m_ladspa_desc->cleanup){
			m_ladspa_desc->cleanup(m_ladspa_handle);
		}
	}
		
	m_ladspa_handle = m_ladspa_desc->instantiate(m_ladspa_desc, (unsigned long)m_freq);
	//qDebug() << "ladspa handle = " << m_ladspa_handle;
	if(m_ladspa_desc->activate){
		m_ladspa_desc->activate(m_ladspa_handle);
	}
}

void ApuLADSPA::deactivate()
{
	if(m_ladspa_handle){
		if(m_ladspa_desc->deactivate){
			m_ladspa_desc->deactivate(m_ladspa_handle);
		}
		if(m_ladspa_desc->cleanup){
			m_ladspa_desc->cleanup(m_ladspa_handle);
		}
	}
}

void ApuLADSPA::setFreq(float freq)
{
	if(freq != m_freq){
		m_freq = freq;

		instantiate();
	}
}

QString ApuLADSPA::getLibraryPath()
{
	return m_lib_path;
}

int ApuLADSPA::getPluginIndex()
{
	return m_plugin_index;
}

QString ApuLADSPA::name()
{
	if(!m_ladspa_desc) return "WARNING: ladspa descriptor=0";
	return m_ladspa_desc->Name;
}

void ApuLADSPA::writeSettings(QSettings & s)
{
	for(int port=0; port<m_ladspa_desc->PortCount; ++port){
		LADSPA_PortDescriptor d = m_ladspa_desc->PortDescriptors[port];
		if (LADSPA_IS_PORT_CONTROL(d) && LADSPA_IS_PORT_INPUT(d)) {
			s.setValue(QString::number(port), m_input_controls[port]);
		}
	}
}

void ApuLADSPA::readSettings(QSettings & s)
{
	foreach(QString key, s.childKeys()){
		bool ok;
		int port = key.toInt(&ok);
		if(!ok) continue;

		LADSPA_PortDescriptor d = m_ladspa_desc->PortDescriptors[port];
		if (LADSPA_IS_PORT_CONTROL(d) && LADSPA_IS_PORT_INPUT(d)) {
			m_input_controls[port] = float(s.value(key).toDouble());
			m_param_val[port]->setText(QString::number(m_input_controls[port]));
		}
		else{
			qDebug() << "WARNING: port #" << port << "is not input control";
		}
	}
}

void ApuLADSPA::processData(short * data, long num_samples, long long start_time)
{
	float * in_buffer = new float[num_samples];
	float * out_buffer = new float[num_samples];

	for(int i=0; i<num_samples; ++i){
		in_buffer[i] = data[i];
	}

	unsigned long p;

	for(p=0; p<m_inputs; ++p) {
		m_ladspa_desc->connect_port(m_ladspa_handle, m_input_ports[p], in_buffer);
	}
	for(p=0; p<m_outputs; ++p) {
		m_ladspa_desc->connect_port(m_ladspa_handle, m_output_ports[p], out_buffer);
	}
//	qDebug() << "in -> out; " << m_input_ports[0] << " -> " << m_output_ports[0];
//	m_ladspa_desc->connect_port(m_ladspa_handle, m_input_ports[0], in_buffer);
//	m_ladspa_desc->connect_port(m_ladspa_handle, m_output_ports[0], out_buffer);

	for(p=0; p<m_ladspa_desc->PortCount; ++p) {
		LADSPA_PortDescriptor d = m_ladspa_desc->PortDescriptors[p];
		if (LADSPA_IS_PORT_CONTROL(d)) {
			if (LADSPA_IS_PORT_INPUT(d)) {
				m_ladspa_desc->connect_port(m_ladspa_handle, p, &m_input_controls[p]);
			}
			else {
				m_ladspa_desc->connect_port(m_ladspa_handle, p, &m_output_controls[p]);
			}
		}
	}

	m_ladspa_desc->run(m_ladspa_handle, num_samples);

	for(int i=0; i<num_samples; ++i){
		data[i] = (short)out_buffer[i];
	}

	delete [] in_buffer;
	delete [] out_buffer;
}

void ApuLADSPA::reset()
{
}
