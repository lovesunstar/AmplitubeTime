#include "source_edit_dialog.h"
//
SourceEditDialog::SourceEditDialog( QWidget *parent ):
	QDialog(parent) 
{
	setWindowModality(Qt::ApplicationModal);
	
	QBoxLayout *main_layout=new QVBoxLayout(this);
	setLayout(main_layout);
	
	QGridLayout *controls_layout=new QGridLayout;
	main_layout->addLayout(controls_layout);
	controls_layout->addWidget(new QLabel(tr("Location:")), 0, 0);
	controls_layout->addWidget(new QLabel(tr("Frequency:")), 1, 0);
	controls_layout->addWidget(new QLabel(tr("Sample size:")), 2, 0);
	controls_layout->addWidget(new QLabel(tr("Sample format:")), 3, 0);
	controls_layout->addWidget(new QLabel(tr("Swap samples order:")), 4, 0);
	m_filename_edit=new QLineEdit;
	
	m_freq_combo=new QComboBox;
	m_freq_combo->addItem("8000 Hz");
	m_freq_map[m_freq_combo->count()-1]=8000;
	m_freq_combo->addItem("11025 Hz");
	m_freq_map[m_freq_combo->count()-1]=11025;
	m_freq_combo->addItem("16000 Hz");
	m_freq_map[m_freq_combo->count()-1]=16000;
	m_freq_combo->addItem("22050 Hz");
	m_freq_map[m_freq_combo->count()-1]=22050;
	m_freq_combo->addItem("32000 Hz");
	m_freq_map[m_freq_combo->count()-1]=32000;
	m_freq_combo->addItem("44100 Hz");
	m_freq_map[m_freq_combo->count()-1]=44100;
	m_freq_combo->addItem("48000 Hz");
	m_freq_map[m_freq_combo->count()-1]=48000;
	m_freq_combo->addItem("88200 Hz");
	m_freq_map[m_freq_combo->count()-1]=88200;
	
	m_format_combo=new QComboBox;
	m_format_combo->addItem("8 Bit");
	m_format_map[m_format_combo->count()-1]=QPair<int, int>(1, 8);
	m_format_combo->addItem("16 Bit");
	m_format_map[m_format_combo->count()-1]=QPair<int, int>(1, 16);
	
	m_format_signed_combo = new QComboBox;
	m_format_signed_combo->addItem("Unsigned");
	m_format_signed_combo->addItem("Signed");
	
	m_swap_samples_order_combo = new QComboBox;
	m_swap_samples_order_combo->addItem("False");
	m_swap_samples_order_combo->addItem("True");
	
	//m_format_combo->addItem("Mono 24 Bit");
	//m_format_map[m_format_combo->count()-1]=QPair<int, int>(1, 24);
	//m_format_combo->addItem("Mono 32 Bit");
	//m_format_map[m_format_combo->count()-1]=QPair<int, int>(1, 32);
	
	//m_format_combo->addItem("Stereo 8 Bit");
	//m_format_map[m_format_combo->count()-1]=QPair<int, int>(2, 8);
	//m_format_combo->addItem("Stereo 16 Bit");
	//m_format_map[m_format_combo->count()-1]=QPair<int, int>(2, 16);
	
	//QFileDialog * file_dialog = new QFileDialog;
	
	QAction * fileopen_action = new QAction (QIcon(":images/fileopen.png"), tr("Select location"), this);
	QToolButton * m_filename_select_button = new QToolButton;
	m_filename_select_button->setAutoRaise(true); 
	m_filename_select_button->setDefaultAction(fileopen_action);
	
	connect(fileopen_action, SIGNAL(triggered()), this, SLOT(selectLocation()) );
	
	QBoxLayout * m_filename_edit_layout = new QHBoxLayout;
	m_filename_edit_layout->addWidget(m_filename_edit);
	m_filename_edit_layout->addWidget(m_filename_select_button);
	
	
	controls_layout->addLayout(m_filename_edit_layout, 0, 1);
	controls_layout->addWidget(m_freq_combo, 1, 1);
	controls_layout->addWidget(m_format_combo, 2, 1);
	controls_layout->addWidget(m_format_signed_combo, 3, 1);
	controls_layout->addWidget(m_swap_samples_order_combo, 4, 1);
	
	QBoxLayout *buttons_layout=new QHBoxLayout;
	buttons_layout->setContentsMargins(0,11,0,0);
	main_layout->addLayout(buttons_layout);
	QPushButton *ok_button=new QPushButton(QIcon(":/images/ok.png"), tr("Ok"));
	QPushButton *cancel_button=new QPushButton(QIcon(":/images/no.png"), tr("Cancel"));
	connect(ok_button, SIGNAL(clicked()), this, SLOT(onOk()));
	connect(cancel_button, SIGNAL(clicked()), this, SLOT(hide()));
	buttons_layout->addStretch(1);
	buttons_layout->addWidget(ok_button);
	buttons_layout->addWidget(cancel_button);
}
//
void SourceEditDialog::selectLocation()
{
	QString path = QFileDialog::getExistingDirectory();
	if(path!=""){
		m_filename_edit->setText( path );
	}
}
//
void SourceEditDialog::newSource()
{
	m_au=0;
	m_filename_edit->clear();
	setWindowTitle(tr("Create new source"));
	show();
}

void SourceEditDialog::editSource(AudioSource *au)
{
	m_au=au;
	m_filename_edit->setText(m_au->getDirectory());
	m_freq_combo->setCurrentIndex( m_freq_map.key(au->getFreq()) );
	m_format_combo->setCurrentIndex( m_format_map.key(QPair<int, int>(1, au->getBytesInSample()*8)) );
	m_format_signed_combo->setCurrentIndex( au->getFormatSigned() );
	setWindowTitle(tr("Modifying source"));
	show();
}

void SourceEditDialog::onOk()
{
	QString filename = m_filename_edit->text();
	int bits_in_sample = m_format_map[m_format_combo->currentIndex()].second;
	int freq = m_freq_map[m_freq_combo->currentIndex()];
	bool format_signed = m_format_signed_combo->currentIndex();
	bool swap_samples_order = m_swap_samples_order_combo->currentIndex();
	
	if(m_au){
		m_au->setDirectory(filename);
		m_au->setFreq(freq);
		m_au->setBytesInSample(bits_in_sample/8);
		m_au->setFormatSigned(format_signed);
		m_au->setSwapSamplesOrder(swap_samples_order);
		emit sourceModified(m_au);
	}
	else{
		m_au=new AudioSource(filename, bits_in_sample/8, freq, format_signed);
		m_au->setSwapSamplesOrder(swap_samples_order);
		emit sourceCreated(m_au);
	}
	
	hide();
}

