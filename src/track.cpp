#include "track.h"
//
Track::Track( QWidget * parent ) 
	: QWidget(parent)
{
	m_action = NoAction;
	m_selection_mode = RangeSelectionMode;
	m_scaling_pivot = LeftSidePivot;
}
//
Track::~Track()
{
}
//
void Track::setRange(double range)
{
	if(m_range != range){
		m_range = range;
		update();
	}
}

void Track::setBegin(double begin)
{
	if(m_begin != begin){
		m_begin = begin;
		update();
	}
}

void Track::setEnd(double end)
{
	setBegin(end-m_range);
}

//
void Track::mousePressEvent(QMouseEvent *event)
{
	m_action_start_x = event->x();
	
//	if(event->button() == Qt::LeftButton){
//		m_action=SelectingAction;
//	}
	
//	if(event->button() == Qt::RightButton){
//		m_action=ScalingAction;
//		start_scale = m_scale;
//		setCursor(Qt::SizeHorCursor);
//	}
	
	if(event->button() == Qt::MidButton){
		m_action=ShiftingAction;
		m_action_start_offset = m_begin;
		setCursor(Qt::OpenHandCursor);
	}
	
	mouseMoveEvent(event);
	m_action_moved = false;
}

inline int sign(long double val){
	if(val>0){
		return 1;
	}
	if(val<0){
		return -1;
	}
	
	return 0;
}

void Track::wheelEvent(QWheelEvent *event)
{
/*
	if(qApp->keyboardModifiers() == Qt::ControlModifier){
		float koef = 0.75*event->delta()/120.0;
		
		
		if(koef < 0){
			koef = -1/koef;
		}
		
		float prev_scale = m_scale;
		
		setScale(m_scale * koef);
		
		if(m_scaling_pivot == SelectionPivot && selection_begin>0){
			setOffset( m_offset + (1 - prev_scale/m_scale)*(selection_begin - m_offset) );
		}
	}
*/
	
	if(qApp->keyboardModifiers() == Qt::NoModifier){
		double step = m_range/10.0;
		//step = getCurrentStep();
		setBegin(m_begin-step*event->delta()/120.0);
	}
	
}

void Track::mouseReleaseEvent(QMouseEvent *event)
{
	if(!m_action_moved && event->button()==Qt::RightButton){
		emit contextMenuRequested(mapToGlobal(QPoint(event->x(), event->y())));
	}
	m_action=NoAction;
	setCursor(Qt::ArrowCursor);
}

void Track::mouseMoveEvent(QMouseEvent *event)
{
	m_action_moved = true;
/*	
	if(m_action==ScalingAction){
		if(event->x()<2)return;
		float new_scale;
		float koef = ((float)(event->x())/(action_start_pos));
		float prev_scale = m_scale;
		
		if(m_scaling_pivot == SelectionPivot && selection_begin>0 && event->x() < (selection_begin-m_offset)*m_scale){
			new_scale = start_scale / koef;
			setCursor(Qt::ArrowCursor);
		}
		else{
			new_scale = start_scale * koef;
			setCursor(Qt::ArrowCursor);
		}
		
		setScale(new_scale);
		
		if(m_scaling_pivot == SelectionPivot && selection_begin>0){
			setOffset( m_offset + (1 - prev_scale/m_scale)*(selection_begin - m_offset) );
		}
	}
*/	
	if(m_action==ShiftingAction){
		double new_offset = m_action_start_offset + (m_action_start_x-event->x())*m_range/width();
		setCursor(Qt::ClosedHandCursor);
		setBegin(new_offset);
	}
	
//	if(m_action==SelectingAction){
//		if(m_selection_mode==RangeSelectionMode){
//			setSelectionRange(action_start_pos/m_scale + m_offset+crop_begin ,
//				event->x()/m_scale + m_offset+crop_begin );
//		}
//		if(m_selection_mode==PositionSelectionMode){
//			setSelectionRange(event->x()/m_scale + m_offset+crop_begin ,
//				event->x()/m_scale + m_offset+crop_begin );
//		}
//	}
	
}
