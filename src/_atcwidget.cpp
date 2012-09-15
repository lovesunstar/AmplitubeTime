#include "atcwidget.h"
#include <cmath>

using namespace std;
//
ATCWidget::ATCWidget(AudioSource *audio_source, QWidget * parent ) 
	: QWidget(parent)
{
	m_audio_source = audio_source;
	
	//second_pixels=50;
	m_scaling_pivot = LeftSidePivot;
	m_selection_tracking = false;
	m_min_scale = -1.0;
	m_max_scale = -1.0;
	
	m_autoscroll = false;
	density=7;
	m_scale=50.0;
	//m_max_view=0.0;
	//m_min_view=0.0;
	m_offset=0;
	crop_begin = 0;
	crop_end = 0;
	selection_begin = 0;
	selection_end = 0;
	//buf=0;
	//buf_samples=0;
	m_play_position=0;
	scrollbar_height=5;
	timingbar_height=25;
	m_action=NoAction;
	m_selection_mode=RangeSelectionMode;
	setMinimumHeight(scrollbar_height+timingbar_height+40);
	//setBaseSize(10, scrollbar_height+timingbar_height+40);
	
	m_bg_color = QColor(225,225,225);

	setFocusPolicy(Qt::ClickFocus);
	
	startTimer(500);
}
//
ATCWidget::~ATCWidget()
{
}

#define MIN(x,y) (x>y?y:x)
#define MAX(x,y) (x>y?x:y)

void ATCWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, false);
	
	if(!m_audio_source){
		return;
	}
	
	m_audio_source->reload( m_offset, m_offset+viewLength() );
	
	QPen black_pen(QColor(0,0,0));
	QPen grey_dashed_pen(QColor(0,0,0, 64));
	grey_dashed_pen.setStyle(Qt::DashLine);
	
	
	long long num_samples = 0;
	num_samples = m_audio_source->getNumSamples();
	
	double pixel_samples=m_audio_source->getFreq()/m_scale;
	long long offset = (long long)(m_offset * m_audio_source->getFreq());
	
	if(crop_end!=crop_begin){
		num_samples = MIN(num_samples, crop_end);
		offset += crop_begin;
	}
	/*
	if(num_samples <= 0){
		return;
	}*/
	long long wave_end = (long long)( (num_samples-offset)/pixel_samples )+1;//in pixels
	
	
	QRect scroll_rect(0, 0, width()-1, scrollbar_height);
	QRect timing_rect(0, scrollbar_height, width()-1, timingbar_height);
	QRect wave_rect(0, scrollbar_height+timingbar_height, width(), height()-scrollbar_height-timingbar_height-1);
	
	
	//scroll bar
	//if(scrollbar_height>0){
		//painter.fillRect(scroll_rect, QBrush(QColor(225,225,225)));
		//QRect scroller_rect((m_offset-crop_begin)*scroll_rect.width()/(num_samples-crop_begin), 0, 
			//int (MIN(wave_end, scroll_rect.width())*pixel_samples*scroll_rect.width()/(num_samples-crop_begin))+2, scroll_rect.height());
		//painter.fillRect(scroller_rect, QBrush(QColor(150, 150, 250, 150)));
	//}
	
	
	//timing bar
	
	if(timingbar_height>0){
		qDrawShadePanel(&painter, timing_rect, QPalette() );
	}
	
	
	float marks_step_long;
	float marks_step_short;
	marks_step_long = getCurrentStep();
	marks_step_short = marks_step_long/2;
	
	//in msecs
	long long time_shift = (long long)(m_offset/marks_step_long)*marks_step_long*1000 ;
	QDateTime time = QDateTime::fromTime_t(0);
	time=time.addMSecs( time_shift );
	
	QString format;
	if(marks_step_long<1.0){
		format="s.zzz";
	}
	else
	if(marks_step_long<60.0){
		format="hh:mm:ss";
	}
	else
	if(marks_step_long<86400.0){
		format="hh:mm";
	}
	else{
		format="dd.MM.yyyy";
	}
	
	
	//main view
	if(wave_rect.height()>2){
		QRect empty_wave_rect = wave_rect;
		empty_wave_rect.setLeft( MIN(wave_rect.right(), wave_end) );
		empty_wave_rect.setRight( wave_rect.right() );
	
		painter.fillRect(wave_rect, QBrush(m_bg_color) );
		painter.fillRect(empty_wave_rect, QBrush(m_bg_color) );
		//painter.fillRect(empty_wave_rect, QPalette().window() );
	}
	
	//time marks
	float mark_pos=0.0;
	for(int i=0; mark_pos<width(); ++i){
		mark_pos = ((time_shift/1000.0 + i*marks_step_long)*m_audio_source->getFreq()-offset)/pixel_samples;
		
		if(timingbar_height>0){
			painter.setPen(black_pen);
			
			QLineF line( mark_pos, timing_rect.bottom()-timingbar_height/5, mark_pos, timing_rect.bottom());
			painter.drawLine(line);
			
			QString lbl;
			if(marks_step_long<86400.0 && time.time().hour()==0 && time.time().minute()==0 && time.time().second()==0 && time.time().msec()==0){
				lbl=time.toString("dd.MM.yyyy");
				QFont font;
				font.setBold(true);
				painter.setFont(font);
			}
			else{
				lbl=time.toString(format);
				painter.setFont(QFont());
			}
			
			QRectF rect(mark_pos-100.0, timing_rect.top(), 200.0, timingbar_height-timingbar_height/10.0);
			//qDebug() << rect << mark_pos;
			painter.drawText(rect, Qt::AlignCenter, lbl);
		}
		
		if(wave_rect.height()>1){
			painter.setPen(grey_dashed_pen);
			QLineF line( mark_pos, wave_rect.top(), mark_pos, wave_rect.bottom());
			//qDebug() << line << mark_pos;
			painter.drawLine(line);
		}
		
		time=time.addMSecs(int(marks_step_long*1000));
	}
	
	//qDebug() << "";
	
	//cursor, selection
	float cursor_pos = (selection_begin*m_audio_source->getFreq()-offset)/pixel_samples;
	QLineF cursor_line (cursor_pos, 0, cursor_pos, height());
	QLineF sel_begin_line ((selection_begin*m_audio_source->getFreq()-offset)/pixel_samples, 0, (selection_begin*m_audio_source->getFreq()-offset)/pixel_samples, height());
	QLineF sel_end_line ((selection_end*m_audio_source->getFreq()-offset)/pixel_samples, 0, (selection_end*m_audio_source->getFreq()-offset)/pixel_samples, height());
	
	QPen cursor_pen;
	QBrush selection_brush(QColor(0, 0, 0, 70));
	painter.fillRect( (selection_begin*m_audio_source->getFreq()-offset)/pixel_samples, 0, 
		(selection_end-selection_begin)*m_audio_source->getFreq()/pixel_samples+1, height(), selection_brush);
	
	
	if(selection_begin != selection_end){
		cursor_pen.setColor(QColor(0, 0, 0, 200));
		cursor_pen.setWidth(1);
		painter.setPen(cursor_pen);
		painter.drawLine(sel_begin_line);
		painter.drawLine(sel_end_line);
	}
	else{
		cursor_pen.setColor(QColor(0, 0, 0, 40));
		cursor_pen.setWidth(3);
		painter.setPen(cursor_pen);
		painter.drawLine(cursor_line);
		
		cursor_pen.setColor(QColor(0, 0, 0, 200));
		cursor_pen.setWidth(1);
		painter.setPen(cursor_pen);
		painter.drawLine(cursor_line);
	}
	
	painter.drawLine( (m_play_position-offset)/pixel_samples, 0, (m_play_position-offset)/pixel_samples, height() );
	
	
	//samples
	
	if(wave_end<=0 || wave_rect.height()<2)goto skip_wave;
	painter.setPen(QPen(QColor(0,0,0,255)));
	
	unsigned int max_sample;
	max_sample = (unsigned int)( pow(2.0, m_audio_source->getBytesInSample()*8) )-1;
	/*
	long long samples_step = (long long)pixel_samples/2.0;//(m_audio_source->getFreq()/density/m_scale);
	if(samples_step==0)samples_step=1;
	offset -= offset%samples_step;
	
	unsigned int prev_sample=m_audio_source->getSample(offset-1);
	unsigned int cur_sample;
	
	int pts;
	pts = 50;
	long long in_sample_step;
	in_sample_step = samples_step/pts;
	
	long long i;
	unsigned int sample_max;
	unsigned int sample_min;
	
	for( i=offset; i<num_samples && (i-offset)/pixel_samples<wave_rect.width(); i+=samples_step ){
		
		sample_min=sample_max=m_audio_source->getSample(i);
		
		if(in_sample_step>=1){
			for(int j=0; j<samples_step; j+=in_sample_step){
				unsigned int sample = m_audio_source->getSample(i+j);
				if(sample>sample_max){
					sample_max = sample;
				}
				if(sample<sample_min){
					sample_min = sample;
				}
			}
		}
		
		sample_max = wave_rect.top()+wave_rect.height()/2+( float((signed int)sample_max)/max_sample*wave_rect.height() );
		sample_min = wave_rect.top()+wave_rect.height()/2+( float((signed int)sample_min)/max_sample*wave_rect.height() );
				
		painter.drawLine(QLineF(
			(i-offset)/pixel_samples, prev_sample, 
			(i-offset+samples_step)/pixel_samples, sample_max)
			);
		
		if(sample_max != sample_min)
		painter.drawLine(QLineF( 
			(i-offset+samples_step)/pixel_samples, sample_max, 
			(i-offset+samples_step)/pixel_samples, sample_min)
			);
		
		prev_sample=sample_min;
	}
	*/
	
	long long samples_step;
	samples_step = (long long)pixel_samples;//(m_audio_source->getFreq()/density/m_scale);
	if(samples_step==0)samples_step=1;
	offset -= offset%samples_step;
	
	unsigned int prev_sample;
	prev_sample=wave_rect.top()+wave_rect.height()/2+(signed int)( float(m_audio_source->getSample(offset-1))/max_sample*wave_rect.height() );
	unsigned int cur_sample;
	
	long long i;
	for( i=offset; i<num_samples && (i-offset)/pixel_samples<wave_rect.width(); i+=samples_step ){
		cur_sample=wave_rect.top()+wave_rect.height()/2+(signed int)( float(m_audio_source->getSample(i))/max_sample*wave_rect.height() );
		
		QLineF line( (i-offset)/pixel_samples, prev_sample, 
			(i-offset+samples_step)/pixel_samples, cur_sample);
		painter.drawLine(line);
		prev_sample=cur_sample;
	}
	
	
	
	skip_wave:
	//frames
	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setPen(QPen(QColor(50,50,50)));
	painter.setBrush(QBrush(QColor(0,0,0,0)));
	if(scrollbar_height>0)
		painter.drawRect(scroll_rect);
	if(timingbar_height>0)
		painter.drawRect(timing_rect);
	if(wave_rect.height()>1){
		wave_rect.setRight(wave_rect.right()-1);
		painter.drawRect(wave_rect);
	}
	
}


void ATCWidget::mousePressEvent(QMouseEvent *event)
{
	if(!m_audio_source)return;
	action_start_pos = event->x();
	
	if(event->button() == Qt::LeftButton){
		m_action=SelectingAction;
	}
	
	if(event->button() == Qt::RightButton){
		m_action=ScalingAction;
		start_scale = m_scale;
		setCursor(Qt::SizeHorCursor);
	}
	
	if(event->button() == Qt::MidButton){
		//if(m_autoscroll)return;
		m_action=ShiftingAction;
		start_offset = m_offset;
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

void ATCWidget::wheelEvent(QWheelEvent *event)
{
	if(!m_audio_source)return;
	
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
	
	if(qApp->keyboardModifiers() == Qt::NoModifier){
		//if(m_autoscroll)return;
		
		float step;
		step = getCurrentStep();
		setOffset(m_offset-step*event->delta()/120.0);
	}
	
}

void ATCWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(!m_audio_source)return;
	
	if(!m_action_moved && event->button()==Qt::RightButton){
		emit contextMenuRequested(mapToGlobal(QPoint(event->x(), event->y())));
	}
	/*
	if(m_action==SelectingAction){
		if(m_selection_mode==RangeSelectionMode){
			setSelectionRange(action_start_pos/m_scale + m_offset+crop_begin ,
				event->x()/m_scale + m_offset+crop_begin );
		}
		if(m_selection_mode==PositionSelectionMode){
			setSelectionRange(event->x()/m_scale + m_offset+crop_begin ,
				event->x()/m_scale + m_offset+crop_begin );
		}
	}
	*/
	m_action=NoAction;
	setCursor(Qt::ArrowCursor);
}

void ATCWidget::mouseMoveEvent(QMouseEvent *event)
{
	m_action_moved = true;
	
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
	
	if(m_action==ShiftingAction){
		//qDebug() << "start_offset =" << start_offset;
		offset_t new_offset = start_offset + (action_start_pos-event->x())/m_scale;
		//qDebug() << "new_offset =" << new_offset;
		//long long new_offset = m_offset + (action_start_pos-event->x())*m_audio_source->getFreq()/m_scale;
		//action_start_pos=event->x();
		setCursor(Qt::ClosedHandCursor);
		//m_offset = new_offset;
		//update();
		setOffset(new_offset);
	}
	
	if(m_action==SelectingAction){
		if(m_selection_mode==RangeSelectionMode){
			setSelectionRange(action_start_pos/m_scale + m_offset+crop_begin ,
				event->x()/m_scale + m_offset+crop_begin );
		}
		if(m_selection_mode==PositionSelectionMode){
			setSelectionRange(event->x()/m_scale + m_offset+crop_begin ,
				event->x()/m_scale + m_offset+crop_begin );
		}
	}
	
}

void ATCWidget::setCropRange(offset_t begin, offset_t end)
{
	if(begin<0){
		begin=0;
	}
	if(end<0){
		end=0;
	}
	
	if(begin>end){
		offset_t t = begin;
		begin = end;
		end = t;
	}
	
	if(begin==crop_begin && end==crop_end){
		return;
	}
	
	
	crop_begin = begin;
	crop_end = end;
	setOffset(0);
	update();
}

void ATCWidget::setSelectionRange(offset_t begin, offset_t end)
{
	if(begin>end){
		offset_t t = begin;
		begin = end;
		end = t;
	}
	
	if(selection_begin==begin && selection_end==end){
		return;
	}
	
	
	selection_begin = begin;
	selection_end = end;
	emit selectionChanged(begin, end);
	
	if(m_selection_tracking){
		if((long double)(begin-m_offset)/viewLength() < 0.2){
			setOffset(begin - viewLength()*0.2);
		}
		
		if((long double)(begin-m_offset)/viewLength() > 0.8){
			setOffset(begin - viewLength()*0.8);
		}
	}
	
	update();
}



void ATCWidget::setAutoscroll(bool autoscroll)
{
	if(!m_audio_source)return;
	
	m_autoscroll = autoscroll;
	if(m_autoscroll){
		setOffset((long long)QDateTime::currentDateTime().toTime_t() - viewLength() );
	}
}

void ATCWidget::setScrollbarSize(int height_)
{
	scrollbar_height=height_;
	update();
}

void ATCWidget::setTimingbarSize(int height_)
{
	timingbar_height=height_;
	update();
}

void ATCWidget::setMinimumScale(float scale)
{
	if(m_min_scale!=scale){
		m_min_scale=scale;
		//setScale(m_scale);
		update();
	}
}

void ATCWidget::setMaximumScale(float scale)
{
	if(m_max_scale!=scale){
		m_max_scale=scale;
		//setScale(m_scale);
		update();
	}
}

void ATCWidget::setScale(float scale)
{
	if(m_scale!=scale){
		if(m_min_scale >0 && scale < m_min_scale){
			scale = m_min_scale;
		}
		if(m_max_scale >0 && scale > m_max_scale){
			scale = m_max_scale;
		}
		
		m_scale=scale;
		emit scaleChanged(m_scale);
		emit viewChanged(m_offset, m_offset+viewLength() );
		update();
	}
}

void setMinimumScale(float view)
{
	//m_min_scale =
}

void ATCWidget::setOffset(offset_t offset)
{
	if(!m_audio_source)return;
	
	if(m_offset != offset){
		m_offset=offset;
		if(m_offset<0){
			m_offset=0;
		}
		if(m_offset+viewLength()>(offset_t)(QDateTime::currentDateTime().toTime_t())){
			m_offset = (offset_t)(QDateTime::currentDateTime().toTime_t()) - viewLength();
		}
		
		emit offsetChanged(m_offset);
		emit viewChanged(m_offset, m_offset+viewLength() );
		//m_audio_source->reload(m_offset, m_offset+viewLength());
		update();
	}
}


void ATCWidget::setDensity(float density_)
{
	density=density_;
	update();
}

void ATCWidget::setScalingPivot(ScalingPivot pivot)
{
	m_scaling_pivot = pivot;
}

void ATCWidget::enableSelectionTracking(bool tracking)
{
	m_selection_tracking = tracking;
}

float ATCWidget::getCurrentStep()
{
	float seconds_in_view = 400 / m_scale;
	float step;
	
	if(seconds_in_view < 0.1){
		step = 0.0125;
	}
	else
	if(seconds_in_view < 0.3){
		step = 0.05;
	}
	else
	if(seconds_in_view < 0.6){
		step = 0.1;
	}
	else
	if(seconds_in_view < 1.0){
		step = 0.2;
	}
	else
	if(seconds_in_view < 2.0){
		step = 0.25;
	}
	else
	if(seconds_in_view < 4){
		step = 0.5;
	}
	else
	if(seconds_in_view < 8.0){
		step = 1.0;
	}
	else
	if(seconds_in_view < 15.0){
		step = 2.0;
	}
	else
	if(seconds_in_view < 25.0){
		step = 5.0;
	}
	else
	if(seconds_in_view < 50.0){
		step = 10.0;
	}
	else
	if(seconds_in_view < 100.0){
		step = 20.0;
	}
	else
	if(seconds_in_view < 150.0){
		step = 30.0;
	}
	else
	if(seconds_in_view < 200.0){
		step = 60.0;
	}
	else
	if(seconds_in_view < 400.0){
		step = 120.0;
	}
	else
	if(seconds_in_view < 600.0){
		step = 300.0;
	}
	else
	if(seconds_in_view < 1500.0){
		step = 600.0;
	}
	else
	if(seconds_in_view < 3000.0){
		step = 1200.0;
	}
	else
	if(seconds_in_view < 5000.0){
		step = 1800.0;
	}
	else
	if(seconds_in_view < 10000.0){
		step = 3600.0;
	}
	else
	if(seconds_in_view < 20000.0){
		step = 7200.0;
	}
	else
	if(seconds_in_view < 40000.0){
		step = 14400.0;
	}
	else
	if(seconds_in_view < 500000.0){
		step = 86400.0;
	}
	else{
		step = 604800.0;
	}
	
	
	return step;
}

AudioSource* ATCWidget::getAudioSource()
{
	return m_audio_source;
}

float ATCWidget::getScale()
{
	return m_scale;
}

ATCWidget::offset_t ATCWidget::getOffset()
{
	return m_offset;
}

ATCWidget::ActionMode ATCWidget::getCurrentAction()
{
	return m_action;
}

bool ATCWidget::isAutoscrolling()
{
	return m_autoscroll
		&& m_action!=ShiftingAction
		&& m_offset+viewLength() > (offset_t)(QDateTime::currentDateTime().toTime_t()-5);
}


void ATCWidget::timerEvent(QTimerEvent *event)
{
	if(isAutoscrolling()){
		setOffset( (offset_t)(QDateTime::currentDateTime().toTime_t()) - viewLength() );
	}
}

void ATCWidget::setPlayPosition(offset_t play_position)
{
	if(m_play_position != play_position){
		m_play_position=play_position;
		update();
	}
}

void ATCWidget::setAudioSource(AudioSource *au)
{
	m_audio_source = au;
}

void ATCWidget::setSelectionMode(SelectionMode selection_mode)
{
	m_selection_mode = selection_mode;
}

void ATCWidget::setBackground(QColor bg)
{
	if(m_bg_color != bg){
		m_bg_color = bg;
		update();
	}
}

ATCWidget::offset_t ATCWidget::viewLength()
{
	if(!m_audio_source){
		return 0;
	}
	return width()/m_scale;
}
