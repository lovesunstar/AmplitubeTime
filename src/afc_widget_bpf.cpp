#include "afc_widget_bpf.h"
#include <cmath>

using namespace std;
//
AFCWidgetBandPass::AFCWidgetBandPass(QWidget *parent) 
	: AFCWidget(parent)
{
	m_mouse_pressed = false;

	m_cutoff = 0;
}
//
AFCWidgetBandPass::~AFCWidgetBandPass()
{
}
//


void AFCWidgetBandPass::timerEvent(QTimerEvent *event)
{
}

void AFCWidgetBandPass::updateParameters(int x, int y)
{
	if(m_mouse_pressed && m_view_mode == MagnitudeView){
		m_cutoff = linFreq(x);//m_freq1+pow(10.0, float(x-m_left_scale_size)/(width()-m_left_scale_size)*(m_freq2-m_freq1)*log10(m_freq2-m_freq1)/m_freq2);

		if(m_cutoff>m_freq1 && m_cutoff<m_freq2){
			emit parametersChanged(m_cutoff);
		}
	}
}

void AFCWidgetBandPass::mousePressEvent(QMouseEvent *event)
{
	if(event->button()!=Qt::RightButton){
		m_mouse_pressed = true;
		updateParameters(event->x(), event->y() );
	}
}

void AFCWidgetBandPass::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouse_pressed = false;

	if(event->button()==Qt::RightButton){
		emit contextMenuRequested(mapToGlobal(QPoint(event->x(), event->y())));
	}
	else{
		updateParameters(event->x(), event->y() );
	}
}

void AFCWidgetBandPass::mouseMoveEvent(QMouseEvent *event)
{
	updateParameters(event->x(), event->y() );
}

void AFCWidgetBandPass::drawSpecificDetails(QPainter & painter, QRectF rect)
{
	QPen grey_dashed_pen(QColor(0,0,0, 64));
//	grey_dashed_pen.setStyle(Qt::DashLine);
	painter.setPen(grey_dashed_pen);

	float rad_range = m_rad2-m_rad1;


	if(m_view_mode == MagnitudeView){
		float x = rect.left()+logFreq(m_cutoff)*rect.width();
		painter.drawLine(x, rect.top(), x, rect.bottom());
	}
	else
	if(m_view_mode == PhaseView){
		painter.drawLine(rect.left(), rect.top()+(m_rad2)*rect.height()/rad_range, rect.right(), rect.top()+(m_rad2)*rect.height()/rad_range);
	}
}
