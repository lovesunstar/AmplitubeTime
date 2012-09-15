#ifndef TIME_RULER_H
#define TIME_RULER_H
//
#include <QtGui>
#include "track.h"
//
class TimeRuler : public Track
{
Q_OBJECT

	QString m_marker_text_format;
	double m_marker_first;
	double m_marker_step;
	int m_marker_num;

	void updateMarkers();

protected:
	void paintEvent(QPaintEvent *event);

public:
	TimeRuler(QWidget * parent=0);
	virtual ~TimeRuler();

};
#endif
