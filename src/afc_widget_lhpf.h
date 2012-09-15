#ifndef AFC_LHPF_WIDGET_H
#define AFC_LHPF_WIDGET_H
//
#include <QtGui>
#include "afc_widget.h"
//
class AFCWidgetLowHigh : public AFCWidget
{
Q_OBJECT

	bool m_mouse_pressed;
	int m_cutoff;

	void updateParameters(int x, int y);

public:
	AFCWidgetLowHigh(QWidget *parent = 0);
	virtual ~AFCWidgetLowHigh();
	
protected:
	void timerEvent(QTimerEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void drawSpecificDetails(QPainter & painter, QRectF rect);

signals:
	void parametersChanged(int freq);
};
#endif
