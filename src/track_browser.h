#ifndef __TRACK_BROWSER_H
#define __TRACK_BROWSER_H
//
#include <QtGui>
#include "time_ruler.h"
//
class TrackBrowser : public QWidget
{
Q_OBJECT

	QDateTimeEdit * m_date_edit;
	QPushButton * m_autoscroll_button;

	QBoxLayout * m_track_box;
	TimeRuler * m_ruler;

	QLayout * createCalendarLayout();

protected:

public:
	TrackBrowser(QWidget * parent=0);
	virtual ~TrackBrowser();

//	void addSource(Source * source);
//	void removeSource(Source * source);

signals:
//	void selectionChanged(Source * source, float begin, float end);
};
#endif
