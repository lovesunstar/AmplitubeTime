#include "sourceslist.h"
//
SourcesList::SourcesList(  ) 
	: QWidget()
{
	layout = new QVBoxLayout;
	layout->addStretch(1);
	setLayout(layout);
	layout->setContentsMargins(4,4,4,4);
}
//
SourcesList::~SourcesList()
{
}
//

void SourcesList::addSource(QString name, int freq, int sample_size)
{/*
	QGroupBox *wid = new QGroupBox(name);
	
	QPushButton *button;
	button = new QPushButton(name);
	button->setCheckable(true);
	
	QBoxLayout *layout1 = new QHBoxLayout;
	layout1->setContentsMargins(0,0,0,0);
	//layout1->addWidget(button);
	layout1->addWidget(new QLabel(QString::number(freq)+" Hz "+QString::number(sample_size*8)+" Bits") );
	
	ATCWidget *atc = new ATCWidget;
	atc->setSecondLength(30);
	atc->setDensity(0.02);
	atc->setScrollbarSize(0);
	atc->setTimingbarSize(0);
	atc->setFrequency(44100);
	QFile file("sound.wav");
	file.open(QIODevice::ReadOnly);
	atc->addBuffer(file.readAll(), sample_size);
	atc->setMinimumHeight(25);
	atc->setShowCorners(false);
	atc->setAutoscroll(true);
	
	atc_list.push_back(atc);
	
	connect(atc, SIGNAL(scaleChanged(float)), SLOT(setScale(float)));
	
	QBoxLayout *main_layout = new QVBoxLayout;
	main_layout->setContentsMargins(3,3,3,3);
	wid->setLayout(main_layout);
	main_layout->addLayout(layout1);
	main_layout->addWidget(atc);
	
	layout->insertWidget(layout->count()-1, wid);
	//layout->insertWidget(layout->count()-1, new );*/
}

void SourcesList::setScale(float scale_)
{
	foreach(ATCWidget* atc, atc_list){
		atc->setScale(scale_);
	}
}
