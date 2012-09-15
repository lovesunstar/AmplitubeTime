#include "apu_spectrum_view.h"
#include <cmath>
#include "FFT.h"

using namespace std;
//
SpectrumView::SpectrumView(QWidget *parent) 
	: Apu(parent)
{
	//m_pos = 0;
	m_update_time = 1.0;
	m_spectrum = 0;
	m_input_data = 0;
	m_output_data = 0;
	m_sensitivity = -100.0;
	setFFTExponent(11);

	m_freq1 = 20;
	m_freq2 = 20000;

	m_freq_labels.append(30);
	m_freq_labels.append(100);
	m_freq_labels.append(300);
	m_freq_labels.append(1000);
	m_freq_labels.append(3000);
	m_freq_labels.append(10000);

	
	m_freq = 44100;
	m_show_info = false;
	m_left_scale_size = 35;
	m_bottom_scale_size = 10;
	
	m_prev_clock=clock();
	
	//m_current_buffer = 0; //no current buffer working on
	
	setMinimumHeight(100);
	//startTimer(10);
}
//
QString SpectrumView::name()
{
	return tr("Spectrum viewer");
}
//
SpectrumView::~SpectrumView()
{
	delete[] m_spectrum;
	delete[] m_input_data;
	delete[] m_output_data;
}
//


void SpectrumView::setFFTExponent(int exp)
{
	m_fft_size = (long long)pow(2.0, exp);
	
	delete[] m_spectrum;
	delete[] m_input_data;
	delete[] m_output_data;
	
	m_input_data = new float[m_fft_size];
	m_output_data = new float[m_fft_size/2];
	m_spectrum = new float[m_fft_size/2];
	
	memset(m_input_data, 0, sizeof(float)*m_fft_size);
	memset(m_output_data, 0, sizeof(float)*m_fft_size/2);
	memset(m_spectrum, 0, sizeof(float)*m_fft_size/2);
}

void SpectrumView::setUpdateTime(float secs)
{
	m_update_time = secs;
	startTimer(int(secs*1000));
}

void SpectrumView::setSensitivity(float sens)
{
	m_sensitivity = sens;
}

void SpectrumView::timerEvent(QTimerEvent *event)
{
	if( isVisible() ){
		updateSpectrum();
		update();
	}
}

void SpectrumView::mousePressEvent(QMouseEvent *event)
{
	m_mouse_x = event->x();
	m_mouse_y = event->y();
	m_show_info = true;
	update();
}

void SpectrumView::mouseReleaseEvent(QMouseEvent *event)
{
	m_show_info = false;
	update();
}

void SpectrumView::mouseMoveEvent(QMouseEvent *event)
{
	m_mouse_x = event->x();
	m_mouse_y = event->y();
	m_show_info = true;
	update();
}


void SpectrumView::processData(short * data, long num_samples, long long start_time)
{
	data_buf buf = {vector<short>(data, data+num_samples), start_time};
	m_data_bufs.push_back( buf );
}


void SpectrumView::updateSpectrum()
{
	clock_t dt = clock() - m_prev_clock;
	m_prev_clock = clock();
	float elapsed_time = float(dt)/CLOCKS_PER_SEC;
	
	//qDebug() << elapsed_time;
	
	//draw testing code
	/*
	static long long pos = 0;
	pos+=m_update_time*1000.0;
	
	for(int i=0; i<m_fft_samples; ++i){
		float phase = float(pos) / 10.0;
		m_spectrum[i] = (sin((i-phase/2.0)*2*3.1415/m_fft_samples)+1.0) / 4.0 * (cos((i+phase)*2*2*3.1415/m_fft_samples)+1.0)*0.5 + 0.25 + 0.25*cos((i+phase/5.0)*5*3.1415/m_fft_samples);
	}
	*/
	
	int j = 0;
	list< data_buf >::iterator buf_iter;
	list< data_buf >::iterator first_need_buf_iter;
	first_need_buf_iter = buf_iter = m_data_bufs.begin();
	long long now = (long long) QDateTime::currentDateTime().toTime_t()*1000.0+QDateTime::currentDateTime().time().msec();
	while( buf_iter != m_data_bufs.end() && j<m_fft_size){
		int getted=0;
		
		if(now >= buf_iter->time){
			if(first_need_buf_iter == m_data_bufs.begin()){
				first_need_buf_iter = buf_iter;
			}
			int i=(now-buf_iter->time)*m_freq/1000;
			while( i<buf_iter->data.size() && j<m_fft_size ){
				m_input_data[j] = buf_iter->data[i];
				++j;
				++i;
				++getted;
			}
		}
		
		
		now += (long long) getted*1000.0/m_freq + 1;
		++buf_iter;
	}
	
	
	m_data_bufs.erase(m_data_bufs.begin(), first_need_buf_iter);
	
	while(j<m_fft_size){
		m_input_data[j] = 0;
		++j;
	}
	
	WindowFunc(2, m_fft_size, m_input_data);
	PowerSpectrum(m_fft_size, m_input_data, m_output_data);
	
	float decr = 0.85;
	float max_sample = pow(2.0, sizeof(short)*8.0)-1;
	
	for (int i = 0; i < m_fft_size/2; ++i)
	{
		m_spectrum[i] *= decr;
		if(m_spectrum[i]<0)  m_spectrum[i] = 0.0;
		
		m_output_data[i] = sqrt(m_output_data[i])/max_sample/m_fft_size;
		if(m_output_data[i] > m_spectrum[i]){
			m_spectrum[i] = m_output_data[i];
		}
		
	}
	
	/*
	//wikipedia fft
	long long i, j, n, m, mmax, istep;
	float tempr, tempi, wtemp, theta, wpr, wpi, wr, wi;
	
	float max_sample = pow(2.0, sizeof(short)*8.0)-1;
	
	
	int isign = -1;
	vector<float> data(m_fft_samples * 2 + 1);
	
	j = 0;
	list< data_buf >::iterator buf_iter;
	list< data_buf >::iterator first_need_buf_iter;
	first_need_buf_iter = buf_iter = m_data_bufs.begin();
	long long now = (long long) QDateTime::currentDateTime().toTime_t()*1000.0+QDateTime::currentDateTime().time().msec();
	//qDebug() << "spectrum now" << now;
	//qDebug() << "required len " << float(m_fft_samples*1000.0/m_freq); 
	while( buf_iter != m_data_bufs.end() && j<m_fft_samples){
		int getted=0;
		//bool trying=false;
		
		if(now >= buf_iter->time){
			if(first_need_buf_iter == m_data_bufs.begin()){
				first_need_buf_iter = buf_iter;
			}
			//trying = true;
			int i=(now-buf_iter->time)*m_freq/1000;
			while( i<buf_iter->data.size() && j<m_fft_samples ){
				data[j*2] = 0;
				data[j*2+1] = buf_iter->data[i];
				++j;
				++i;
				++getted;
			}
		}
		
		//qDebug() << "now=" << now << "buffer " << buf_iter->time << " - " << buf_iter->time+buf_iter->data.size()*1000/m_freq << " fetched " << getted << " trying " << trying;
		
		now += (long long) getted*1000.0/m_freq + 1;
		++buf_iter;
	}
	
	//if(j<m_fft_samples){
		//qDebug() << ":'-(";
		//exit(1);
	//}
	//else{
		//qDebug() << "OG";
	//}
	//qDebug() << "";
	
	m_data_bufs.erase(m_data_bufs.begin(), first_need_buf_iter);
	
	while(j<m_fft_samples){
		data[j*2] = 0;
		data[j*2+1] = 0.0;
		++j;
	}
	
	
	n = m_fft_samples << 1;
	j = 1;
	i = 1;
	while (i < n)
	{
		if (j > i)
		{
			tempr = data[i];   data[i]   = data[j];   data[j]   = tempr;
			tempr = data[i+1]; data[i+1] = data[j+1]; data[j+1] = tempr;
		}
		m = n >> 1;
		while ((m >= 2) && (j > m))
		{
			j = j - m;
			m = m >> 1;
		}
		j = j + m;
		i = i + 2;
	}
	mmax = 2;
	
	
	while (n > mmax)
	{
		istep = 2 * mmax;
		theta = 2.0*M_PI / (isign * mmax);
		wtemp = sin(0.5 * theta);
		wpr   = -2.0 * wtemp * wtemp;
		wpi   = sin(theta);
		wr    = 1.0;
		wi    = 0.0;
		m    = 1;
		while (m < mmax)
		{
			i = m;
			while (i < n)
			{
				j         = i + mmax;
				tempr     = wr * data[j] - wi * data[j+1];
				tempi     = wr * data[j+1] + wi * data[j];
				data[j]   = data[i] - tempr;
				data[j+1] = data[i+1] - tempi;
				data[i]   = data[i] + tempr;
				data[i+1] = data[i+1] + tempi;
				i         = i + istep;
			}
			wtemp = wr;
			wr    = wtemp * wpr - wi * wpi + wr;
			wi    = wi * wpr + wtemp * wpi + wi;
			m     = m + 2;
		}
		mmax = istep;
	}
	
	
	
	float new_value;
	float decr;
	decr = 0.8;//1 / pow(10, elapsed_time*3.0);
	
	for (i = 0; i < (m_fft_samples / 2); i++)
	{
		//float decr = elapsed_time * 7.0;
		//m_spectrum[i] -= m_spectrum[i]*pow(10, decr)/20.0;
		
		m_spectrum[i] *= decr;
		if(m_spectrum[i]<0)  m_spectrum[i] = 0.0;
			
		new_value = sqrt( float(data[i*2] * data[i*2] + data[i*2+1] * data[i*2+1]) ) / m_fft_samples / max_sample;
		if(new_value > m_spectrum[i]){
			m_spectrum[i] = new_value;
		}
		//m_spectrum[i] = new_value;
	}
	*/
}

void SpectrumView::reset()
{
	m_data_bufs.clear();
}

float SpectrumView::todB(float x)
{
	float h;
	if(x != 0.0){
		h = 20*log10( x );
	}
	else{
		h = m_sensitivity;
	}
	
	if(h < m_sensitivity){
		h = 0.0;
	}
	else{
		h = 1.0 - h/m_sensitivity;
	}

	return h;
}

void SpectrumView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, false);
	
	QRect rect(m_left_scale_size, 0, width()-m_left_scale_size-1, height()-m_bottom_scale_size-1);
	
	QFont font;
	font.setPointSize(7);
	painter.setFont(font);
	
	m_show_info = m_show_info && rect.contains(m_mouse_x, m_mouse_y);
	
	drawSpectrum(painter, rect);
	drawAmplitudeScale(painter);
	drawFrequencyScale(painter);
	
	painter.setPen( QPen(QColor(0, 0, 0, 255)) );
	QRectF text_rect(4, 4, width()-8, height()-8);
	painter.drawText( text_rect, Qt::AlignRight | Qt::AlignTop, QString::number(m_fft_size)+" pts" );
	
	
	painter.setPen( QPen(QColor(0, 0, 0, 255)) );
	painter.drawRect(rect);
	
}

void SpectrumView::drawSpectrum(QPainter & painter, QRectF rect)
{
	QBrush brush(QColor(225, 225, 225));
	painter.fillRect(rect, brush);
	
	//QLinearGradient grad(0.0, 0.0, 0.0, rect.height()/0.707);
	//grad.setColorAt(0.0, QColor(0, 255, 0));
	//grad.setColorAt(1.0, QColor(0, 0, 0));
	//QBrush spectre_brush(grad);
	QBrush spectre_brush(QColor(0,0,0));
	
	
	float rect_pos;
	float h;
	float freq_step = float(m_freq)/m_fft_size;
	
	QPointF prev_point(rect.left(), rect.height()*(1-todB(m_spectrum[int(m_freq1/freq_step)])));
	QPointF cur_point;

	for(int i=trunc(m_freq1/freq_step)+1; i<=m_freq2/freq_step && i<=m_fft_size/2; ++i){
		h = todB(m_spectrum[i]);
		
		//rect_pos = i;
		
		if(i>0){
			rect_pos = (log10(i*freq_step)-log10(m_freq1))/(log10(m_freq2)-log10(m_freq1));
		}
		else{
			rect_pos = 0;
		}
		

		cur_point = QPointF(rect.left()+rect_pos*rect.width(), rect.height()*(1-h));
		painter.drawLine(prev_point, cur_point);
		prev_point = cur_point;
		
	}
	
}

void SpectrumView::drawAmplitudeScale(QPainter & painter)
{
	painter.setPen( QPen(QColor(0, 0, 0, 255)) );
	QRectF text_rect;
	text_rect = QRectF(0, 0, m_left_scale_size-2, 100);
	painter.drawText( text_rect, Qt::AlignRight | Qt::AlignTop, "0 dB" );
	text_rect = QRectF(0, 0, m_left_scale_size-2, height()-m_bottom_scale_size);
	painter.drawText( text_rect, Qt::AlignRight | Qt::AlignBottom, QString::number(m_sensitivity)+" dB" );
}

void SpectrumView::drawFrequencyScale(QPainter & painter)
{
	painter.setPen( QPen(QColor(0, 0, 0, 255)) );
	QRectF text_rect;
	text_rect = QRectF(m_left_scale_size, height()-m_bottom_scale_size, 100, 100);
	painter.drawText( text_rect, Qt::AlignLeft | Qt::AlignTop, QString::number(m_freq1) );
	
	
	text_rect = QRectF(0, height()-m_bottom_scale_size, width(), 100);
	painter.drawText( text_rect, Qt::AlignRight | Qt::AlignTop, QString::number(m_freq2/1000)+"k" );
	
	if(m_show_info){
		text_rect = QRectF(m_left_scale_size, height()-m_bottom_scale_size, width()-m_left_scale_size, 100);
		//painter.drawText( text_rect, Qt::AlignHCenter | Qt::AlignTop, QString::number(double(m_freq/2.0*(m_mouse_x-m_left_scale_size)/(width()-m_left_scale_size)))+" Hz, "+QString::number(double(m_sensitivity*m_mouse_y/(height()-m_bottom_scale_size)),'g',2)+" dB" );
		
		int dBgain = int(m_sensitivity*m_mouse_y/(height()-m_bottom_scale_size));
		float freq = pow(10.0, log10(m_freq1)+(m_mouse_x-m_left_scale_size)/float(width()-m_left_scale_size)*(log10(m_freq2)-log10(m_freq1)));
		QString text = QString::number(freq)+" Hz, ";
		text += QString::number(dBgain)+" dB";
		painter.drawText( text_rect, Qt::AlignHCenter | Qt::AlignTop, text );
	}



	QPen grey_dashed_pen(QColor(0,0,0, 64));
	grey_dashed_pen.setStyle(Qt::DashLine);

	foreach(int freq, m_freq_labels){
		QString lbl;
		if(freq<1000){
			lbl = QString::number(freq);
		}
		else{
			lbl = QString::number(freq/1000) + "k";
		}

		float x = m_left_scale_size+(log10(freq)-log10(m_freq1))/(log10(m_freq2)-log10(m_freq1))*(width()-m_left_scale_size);
		//float x = m_left_scale_size + logFreq(freq)*(width()-m_left_scale_size);

		if(!m_show_info){
			text_rect = QRectF(x-100, height()-m_bottom_scale_size, 200, 100);
			painter.setPen( QPen(QColor(0, 0, 0, 255)) );
			painter.drawText( text_rect, Qt::AlignHCenter | Qt::AlignTop, lbl );
		}

		painter.setPen( grey_dashed_pen );
		painter.drawLine(x, 0, x, height()-m_bottom_scale_size);
	}
}
