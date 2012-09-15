#ifndef VOLUMEEDITWIDGET_H
#define VOLUMEEDITWIDGET_H
//
#include <QtGui>
//
class VolumeEditWidget : public QFrame
{
Q_OBJECT

	QSlider *m_slider;
	
public:
	VolumeEditWidget(QWidget *parent = 0);
	
private slots:
	void valueChange(int value);
	
public slots:
	void popup();
	
signals:
	void volumeChanged(float volume);
	
protected:
	void timerEvent(QTimerEvent *event);
	void keyPressEvent ( QKeyEvent * event );
};
#endif
