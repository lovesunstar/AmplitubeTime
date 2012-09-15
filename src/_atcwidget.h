#ifndef ATCWIDGET_H
#define ATCWIDGET_H
//
#include <QtGui>
#include <QtOpenGL>
#include "audiosource.h"
/*
enum ATCWidgetActionMode{
	ScalingAction,
	ShiftingAction,
	SelectingAction,
	NoAction
};
*/

//
class ATCWidget : public QWidget
{
Q_OBJECT
public:
	enum ScalingPivot { LeftSidePivot, RightSidePivot, SelectionPivot };
	enum SelectionMode { RangeSelectionMode, PositionSelectionMode, NoSelectionMode };
	enum ActionMode{ ScalingAction, ShiftingAction, SelectingAction, NoAction};
	typedef long double offset_t;//seconds
	
private:
	QColor m_bg_color;

	bool m_selection_tracking;

	ScalingPivot m_scaling_pivot;
	SelectionMode m_selection_mode;
	ActionMode m_action;
	bool m_action_moved;
	int action_start_pos;
	float start_scale;
	offset_t start_offset;
	
	offset_t m_play_position;//samples
	
	bool m_autoscroll;
	
	float m_scale;//pixels in second
	float m_max_scale, m_min_scale;
	//float m_max_view, m_min_view;
	offset_t m_offset;
	float density;
	
	int scrollbar_height;
	int timingbar_height;
	
	AudioSource * m_audio_source;
	
	float getCurrentStep();
	
public:
	ATCWidget(AudioSource *audio_source_, QWidget * parent = 0);
	~ATCWidget();
	
	offset_t selection_begin;
	offset_t selection_end;
	offset_t crop_begin;
	offset_t crop_end;
	offset_t viewLength();
	
	float getScale();
	offset_t getOffset();
	
	AudioSource * getAudioSource();
	ActionMode getCurrentAction();
	bool isAutoscrolling();
	
public slots:
	void setScale(float scale);
	void setMinimumScale(float scale);
	void setMaximumScale(float scale);
	void setScalingPivot(ScalingPivot pivot);
	void setOffset(ATCWidget::offset_t offset_);
	void setDensity(float density_);
	void setScrollbarSize(int height_);
	void setTimingbarSize(int height_);
	void setSelectionRange(ATCWidget::offset_t begin, ATCWidget::offset_t end);
	void setAutoscroll(bool autoscroll);
	void setCropRange(ATCWidget::offset_t begin_, ATCWidget::offset_t end_);
	void setPlayPosition(ATCWidget::offset_t play_position);
	
	void setSelectionMode(SelectionMode selection_mode);
	void setAudioSource(AudioSource *au);
	void setBackground(QColor bg);
	
	void enableSelectionTracking(bool tracking);
	
signals:
	void scaleChanged(float scale);
	void offsetChanged(ATCWidget::offset_t offset);
	void selectionChanged(ATCWidget::offset_t begin, ATCWidget::offset_t end);
	void viewChanged(ATCWidget::offset_t begin, ATCWidget::offset_t end);
	void contextMenuRequested(QPoint);
	
	
protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent( QWheelEvent *event);
	
	void setOffsetPixels(float pixels_offset);

	
};
#endif
