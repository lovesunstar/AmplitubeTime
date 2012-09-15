#include "time_ruler.h"
//
TimeRuler::TimeRuler( QWidget * parent ) 
	: Track(parent)
{
	setMinimumHeight(25);
	setMaximumHeight(25);

	m_marker_num = 0;
}
//
TimeRuler::~TimeRuler()
{
}
//

void TimeRuler::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, false);

	qDrawShadePanel(&painter, QRect(1, 1, width()-2, height()-2), QPalette() );

	updateMarkers();

	for(int i=0; i<m_marker_num; ++i){
		QDateTime time = QDateTime::fromTime_t(0);
		time=time.addMSecs( (long long) ((m_marker_first+i*m_marker_step)*1000.0) );

		double pos = width()*(m_marker_first+i*m_marker_step-m_begin)/m_range;
		QLineF line( pos, height()-5, pos, height());
		painter.drawLine(line);

		QString lbl;
		if(m_marker_step<86400.0 && time.time().hour()==0 && time.time().minute()==0 && time.time().second()==0 && time.time().msec()==0){
			lbl=time.toString("dd.MM.yyyy");
			QFont font;
			font.setBold(true);
			painter.setFont(font);
		}
		else{
			lbl=time.toString(m_marker_text_format);
			painter.setFont(QFont());
		}
		
		QRectF rect(pos-100.0, 0, 200.0, height()-5);
		painter.drawText(rect, Qt::AlignCenter, lbl);
	}

	painter.drawRect(0, 0, width()-1, height()-1);
}

void TimeRuler::updateMarkers()
{
	if(m_range < 0.1){m_marker_step = 0.0125;}
	else
	if(m_range < 0.3){m_marker_step = 0.05;}
	else
	if(m_range < 0.6){m_marker_step = 0.1;}
	else
	if(m_range < 1.0){m_marker_step = 0.2;}
	else
	if(m_range < 2.0){m_marker_step = 0.25;}
	else
	if(m_range < 4.0){m_marker_step = 0.5;}
	else
	if(m_range < 8.0){m_marker_step = 1.0;}
	else
	if(m_range < 15.0){m_marker_step = 2.0;}
	else
	if(m_range < 25.0){m_marker_step = 5.0;}
	else
	if(m_range < 50.0){m_marker_step = 10.0;}
	else
	if(m_range < 100.0){m_marker_step = 20.0;}
	else
	if(m_range < 150.0){m_marker_step = 30.0;}
	else
	if(m_range < 200.0){m_marker_step = 60.0;}
	else
	if(m_range < 400.0){m_marker_step = 120.0;}
	else
	if(m_range < 600.0){m_marker_step = 300.0;}
	else
	if(m_range < 1500.0){m_marker_step = 600.0;}
	else
	if(m_range < 3000.0){m_marker_step = 1200.0;}
	else
	if(m_range < 5000.0){m_marker_step = 1800.0;}
	else
	if(m_range < 10000.0){m_marker_step = 3600.0;}
	else
	if(m_range < 20000.0){m_marker_step = 7200.0;}
	else
	if(m_range < 40000.0){m_marker_step = 14400.0;}
	else
	if(m_range < 500000.0){m_marker_step = 86400.0;}
	else{m_marker_step = 604800.0;}

	long last_marker = (long)trunc((m_begin+m_range)/m_marker_step);
	long first_marker = (long)trunc(m_begin/m_marker_step)+1;
	m_marker_first = first_marker*m_marker_step;
	m_marker_num = last_marker-first_marker+1;

	if(m_marker_step<1.0){
		m_marker_text_format="s.zzz";
	}
	else
	if(m_marker_step<60.0){
		m_marker_text_format="hh:mm:ss";
	}
	else
	if(m_marker_step<86400.0){
		m_marker_text_format="hh:mm";
	}
	else{
		m_marker_text_format="dd.MM.yyyy";
	}
}
