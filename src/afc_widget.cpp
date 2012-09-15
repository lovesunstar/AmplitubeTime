#include "afc_widget.h"
#include <cmath>

using namespace std;
//
AFCWidget::AFCWidget(QWidget *parent) 
	: QWidget(parent)
{
	m_left_scale_size = 35;
	m_bottom_scale_size = 10;

	setdBRange(-40, 40);
	setRadRange(-M_PI, M_PI);
	setFreqRange(20, 20000);

	m_freq_labels.append(30);
	m_freq_labels.append(100);
	m_freq_labels.append(300);
	m_freq_labels.append(1000);
	m_freq_labels.append(3000);
	m_freq_labels.append(10000);

	m_show_info = false;

	QAction * m_act_magnitude = new QAction(QIcon(""), tr("Magnitude"), this);
	QAction * m_act_phase = new QAction(QIcon(""), tr("Phase"), this);
	QAction * m_act_phase_magnitude = new QAction(QIcon(""), tr("Magnitude+Phase"), this);

	m_act_magnitude->setCheckable(true);
	m_act_phase->setCheckable(true);
	m_act_phase_magnitude->setCheckable(true);

	QActionGroup * act_gr = new QActionGroup(this);
	act_gr->addAction(m_act_magnitude);
	act_gr->addAction(m_act_phase);
	act_gr->addAction(m_act_phase_magnitude);

	connect(m_act_magnitude, SIGNAL(triggered()), this, SLOT(viewMagnitude()) );
	connect(m_act_phase, SIGNAL(triggered()), this, SLOT(viewPhase()) );
	connect(m_act_phase_magnitude, SIGNAL(triggered()), this, SLOT(viewPhaseMagnitude()) );

	m_context_menu = new QMenu;
	m_context_menu->addAction(m_act_magnitude);
	m_context_menu->addAction(m_act_phase);
	//m_context_menu->addAction(m_act_phase_magnitude);

	m_act_magnitude->setChecked(true);
	viewMagnitude();

	connect(this, SIGNAL(contextMenuRequested(QPoint)), this, SLOT(contextMenuPopup(QPoint)) );

	setMinimumHeight(100);
}

AFCWidget::~AFCWidget()
{
}
//

void AFCWidget::viewPhase()
{
	m_view_mode = PhaseView;
	update();
}
void AFCWidget::viewMagnitude()
{
	m_view_mode = MagnitudeView;
	update();
}
void AFCWidget::viewPhaseMagnitude()
{
	m_view_mode = PhaseMagnitudeView;
	update();
}



void AFCWidget::contextMenuPopup(QPoint pos)
{
	m_context_menu->popup(pos);
}



void AFCWidget::timerEvent(QTimerEvent *event)
{
}

void AFCWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->button()!=Qt::RightButton){
		m_show_info = true;
		m_mouse_x = event->x();
		m_mouse_y = event->y();
		update();
	}
}

void AFCWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button()==Qt::RightButton){
		emit contextMenuRequested(mapToGlobal(QPoint(event->x(), event->y())));
	}

	m_show_info = false;
	update();
}

void AFCWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(m_mouse_x != event->x() || m_mouse_y || event->y()){
		m_mouse_x = event->x();
		m_mouse_y = event->y();
		update();
	}
}

void AFCWidget::clear()
{
	m_dB_points.clear();
	m_rad_points.clear();
}

void AFCWidget::addPointdB(float x, float y)
{
	m_dB_points.append(QPointF(x, y));
}

void AFCWidget::addPointRad(float x, float y)
{
	m_rad_points.append(QPointF(x, y));
}

void AFCWidget::setdBRange(int dB1, int dB2)
{
	m_dB1 = dB1>dB2?dB2:dB1;
	m_dB2 = dB1>dB2?dB1:dB2;

	update();
}

void AFCWidget::setRadRange(float rad1, float rad2)
{
	m_rad1 = rad1>rad2?rad2:rad1;
	m_rad2 = rad1>rad2?rad1:rad2;

	update();
}

void AFCWidget::setFreqRange(int freq1, int freq2)
{
	m_freq1 = freq1>freq2?freq2:freq1;
	m_freq2 = freq1>freq2?freq1:freq2;

	m_log_freq1 = log10((float)m_freq1);
	m_log_freq2 = log10((float)m_freq2);

	update();
}

float AFCWidget::logFreq(float freq)
{
	return (log10(freq)-m_log_freq1)/(m_log_freq2-m_log_freq1);
}
float AFCWidget::linFreq(float x)
{
	return pow(10.0f, m_log_freq1+(x-m_left_scale_size)/(width()-m_left_scale_size)*(m_log_freq2-m_log_freq1));
}

void AFCWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, false);
	
	QRect rect(m_left_scale_size, 0, width()-m_left_scale_size-1, height()-m_bottom_scale_size-1);
	
	QFont font;
	font.setPointSize(7);
	painter.setFont(font);
	
	if(m_view_mode == MagnitudeView){
		drawAmplitude(painter, rect);
	}
	else
	if(m_view_mode == PhaseView){
		drawPhase(painter, rect);
	}
	else{
		drawAmplitude(painter, rect);
		drawPhase(painter, rect);
	}

	drawScales(painter);
	drawSpecificDetails(painter, rect);
	
	//painter.setPen( QPen(QColor(0, 0, 0, 255)) );
	//QRectF text_rect(4, 4, width()-8, height()-8);
	//painter.drawText( text_rect, Qt::AlignRight | Qt::AlignTop, QString::number(m_fft_size)+" pts" );
	
	painter.setPen( QPen(QColor(0, 0, 0, 255)) );
	painter.drawRect(rect);
	
}

void AFCWidget::drawAmplitude(QPainter & painter, QRectF rect)
{
	if(m_dB_points.size()<2) return;

	painter.setClipping(true);
	painter.setClipRect(rect);
	painter.setRenderHint(QPainter::Antialiasing, true);
	

	//amplitude
	painter.setPen( QPen(QColor(0, 0, 0, 255)) );

	float prev_x=m_dB_points.first().x();
	float prev_y=m_dB_points.first().y();

	float dB_range = m_dB2-m_dB1;

	foreach(QPointF p, m_dB_points){
//		QPointF p1( rect.left()+log_coef*log10(prev_x-m_freq1)/freq_range*rect.width(), rect.bottom()-(prev_y-m_dB1)/dB_range*rect.height() );
//		QPointF p2( rect.left()+log_coef*log10(p.rx()-m_freq1)/freq_range*rect.width(), rect.bottom()-(p.ry()-m_dB1)/dB_range*rect.height() );

		//linear scale
//		QPointF p1( rect.left()+(prev_x-m_freq1)/(m_freq2-m_freq1)*rect.width(), rect.bottom()-(prev_y-m_dB1)/dB_range*rect.height() );
//		QPointF p2( rect.left()+(p.rx()-m_freq1)/(m_freq2-m_freq1)*rect.width(), rect.bottom()-(p.ry()-m_dB1)/dB_range*rect.height() );

		//log scale
		QPointF p1( rect.left()+logFreq(prev_x)*rect.width(), rect.bottom()-(prev_y-m_dB1)/dB_range*rect.height() );
		QPointF p2( rect.left()+logFreq(p.rx())*rect.width(), rect.bottom()-(p.ry()-m_dB1)/dB_range*rect.height() );


		if(p.x() <= m_freq2) painter.drawLine(p1, p2);
		prev_x = p.rx();
		prev_y = p.ry();
	}
	

	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setClipping(false);
}

void AFCWidget::drawPhase(QPainter & painter, QRectF rect)
{
	if(m_rad_points.size()<2) return;

	painter.setClipping(true);
	painter.setClipRect(rect);
	painter.setRenderHint(QPainter::Antialiasing, true);
	

	//phase
	painter.setPen( QPen(QColor(0, 0, 180, 255)) );

	float prev_x=m_rad_points.first().x();
	float prev_y=m_rad_points.first().y();

	float rad_range = m_rad2-m_rad1;
	float freq_range = m_freq2-m_freq1;

//	float log_coef = m_freq2/log10(freq_range);

	foreach(QPointF p, m_rad_points){
//		QPointF p1( rect.left()+log_coef*log10(prev_x-m_freq1)/freq_range*rect.width(), rect.bottom()-(prev_y-m_rad1)/rad_range*rect.height() );
//		QPointF p2( rect.left()+log_coef*log10(p.rx()-m_freq1)/freq_range*rect.width(), rect.bottom()-(p.ry()-m_rad1)/rad_range*rect.height() );

		QPointF p1( rect.left()+logFreq(prev_x)*rect.width(), rect.bottom()-(prev_y-m_rad1)/rad_range*rect.height() );
		QPointF p2( rect.left()+logFreq(p.rx())*rect.width(), rect.bottom()-(p.ry()-m_rad1)/rad_range*rect.height() );

		painter.drawLine(p1, p2);
		prev_x = p.rx();
		prev_y = p.ry();
	}

	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setClipping(false);
}

void AFCWidget::drawScales(QPainter & painter)
{
	painter.setPen( QPen(QColor(0, 0, 0, 255)) );
	QRectF text_rect;

	float dB_range = m_dB2-m_dB1;
	float rad_range = m_rad2-m_rad1;
	float freq_range = m_freq2-m_freq1;

//	float log_coef = m_freq2/log10(m_freq2);//log10(freq_range);

	if(m_view_mode == MagnitudeView){
		if(m_dB2>0){
			float h = (height()-m_bottom_scale_size)*m_dB2/dB_range;
			text_rect = QRectF(0, h-50, m_left_scale_size-2, 100);
			painter.drawText( text_rect, Qt::AlignRight | Qt::AlignVCenter, "0 dB" );
		}
	
		text_rect = QRectF(0, 0, m_left_scale_size-2, 100);
		painter.drawText( text_rect, Qt::AlignRight | Qt::AlignTop, QString::number(m_dB2)+" dB" );
	
		text_rect = QRectF(0, 0, m_left_scale_size-2, height()-m_bottom_scale_size);
		painter.drawText( text_rect, Qt::AlignRight | Qt::AlignBottom, QString::number(m_dB1)+" dB" );
	}
	else
	if(m_view_mode == PhaseView){
		text_rect = QRectF(0, (height()-m_bottom_scale_size)*m_rad2/rad_range-50, m_left_scale_size-2, 100);
		painter.drawText( text_rect, Qt::AlignRight | Qt::AlignVCenter, "0" );

		text_rect = QRectF(0, 0, m_left_scale_size-2, 100);
		painter.drawText( text_rect, Qt::AlignRight | Qt::AlignTop, QString::number(m_rad2/M_PI*180)+"" );
	
		text_rect = QRectF(0, 0, m_left_scale_size-2, height()-m_bottom_scale_size);
		painter.drawText( text_rect, Qt::AlignRight | Qt::AlignBottom, QString::number(m_rad1/M_PI*180)+"" );
	}


	text_rect = QRectF(m_left_scale_size, height()-m_bottom_scale_size, 100, 100);
	painter.drawText( text_rect, Qt::AlignLeft | Qt::AlignTop, QString::number(m_freq1) );
	
	text_rect = QRectF(0, height()-m_bottom_scale_size, width(), 100);
	painter.drawText( text_rect, Qt::AlignRight | Qt::AlignTop, QString::number(m_freq2/1000.0)+"k" );

	if(m_show_info){
		text_rect = QRectF(m_left_scale_size, height()-m_bottom_scale_size, width()-m_left_scale_size, 100);
		//float x = float(freq_range)*(m_mouse_x-m_left_scale_size)/(width()-m_left_scale_size);
		//float freq = pow(10.0, x*log10(m_freq2-m_freq1)/m_freq2);
		//float freq = linFreq(m_mouse_x);
		
		QString text = QString::number(linFreq(m_mouse_x))+" Hz, ";
		
		if(m_view_mode == MagnitudeView){
			float dBgain = m_dB1-int(float(m_dB2-m_dB1)*m_mouse_y/(height()-m_bottom_scale_size))+(m_dB2-m_dB1);
			text += QString::number(double(dBgain),'g',2)+" dB";
		}
		else
		if(m_view_mode == PhaseView){
			float rad = m_rad1-float(m_rad2-m_rad1)*m_mouse_y/(height()-m_bottom_scale_size)+(m_rad2-m_rad1);
			text += "phase " + QString::number(double(rad*180/M_PI),'g');
		}

		painter.drawText( text_rect, Qt::AlignHCenter | Qt::AlignTop, text );
	}

	QPen grey_dashed_pen(QColor(0,0,0, 64));
	grey_dashed_pen.setStyle(Qt::DashLine);

	foreach(int freq, m_freq_labels){
		QString lbl;
		if(freq<1000){
			lbl = QString::number(freq);
		}
		else{
			lbl = QString::number(freq/1000) + "k";
		}

		//float x = m_left_scale_size+(log10(freq)-log10(m_freq1))/(log10(m_freq2)-log10(m_freq1))*(width()-m_left_scale_size);
		float x = m_left_scale_size + logFreq(freq)*(width()-m_left_scale_size);

		if(!m_show_info){
			text_rect = QRectF(x-100, height()-m_bottom_scale_size, 200, 100);
			painter.setPen( QPen(QColor(0, 0, 0, 255)) );
			painter.drawText( text_rect, Qt::AlignHCenter | Qt::AlignTop, lbl );
		}

		painter.setPen( grey_dashed_pen );
		painter.drawLine(x, 0, x, height()-m_bottom_scale_size);
	}
}

void AFCWidget::drawSpecificDetails(QPainter & painter, QRectF rect)
{
	QPen grey_dashed_pen(QColor(0,0,0, 64));
	grey_dashed_pen.setStyle(Qt::DashLine);
	painter.setPen(grey_dashed_pen);

	int dB_range = m_dB2-m_dB1;	
	float rad_range = m_rad2-m_rad1;

	if(m_view_mode == MagnitudeView){
		painter.drawLine(rect.left(), rect.top()+(m_dB2)*rect.height()/dB_range, rect.right(), rect.top()+(m_dB2)*rect.height()/dB_range);
	}
	else
	if(m_view_mode == PhaseView){
		painter.drawLine(rect.left(), rect.top()+(m_rad2)*rect.height()/rad_range, rect.right(), rect.top()+(m_rad2)*rect.height()/rad_range);
	}
}
