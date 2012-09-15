#ifndef __TRACK_H
#define __TRACK_H
//
#include <QtGui>
//
class Track : public QWidget
{
Q_OBJECT
public:
	enum ScalingPivot { LeftSidePivot, RightSidePivot, SelectionPivot };
	enum SelectionMode { RangeSelectionMode, PositionSelectionMode, NoSelectionMode };
	enum ActionMode{ ScalingAction, ShiftingAction, SelectingAction, NoAction};

protected:
	double m_range;
	double m_begin;

private:
	//control action-related members
	ScalingPivot m_scaling_pivot;
	SelectionMode m_selection_mode;
	ActionMode m_action;
	bool m_action_moved;
	int m_action_start_x;
	float m_action_start_range;
	double m_action_start_offset;

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent( QWheelEvent *event);

public:
	Track(QWidget * parent=0);
	virtual ~Track();

	void setRange(double range);
	void setBegin(double begin);
	void setEnd(double end);

signals:
	void contextMenuRequested(QPoint);
};
#endif
