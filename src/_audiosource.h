#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H
//
#include <QtCore>
#include <QDir>
#include <list>
#include <iostream>
//
class AudioBuffer : public QObject
{
Q_OBJECT
	
	long long m_begin;
	long long m_length;
	int *m_data;
	
	int m_freq;
	int m_bytes_in_sample;
	int m_null_sample;
	
	QByteArray m_filedata;
	
public:
	AudioBuffer(int bytes_in_sample, int freq);
	~AudioBuffer();
	
	void setBegin(long long begin);
	void setFreq(int freq);
	
	bool fromFile(QString fname);
	long long getBegin();
	long long getEnd();
	long double getBeginSeconds();
	long double getEndSeconds();
	long long getNumSamples();
	int getFreq();
	int getBytesInSample();
	void* getData(long long *size=0);
	//signed int & getSample(long long nth);
};

//
//
//

class AudioSource : public QObject 
{
Q_OBJECT

	QDir m_dir;
	int m_bytes_in_sample;
	int m_freq;
	bool m_format_signed;
	bool m_swap_samples;
	
	QMap <long long, QPair<long double, QString> > m_dir_bufs;//key - begin timestamp, value - <buffer length in seconds, filename>
	long double m_data_start_time;
	long double m_data_end_time;
	QDateTime m_rescan_datetime;
	
	int m_null_sample;
	
	QMap< long long, AudioBuffer* > m_bufs;// <begin time in seconds, AudioBuffer*>
	QMap< AudioBuffer*, long long > m_bufs_last_need_time;// <begin time in seconds, AudioBuffer*>
	QMap< long long, AudioBuffer* >::iterator m_bufs_iter;

public:
	AudioSource(QString dir_, int bytes_in_sample, int freq, bool format_signed);
	
	void setFreq(int freq);
	void setBytesInSample(int bytes_in_sample);
	void setFormatSigned(bool format_signed);
	void setDirectory(QString path);
	void setSwapSamplesOrder(bool swap_samples);
	
	void reload(long double start, long double end);
	void reload();
	
	long long getNumSamples();
	int getFreq();
	int getBytesInSample();
	bool getFormatSigned();
	bool getSwapSamplesOrder();
	QString getDirectory();
	void* getDataPointer(long long start, long long end, long long *samples_written=0);
	void getData(long long start, long long end, char * dst);
	signed int getSample(long long nth);
	
private:
	void seekTo(long long nth);
	void clearBuffers();
	
};
#endif
