#include "afc_widget_peak.h"
#include <cmath>

using namespace std;
//
AFCWidgetPeak::AFCWidgetPeak(QWidget *parent) 
	: AFCWidget(parent)
{
	m_mouse_pressed = false;
	m_f0=0;
}
//
AFCWidgetPeak::~AFCWidgetPeak()
{
}
//


void AFCWidgetPeak::timerEvent(QTimerEvent *event)
{
}

void AFCWidgetPeak::updateParameters(int x, int y)
{
	if(m_mouse_pressed && m_view_mode == MagnitudeView){
		int dBgain = m_dB1-int(float(m_dB2-m_dB1)*y/(height()-m_bottom_scale_size))+(m_dB2-m_dB1);
		m_f0 = linFreq(x);
//		m_f0 = m_freq1+pow(10.0, float(x-m_left_scale_size)/(width()-m_left_scale_size)*(m_freq2-m_freq1)*log10(m_freq2-m_freq1)/m_freq2);

		if(m_f0>m_freq1 && m_f0<m_freq2 && dBgain>=m_dB1 && dBgain<=m_dB2){
//			qDebug() << dBgain;
//			qDebug() << f0;
//			qDebug() << "\n";
			emit parametersChanged(dBgain, m_f0);
		}
	}
}

void AFCWidgetPeak::mousePressEvent(QMouseEvent *event)
{
	if(event->button()!=Qt::RightButton){
		m_mouse_pressed = true;
		updateParameters(event->x(), event->y() );
	}
}

void AFCWidgetPeak::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouse_pressed = false;
	
	if(event->button()==Qt::RightButton){
		emit contextMenuRequested(mapToGlobal(QPoint(event->x(), event->y())));
	}
	else{
		updateParameters(event->x(), event->y() );
	}
}

void AFCWidgetPeak::mouseMoveEvent(QMouseEvent *event)
{
	updateParameters(event->x(), event->y() );
}

void AFCWidgetPeak::drawSpecificDetails(QPainter & painter, QRectF rect)
{
	QPen grey_dashed_pen(QColor(0,0,0, 64));
//	grey_dashed_pen.setStyle(Qt::DashLine);
	painter.setPen(grey_dashed_pen);

        float rad_range = m_rad2-m_rad1;

	if(m_view_mode == MagnitudeView){
		float x = rect.left()+logFreq(m_f0)*rect.width();
		painter.drawLine(x, rect.top(), x, rect.bottom());
	}
	else
	if(m_view_mode == PhaseView){
		painter.drawLine(rect.left(), rect.top()+(m_rad2)*rect.height()/rad_range, rect.right(), rect.top()+(m_rad2)*rect.height()/rad_range);
	}
}
