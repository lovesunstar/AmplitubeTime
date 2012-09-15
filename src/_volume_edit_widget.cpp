#include "volume_edit_widget.h"
//
VolumeEditWidget::VolumeEditWidget(QWidget *parent) 
	: QFrame(parent)
{
	m_slider = new QSlider();
	m_slider->setRange(0, 100);
	m_slider->setValue(100);
	connect( m_slider, SIGNAL(valueChanged(int)), this, SLOT(valueChange(int)) );
	
	setFrameStyle(QFrame::Panel | QFrame::Raised);
	
	QBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_slider);
	setLayout(layout);
	
	setWindowFlags(Qt::ToolTip /*| Qt::FramelessWindowHint*/);
	
	startTimer(10);
}
//

void VolumeEditWidget::popup()
{
	show();
	//grabKeyboard();
	move( QCursor::pos().x() - width()/2, QCursor::pos().y()-width()/2 );
}

void  VolumeEditWidget::timerEvent(QTimerEvent *event)
{
	int bound = 3;
	QPoint pos = QCursor::pos();
	if(x()-bound>pos.x() || y()-bound>pos.y() || x()+width()+bound<pos.x() || y()+height()+bound<pos.y()){
		//ungrabKeyboard();
		hide();
	}
}

void  VolumeEditWidget::keyPressEvent ( QKeyEvent * event )
{
	if(event->key() == Qt::Key_Escape){
		//ungrabKeyboard();
		hide();
	}
}

void  VolumeEditWidget::valueChange(int value)
{
	emit volumeChanged(value/100.0);
}

