#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H
//
#include <QtGui>
#include "apu.h"
#include <list>
#include <vector>
//
class SpectrumView : public Apu
{
Q_OBJECT

	struct data_buf{
		std::vector<short> data;
		long long time;//msecs
	};
	std::list<data_buf> m_data_bufs;

	long m_fft_size;
	float m_update_time;
	float m_sensitivity;//dB

	int m_freq1;
	int m_freq2;

	QList<int> m_freq_labels;
	
	float * m_input_data;
	float * m_output_data;
	float * m_spectrum;
	
	clock_t m_prev_clock;
	
	int m_left_scale_size;
	int m_bottom_scale_size;
	bool m_show_info;
	int m_mouse_x;
	int m_mouse_y;
	
	void updateSpectrum();
	void drawSpectrum(QPainter & painter, QRectF rect);
	void drawAmplitudeScale(QPainter & painter);
	void drawFrequencyScale(QPainter & painter);

	float todB(float x);
	
public:
	SpectrumView(QWidget *parent = 0);
	~SpectrumView();
	
	void setFFTExponent(int exp);//1 - 2 samples, 4 - 16 samples, 12 - 4096 samples
	void setUpdateTime(float secs);//refresh period, secs < 0 - disable realtime updating
	void setAutoUpdating(bool auto_updating);
	void setSensitivity(float sens);
	
	void processData(short * data, long num_samples, long long start_time);
	void reset();
	
	QString name();
	
protected:
	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

};
#endif
