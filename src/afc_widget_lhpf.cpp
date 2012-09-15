#include "afc_widget_lhpf.h"
#include <cmath>

using namespace std;
//
AFCWidgetLowHigh::AFCWidgetLowHigh(QWidget *parent) 
	: AFCWidget(parent)
{
	m_mouse_pressed = false;

	m_cutoff = 0;
}
//
AFCWidgetLowHigh::~AFCWidgetLowHigh()
{
}
//


void AFCWidgetLowHigh::timerEvent(QTimerEvent *event)
{
}

void AFCWidgetLowHigh::updateParameters(int x, int y)
{
	if(m_mouse_pressed && m_view_mode == MagnitudeView){
		m_cutoff = linFreq(x);
//		m_cutoff = m_freq1+pow(10.0, float(x-m_left_scale_size)/(width()-m_left_scale_size)*(m_freq2-m_freq1)*log10(m_freq2-m_freq1)/m_freq2);

		if(m_cutoff>m_freq1 && m_cutoff<m_freq2){
			emit parametersChanged(m_cutoff);
		}
	}
}

void AFCWidgetLowHigh::mousePressEvent(QMouseEvent *event)
{
	if(event->button()!=Qt::RightButton){
		m_mouse_pressed = true;
		updateParameters(event->x(), event->y() );
	}
}

void AFCWidgetLowHigh::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouse_pressed = false;

	if(event->button()==Qt::RightButton){
		emit contextMenuRequested(mapToGlobal(QPoint(event->x(), event->y())));
	}
	else{
		updateParameters(event->x(), event->y() );
	}
}

void AFCWidgetLowHigh::mouseMoveEvent(QMouseEvent *event)
{
	updateParameters(event->x(), event->y() );
}

void AFCWidgetLowHigh::drawSpecificDetails(QPainter & painter, QRectF rect)
{
	QPen grey_dashed_pen(QColor(0,0,0, 64));
//	grey_dashed_pen.setStyle(Qt::DashLine);
	painter.setPen(grey_dashed_pen);

	int dB_range = m_dB2-m_dB1;
	float rad_range = m_rad2-m_rad1;


	if(m_view_mode == MagnitudeView){
		float x = rect.left()+logFreq(m_cutoff)*rect.width();
		painter.drawLine(x, rect.top(), x, rect.bottom());
		painter.drawLine(rect.left(), rect.top()+(m_dB2+3)*rect.height()/dB_range, rect.right(), rect.top()+(m_dB2+3)*rect.height()/dB_range);
	
		painter.setPen( QPen(QColor(0, 0, 0, 255)) );
	
		float h = (height()-m_bottom_scale_size)*(m_dB2+3)/dB_range;
		QRectF text_rect = QRectF(0, h-50, m_left_scale_size-2, 100);
		painter.drawText( text_rect, Qt::AlignRight | Qt::AlignVCenter, "-3 dB" );
	}
	else
	if(m_view_mode == PhaseView){
		painter.drawLine(rect.left(), rect.top()+(m_rad2)*rect.height()/rad_range, rect.right(), rect.top()+(m_rad2)*rect.height()/rad_range);
	}
}
