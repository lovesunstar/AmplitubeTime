#ifndef AFC_PEAK_WIDGET_H
#define AFC_PEAK_WIDGET_H
//
#include <QtGui>
#include "afc_widget.h"
//
class AFCWidgetPeak : public AFCWidget
{
Q_OBJECT

	bool m_mouse_pressed;
	int m_f0;

	void updateParameters(int x, int y);

public:
	AFCWidgetPeak(QWidget *parent = 0);
	virtual ~AFCWidgetPeak();
	
protected:
	void timerEvent(QTimerEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void drawSpecificDetails(QPainter & painter, QRectF rect);

signals:
	void parametersChanged(int dBgain, int freq);
};
#endif
