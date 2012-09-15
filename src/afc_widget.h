#ifndef AFC_WIDGET_H
#define AFC_WIDGET_H
//
#include <QtGui>
//
class AFCWidget : public QWidget
{
Q_OBJECT
	void drawAmplitude(QPainter & painter, QRectF rect);
	void drawPhase(QPainter & painter, QRectF rect);
	void drawScales(QPainter & painter);

	QList<QPointF> m_dB_points;
	QList<QPointF> m_rad_points;

	QMenu * m_context_menu;

	bool m_show_info;
	int m_mouse_x;
	int m_mouse_y;

	QList<int> m_freq_labels;

protected:
	int m_left_scale_size;
	int m_bottom_scale_size;

	int m_freq1;
	int m_freq2;
	float m_log_freq1;
	float m_log_freq2;
	int m_dB1;
	int m_dB2;
	float m_rad1;
	float m_rad2;

	enum ViewMode {PhaseView, MagnitudeView, PhaseMagnitudeView};
	ViewMode m_view_mode;

	float linFreq(float x);//widget coordinate x -> linear scale freq
	float logFreq(float freq);//linear scale freq -> relative widget coordinate

public:
	AFCWidget(QWidget *parent = 0);
	virtual ~AFCWidget();
	
	//void setFreqRange(int freq1, int freq2);
	//void setdBRange(int dB1, int dB2);
	void clear();
	void addPointdB(float x, float y);
	void addPointRad(float x, float y);
	void setdBRange(int dB1, int dB2);
	void setRadRange(float r1, float r2);
	void setFreqRange(int freq1, int freq2);
	
protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	virtual void drawSpecificDetails(QPainter & painter, QRectF rect);

protected slots:
	void contextMenuPopup(QPoint pos);
	void viewPhase();
	void viewMagnitude();
	void viewPhaseMagnitude();

signals:
	void contextMenuRequested(QPoint);
};
#endif
