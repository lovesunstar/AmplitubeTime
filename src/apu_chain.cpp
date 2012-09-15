#include "apu_chain.h"
#include "apu_lowpass.h"
#include "apu_highpass.h"
#include "apu_bandpass.h"
#include "apu_peak.h"
#include "apu_ladspa.h"

#include <QtOpenGL>
//


//
ApuChain::ApuChain() 
	: QWidget()
{
	m_freq = 44100;

	QBoxLayout * main_layout = new QVBoxLayout;
	setLayout(main_layout);
	
	m_spectrum_in = new SpectrumView;
	m_spectrum_out = new SpectrumView;
	m_spectrum_in->setUpdateTime(1.0/40.0);
	m_spectrum_out->setUpdateTime(1.0/40.0);
	m_apu.push_back(m_spectrum_in);
	m_apu.push_back(m_spectrum_out);
	
	QGroupBox * spectrum_box;
	QBoxLayout * spectrum_layout;
	
	/*
	spectrum_box = new QGroupBox(tr("Input signal spectrum"));
	spectrum_box->setAlignment(Qt::AlignHCenter);
	spectrum_layout = new QVBoxLayout;
	spectrum_layout->setContentsMargins(2,2,5,5);
	spectrum_layout->addWidget(m_spectrum_in);
	spectrum_box->setLayout(spectrum_layout);
	main_layout->addWidget(spectrum_box);
	main_layout->setStretchFactor(spectrum_box, 1);
	*/

	
	QGroupBox * apu_queue_group_box = new QGroupBox(tr("Audio processing queue"));
	main_layout->addWidget(apu_queue_group_box);
	main_layout->setStretchFactor(apu_queue_group_box, 0);
	apu_queue_group_box->setAlignment(Qt::AlignHCenter);
	
	m_apu_queue_vlayout = new QVBoxLayout;
	apu_queue_group_box->setLayout(m_apu_queue_vlayout);

	m_apu_queue_layout = new QHBoxLayout;
	m_apu_queue_vlayout->addLayout(m_apu_queue_layout, 1);

	m_apu_list = new QStandardItemModel(0, 3, this);
	//m_apu_list->setHeaderData(0, Qt::Horizontal, tr("Apu name"));
	//m_apu_list->setHeaderData(1, Qt::Horizontal, tr("Apu widget box pointer"));
	//m_apu_list->setHeaderData(2, Qt::Horizontal, tr("Apu widget pointer"));

	m_apu_list_view = new QTreeView;
	m_apu_list_view->setModel(m_apu_list);
	m_apu_list_view->setRootIsDecorated(false);
	m_apu_list_view->setAlternatingRowColors(false);
	m_apu_list_view->setSelectionMode(QAbstractItemView::SingleSelection);
	m_apu_list_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_apu_list_view->setHeaderHidden(true);
	m_apu_list_view->setColumnHidden(1, true);
	m_apu_list_view->setColumnHidden(2, true);
	connect(m_apu_list_view->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(apuListSelected(QItemSelection)) );
	m_apu_queue_layout->addWidget(m_apu_list_view);

        
	QBoxLayout * management_layout = new QHBoxLayout;
	m_apu_queue_vlayout->addLayout(management_layout);
	QPushButton * add_apu_button = new QPushButton(QIcon(":/images/add.png"), tr("Add unit "));
	management_layout->addWidget(add_apu_button);
	m_moveup_apu_button = new QPushButton(QIcon(":/images/arrow_up.png"), tr(""));
	management_layout->addWidget(m_moveup_apu_button);
	m_movedown_apu_button = new QPushButton(QIcon(":/images/arrow_down.png"), tr(""));
	management_layout->addWidget(m_movedown_apu_button);
	m_remove_apu_button = new QPushButton(QIcon(":/images/cancel.png"), tr(""));
	management_layout->addWidget(m_remove_apu_button);
	m_coeffs_button = new QPushButton(QIcon(":/images/numbers.png"), tr(""));
	management_layout->addWidget(m_coeffs_button);
	QPushButton * save_button = new QPushButton(QIcon(":/images/save.png"), tr("Save chain"));
	management_layout->addWidget(save_button);
	QPushButton * open_button = new QPushButton(QIcon(":/images/fileopen.png"), tr("Load chain"));
	management_layout->addWidget(open_button);
	management_layout->addStretch(1);

	m_remove_apu_button->setDisabled(true);
	m_moveup_apu_button->setDisabled(true);
	m_movedown_apu_button->setDisabled(true);
	m_coeffs_button->setDisabled(true);

	connect(m_remove_apu_button, SIGNAL(clicked()), this, SLOT(removeApu()) );
	connect(m_moveup_apu_button, SIGNAL(clicked()), this, SLOT(moveUpApu()) );
	connect(m_movedown_apu_button, SIGNAL(clicked()), this, SLOT(moveDownApu()) );
	connect(m_coeffs_button, SIGNAL(clicked()), this, SLOT(viewFilterCoeffs()) );
	connect(save_button, SIGNAL(clicked()), this, SLOT(saveSettings()) );
	connect(open_button, SIGNAL(clicked()), this, SLOT(loadSettings()) );


	/*
	QBoxLayout * apu_box_layout = new QVBoxLayout;
	apu_queue_group_box->setLayout(apu_box_layout);
	QScrollArea * scroll_area = new QScrollArea;
	apu_box_layout->addWidget(scroll_area);
	
	QWidget * scroll_widget = new QWidget;
	scroll_area->setWidget(scroll_widget);
	scroll_widget->setLayout(m_apu_queue_layout);
	*/

	m_apu_preview = 0;
	m_apu_widget_preview = 0;
	selectApuWidget(0, 0);


	QMenu * menu = createAddApuMenu();
	add_apu_button->setMenu(menu);
	
	
	m_final_afc = new AFCWidget;
	m_final_afc->setdBRange(20, -40);
	QGroupBox * afc_box = new QGroupBox(tr("Final characteristic"));
	afc_box->setAlignment(Qt::AlignHCenter);
	QBoxLayout * afc_layout = new QVBoxLayout;
	afc_layout->setContentsMargins(2,2,5,5);
	afc_layout->addWidget(m_final_afc);
	afc_box->setLayout(afc_layout);
	main_layout->addWidget(afc_box, 1);

	
	spectrum_box = new QGroupBox(tr("Output signal spectrum"));
	spectrum_box->setAlignment(Qt::AlignHCenter);
	spectrum_layout = new QVBoxLayout;
	spectrum_layout->setContentsMargins(2,2,5,5);
	spectrum_layout->addWidget(m_spectrum_out);
	spectrum_box->setLayout(spectrum_layout);
	main_layout->addWidget(spectrum_box, 1);
}
//
QMenu * ApuChain::createAddApuMenu()
{
	QMenu * menu = new QMenu;
	QAction * add_act;
	
	add_act = new QAction(QIcon(":/images/lpf.png"), tr("Low-pass filter"), this);
	add_act->setProperty("apu_type", QVariant("LPF"));
	connect(add_act, SIGNAL(triggered()), this, SLOT(addApuAction()) );
	menu->addAction(add_act);

	add_act = new QAction(QIcon(":/images/hpf.png"), tr("High-pass filter"), this);
	add_act->setProperty("apu_type", QVariant("HPF"));
	connect(add_act, SIGNAL(triggered()), this, SLOT(addApuAction()) );
	menu->addAction(add_act);

	add_act = new QAction(QIcon(":/images/bpf.png"), tr("Band-pass filter"), this);
	add_act->setProperty("apu_type", QVariant("BPF"));
	connect(add_act, SIGNAL(triggered()), this, SLOT(addApuAction()) );
	menu->addAction(add_act);
	
	add_act = new QAction(QIcon(":/images/peak.png"), tr("Peaking filter"), this);
	add_act->setProperty("apu_type", QVariant("peak"));
	connect(add_act, SIGNAL(triggered()), this, SLOT(addApuAction()) );
	menu->addAction(add_act);


	bool ladspa_separator_added = false;
	
	QDir effects_dir("effects/");
	foreach(QString fname, effects_dir.entryList(QDir::Files)){
		fname = effects_dir.dirName()+"/"+fname;

		int plugin_index=0;
		next_plugin_in_library:
		LADSPA_Descriptor * ladspa_desc = ApuLADSPA::getDescriptor(fname, plugin_index);
		if(!ladspa_desc) continue;

		int inputs = 0;
		int outputs = 0;

		for(int p=0; p<ladspa_desc->PortCount; p++) {
			LADSPA_PortDescriptor d = ladspa_desc->PortDescriptors[p];
			if (LADSPA_IS_PORT_AUDIO(d)) {
				if (LADSPA_IS_PORT_INPUT(d)) {
					++inputs;
				}
				else
				if (LADSPA_IS_PORT_OUTPUT(d)) {
					++outputs;
				}
			}
		}

		if(inputs>0 && outputs>0){
			if(!ladspa_separator_added){
				menu->addSeparator();
				ladspa_separator_added = true;
			}

			ladspa_plugin_data * plugin_data = new ladspa_plugin_data;
			plugin_data->desc = ladspa_desc;
			plugin_data->lib_path = fname;
			plugin_data->index = plugin_index;

			add_act = new QAction(QIcon(":/images/ladspa.png"), ladspa_desc->Name, this);
			add_act->setProperty("apu_type", QVariant("ladspa"));
			add_act->setProperty("apu_data", QVariant((int)plugin_data));
			connect(add_act, SIGNAL(triggered()), this, SLOT(addApuAction()) );
			menu->addAction(add_act);
		}
		else{
			qDebug() << fname << "skipped." << "inputs =" << inputs << "outputs =" << outputs;
		}

		++plugin_index;

		goto next_plugin_in_library;
		//qDebug() << ladspa_desc->Name << " inputs " << inputs << "outputs " << outputs;
	}


	return menu;
}
//
Apu * ApuChain::addApu(Apu * apu, QString apu_type)
{
	QFrame * apu_box = new QFrame;
	apu_box->setFrameStyle(QFrame::Panel | QFrame::Raised);
	QBoxLayout * apu_layout = new QHBoxLayout;
	apu_box->setLayout(apu_layout);
	apu_layout->addWidget(apu);
	apu_layout->setStretchFactor(apu, 1);

	
	qDebug() << apu;
	connect(apu, SIGNAL(changed()), this, SLOT(updateFinalAFC()) );
	
	//add to a apu list
	int row = m_apu_list->rowCount();
	m_apu_list->insertRow(row);
	m_apu_list->setData(m_apu_list->index(row, 0), apu->name() );
	m_apu_list->setData(m_apu_list->index(row, 1), int(apu_box) );
	m_apu_list->setData(m_apu_list->index(row, 2), int(apu) );
	
	//preview created apu
	selectApuWidget(apu_box, apu);
	m_apu_list_view->selectionModel()->select( m_apu_list->index(row, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
	
	m_apu.insert(--m_apu.end(), apu);
	m_apu_type.insert(apu, apu_type);

	updateFinalAFC();

	return apu;
}

Apu * ApuChain::addApu(QString type, void * data)
{
	Apu * apu;
	
	if(type == "LPF"){
		apu = new LowPassFilter;
	}
	else
	if(type == "HPF"){
		apu = new HighPassFilter;
	}
	else
	if(type == "BPF"){
		apu = new BandPassFilter;
	}
	else
	if(type == "peak"){
		apu = new PeakingFilter;
	}
	else
	if(type == "ladspa"){
		apu = new ApuLADSPA(*(ladspa_plugin_data*)data);
	}
	else{
		return 0;
	}
	
	return addApu(apu, type);
}

void ApuChain::addApuAction()
{
	addApu(sender()->property("apu_type").toString(), (void*)sender()->property("apu_data").toInt());
}

void ApuChain::selectApuWidget(QWidget * apubox, Apu * apu)
{
	if(m_apu_widget_preview){
		m_apu_queue_layout->removeWidget(m_apu_widget_preview);
		m_apu_widget_preview->hide();
	}

	if(!apubox){
		apubox = new QWidget(this);
		m_apu_preview =0;
	}
	
	m_apu_widget_preview = apubox;
	m_apu_queue_layout->addWidget(m_apu_widget_preview, 1);
	m_apu_widget_preview->show();
	m_apu_preview = apu;
}

void ApuChain::apuListSelected(QItemSelection sel)
{
	QWidget * apubox=0;
	Apu * apu = 0;
	
	if(!sel.empty()){
		int row = sel.first().indexes().first().row();
		apubox=(QWidget*)m_apu_list->index(row, 1).data().toInt();
		apu=(Apu*)m_apu_list->index(row, 2).data().toInt();
		
		
		selectApuWidget(apubox, apu);
		m_remove_apu_button->setDisabled(false);
		m_moveup_apu_button->setDisabled(row==0);
		m_movedown_apu_button->setDisabled(row == m_apu_list->rowCount()-1);
		m_coeffs_button->setDisabled( !(bool)dynamic_cast<LinearFilter*>(apu) );
	}
	else{
		m_remove_apu_button->setDisabled(true);
		m_moveup_apu_button->setDisabled(true);
		m_movedown_apu_button->setDisabled(true);
		m_coeffs_button->setDisabled(true);
	}
}


void ApuChain::moveUpApu()
{
	int row = m_apu_list_view->selectionModel()->selectedRows().first().row();
	m_apu_list->insertRow( row-1, m_apu_list->takeRow(row) );
	m_apu_list_view->selectionModel()->select( m_apu_list->index(row-1, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}


void ApuChain::moveDownApu()
{
	int row = m_apu_list_view->selectionModel()->selectedRows().first().row();
	m_apu_list->insertRow( row+1, m_apu_list->takeRow(row) );
	m_apu_list_view->selectionModel()->select( m_apu_list->index(row+1, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void ApuChain::removeApu()
{
	m_apu.removeOne(m_apu_preview);
	m_apu_type.remove(m_apu_widget_preview);

	int row = m_apu_list_view->selectionModel()->selectedRows().first().row();
	m_apu_list->removeRow( row );

	m_apu_list_view->selectionModel()->clear();
	selectApuWidget(0, 0);

	updateFinalAFC();
}

void ApuChain::viewFilterCoeffs()
{
	LinearFilter * lf = dynamic_cast<LinearFilter*>(m_apu_preview);

	if(!lf){
		return;
	}

	QWidget * w = new QWidget;
	QTextEdit * text = new QTextEdit;

	w->setWindowModality(Qt::WindowModal);
	w->setWindowTitle(tr("Filter Coefficients"));
		
	QBoxLayout * layout = new QVBoxLayout;
	layout->setContentsMargins(2,2,2,2);
	layout->addWidget(text);
	
	text->setReadOnly(true);


	text->append(tr("Filter Order: ")+QString::number(lf->getOrder())+"\n");

	text->append(tr("Numerator coeffs:"));
	for(int i=0; i<=lf->getOrder(); ++i){
		text->append("a"+QString::number(i)+" = "+QString::number(lf->numerCoef(i), 'g', 10));
	}

	text->append("");

	text->append(tr("Denominator coeffs:"));
	for(int i=1; i<=lf->getOrder(); ++i){
		text->append("b"+QString::number(i)+" = "+QString::number(lf->denomCoef(i), 'g', 10));
	}
	
	w->setLayout(layout);
	w->show();
}

void ApuChain::updateFinalAFC()
{
	QList<LinearFilter*> l;
	foreach(Apu * apu, m_apu){
		LinearFilter * lf = dynamic_cast<LinearFilter*>(apu);
		if(lf) l.append(lf);
	}

        
	m_final_afc->clear();
	int pts = 1000;
	for(int i=0; i<=pts; ++i){
		float freq = 19+pow(10.0, i*log10(20000-20)/pts);
		Complex val(1.0, 0);
		foreach(LinearFilter * lf, l){
			val *= lf->val(freq);
		}
		m_final_afc->addPointdB(freq, 20*log10(abs(val)) );
		m_final_afc->addPointRad(freq, arg(val) );
	}
	m_final_afc->update();
	
}

void ApuChain::processData(short * data, long num_samples, long long start_time)
{
	foreach(Apu * apu, m_apu){
		apu->processData(data, num_samples, start_time);
	}
}

void ApuChain::setFreq(long freq)
{
	m_freq = freq;

	foreach(Apu * apu, m_apu){
		apu->setFreq(freq);
	}
}

void ApuChain::reset()
{
	foreach(Apu * apu, m_apu){
		apu->reset();
	}
}

void ApuChain::clear()
{
	selectApuWidget(0, 0);

	foreach(Apu * apu, m_apu){
		if(apu == m_spectrum_in || apu == m_spectrum_out) continue;

		m_apu.removeOne(apu);
		m_apu_type.remove(apu);
	}

	m_apu_list->removeRows(0, m_apu_list->rowCount());
}

void ApuChain::saveSettings()
{
	QString fn = QFileDialog::getSaveFileName(this, "", "apu", "*.apu");
	if(fn==""){
		return;
	}

	saveSettings(fn);
}

void ApuChain::loadSettings()
{
	QString fn = QFileDialog::getOpenFileName(this, "", "apu", "*.apu");
	if(fn==""){
		return;
	}

	loadSettings(fn);
}


void ApuChain::saveSettings(QString fname)
{
	QSettings s(fname, QSettings::IniFormat);
	s.clear();

	int i=0;

	foreach(Apu * apu, m_apu){
		if(!m_apu_type.contains(apu)) continue;

		s.beginGroup(QString::number(i));
		s.setValue("type", m_apu_type[apu]);
		if(ApuLADSPA * ladspa_apu = dynamic_cast<ApuLADSPA*>(apu)){
			s.setValue("ladspa_plugin_path", ladspa_apu->getLibraryPath());
			s.setValue("ladspa_plugin_index", ladspa_apu->getPluginIndex());
		}
		apu->writeSettings(s);
		s.endGroup();

		++i;
	}
}

void ApuChain::loadSettings(QString fname)
{
	clear();

	QSettings s(fname, QSettings::IniFormat);

	foreach(QString gr, s.childGroups()){
		Apu * apu;
		s.beginGroup(gr);
		QString apu_type = s.value("type").toString();
		if(apu_type == "ladspa"){
			ladspa_plugin_data data;
			data.lib_path = s.value("ladspa_plugin_path").toString();
			data.index = s.value("ladspa_plugin_index").toInt();
			data.desc = ApuLADSPA::getDescriptor(data.lib_path, data.index);
			if(data.desc){
				apu = addApu(apu_type, &data);
			}
		}
		else{
			apu = addApu(apu_type, 0);
		}
		apu->readSettings(s);
		s.endGroup();
	}
}
