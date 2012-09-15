#include "audiosource.h"
#include <cmath>
//
AudioSource::AudioSource( QString dir_, int bytes_in_sample, int freq, bool format_signed) 
{
	m_dir = QDir(dir_);
	m_bytes_in_sample = bytes_in_sample;
	m_freq = freq;
	m_format_signed = format_signed;
	m_null_sample=0;
	m_swap_samples = false;
	
	m_data_start_time = 0.0;
	m_data_end_time = 0.0;
	m_rescan_datetime = QDateTime::fromTime_t(0);
	
	m_bufs_iter = m_bufs.end();
}
//

signed int AudioSource::getSample(long long nth)
{
	//qDebug() << "AudioSource::getSample()";
	seekTo(nth);
	
	if(m_bufs_iter == m_bufs.end()){
		return m_null_sample;
	}
		
		
	static signed int sample;
	static long long data_size;
	static void *data;
	data = (*m_bufs_iter)->getData(&data_size);
	
	long long sample_number = nth-(*m_bufs_iter)->getBegin();
	
	if(data_size/m_bytes_in_sample<=sample_number+1){
		return m_null_sample;
	}
	
	if(m_format_signed){
		if(m_bytes_in_sample==1){
			sample = ((signed char*) data ) [sample_number];
		}else
		if(m_bytes_in_sample==2){
			sample = ((signed short*) data ) [sample_number];
		}
		else
			return m_null_sample;
	}
	else{
		if(m_bytes_in_sample==1){
			sample = ((unsigned char*) data ) [sample_number];
		}else
		if(m_bytes_in_sample==2){
			sample = ((unsigned short*) data ) [sample_number];
		}
		else
			return m_null_sample;
	}
	
	//sample = ((unsigned int*) data ) [sample_number];
	
	return sample;
	
}

void AudioSource::seekTo(long long nth)
{
	if(m_bufs.empty()){
		m_bufs_iter = m_bufs.end();
		return;
	}
	
	
	if(m_bufs_iter == m_bufs.end()){
		m_bufs_iter = m_bufs.begin();
	}
	
	
	while( m_bufs_iter!=m_bufs.end() && !((*m_bufs_iter)->getBegin() <= nth && (*m_bufs_iter)->getEnd() > nth) ){
		++m_bufs_iter;
	}
	
}

void AudioSource::setFreq(int freq)
{
	if(m_freq != freq){
		m_freq = freq;
	
		clearBuffers();
		reload();
	}
}

void AudioSource::clearBuffers()
{	
	foreach( AudioBuffer * buf, m_bufs.values() ){
		delete buf;
	}
	m_bufs.clear();
	m_rescan_datetime = QDateTime::fromTime_t(0);
}

void AudioSource::setFormatSigned(bool format_signed)
{
	m_format_signed = format_signed;
}

void AudioSource::setBytesInSample(int bytes_in_sample)
{
	if( m_bytes_in_sample != bytes_in_sample) {
		m_bytes_in_sample = bytes_in_sample;
		
		clearBuffers();
		reload();
	}
}

void AudioSource::setSwapSamplesOrder(bool swap_samples)
{
	if( m_swap_samples != swap_samples ){
		m_swap_samples = swap_samples;
		
		clearBuffers();
		reload();
	}
}

void AudioSource::setDirectory(QString path)
{
	m_dir.setPath(path);
}

QString AudioSource::getDirectory()
{
	return m_dir.path();
}

int AudioSource::getBytesInSample()
{
	return m_bytes_in_sample;
}

int AudioSource::getFreq()
{
	return m_freq;
}

bool AudioSource::getFormatSigned()
{
	return m_format_signed;
}

bool AudioSource::getSwapSamplesOrder()
{
	return m_swap_samples;
}



void AudioSource::reload(long double start, long double end)
{	
	//qDebug() << "AudioSource::reload()";
	
	m_data_start_time = start;
	m_data_end_time = end;
	
	
	//scaning source directory and create filelist(m_dir_bufs)
	if(QDateTime::currentDateTime().toTime_t() - m_rescan_datetime.toTime_t() > 5.0){
		m_rescan_datetime = QDateTime::currentDateTime();
		
		//qDebug() << "rescanning source directory" << m_dir.path();
		
		m_dir_bufs.clear();
		
		foreach( QString fname, m_dir.entryList(QDir::Files | QDir::Readable) ){
			fname = m_dir.filePath(fname);
			
			bool convert_ok;
			fname.replace('\\', '/');
			QString stimestamp = fname.split('/').last().split('.').first();
			long long timestamp = stimestamp.toLongLong(&convert_ok, 16);
			if( !convert_ok ){
				continue;
			}
			
			m_dir_bufs.insert(timestamp, QPair<long double, QString>((long double)(QFile(fname).size())/m_bytes_in_sample/m_freq, fname) );
		}
	}
	
	//check if there is unvisible buffers
	foreach( AudioBuffer* buf, m_bufs.values() ){
		if( m_data_start_time<=buf->getEndSeconds() && m_data_end_time >=buf->getBeginSeconds() )
			continue;
		
		if(QDateTime::currentDateTime().toTime_t() - m_bufs_last_need_time[buf] < 5){
			m_bufs_last_need_time[buf] = QDateTime::currentDateTime().toTime_t();
			continue;
		}
			
		
		
		long long timestamp = (long long)buf->getBeginSeconds();
		if(m_bufs.remove(timestamp)<1){
			qDebug() << "cannot remove buffer at" << timestamp;
			continue;
		}
			
		delete buf;
			
		QDateTime datetime = QDateTime::fromTime_t(timestamp);
		qDebug() << "Audio buffer removed" << datetime.toString("dd.MM.yyyy hh:mm:ss");
	}
	
	//check if there is not loaded and visible buffers
	foreach( long long time, m_dir_bufs.keys() ){
		
		if(m_bufs.contains(time) || !(m_data_start_time<=time+m_dir_bufs[time].first && m_data_end_time >=time) ){
			continue;
		}
		
		QString fname = m_dir_bufs[time].second;
		
		AudioBuffer *buf = new AudioBuffer(m_bytes_in_sample, m_freq);
		
		
		if( buf->fromFile(fname) ){
			buf->setBegin(time * m_freq);
			m_bufs.insert(time, buf);
			m_bufs_last_need_time.insert(buf, QDateTime::currentDateTime().toTime_t());
			
			QDateTime datetime = QDateTime::fromTime_t(time);
			qDebug() << "Audio buffer from" << m_dir.path() << "loaded at" << datetime.toString("dd.MM.yyyy hh:mm:ss");
		}
		else{
			delete buf;
		}
	}
	
	m_bufs_iter = m_bufs.end();
	
	//qDebug() << "AudioSource::~reload()";
}

void AudioSource::reload()
{
	reload(m_data_start_time, m_data_end_time);
}

void* AudioSource::getDataPointer(long long start, long long end, long long * samples_written)
{
	AudioBuffer *next_buf=0;
	
	
	foreach(AudioBuffer *buf, m_bufs.values()){
		if( (next_buf==0 || next_buf->getBegin()-start > buf->getBegin()-start) && start<=buf->getBegin() ){
			next_buf=buf;
		}
		
		
		if( start > buf->getBegin() && start < buf->getEnd() ){
			char *data = (char*)buf->getData();
			data += (start-buf->getBegin()) * m_bytes_in_sample;
			if(samples_written)*samples_written = qMin(end, buf->getEnd() )-start;
			return data;
		}
	}
	
	if(samples_written){
		*samples_written = 0;
		
		if(next_buf)
			*samples_written = next_buf->getBegin()-start+1;
	}
	
	
	return 0;
}

void AudioSource::getData(long long start, long long end, char * dst)
{
	long long buf_size = 10000;
	long long pos = start;

	
	while(pos < end){
		long long num_samples;
		char * data = (char*) getDataPointer(pos, qMin(end, pos + buf_size), &num_samples);
		
		if(data){
			memcpy(dst, data, num_samples * m_bytes_in_sample);
			
			if(m_bytes_in_sample == 1 && m_format_signed){
				for(int i=0; i<num_samples; ++i){
					dst[i] += 128;
				}
			}
			
		}
		else{
			num_samples = buf_size;
			if(pos + num_samples > end){
				num_samples = end-pos;
			}
			
			memset(dst, 0, num_samples * m_bytes_in_sample );
		}
		
		pos += num_samples;
		dst += num_samples * m_bytes_in_sample;
	}
}

long long AudioSource::getNumSamples()
{
	long long max = 0;
	
	QMapIterator<long long, QPair<long double, QString> > iter(m_dir_bufs);
	while(iter.hasNext()){
		iter.next();
		long long end = (long long) ( (iter.key()+iter.value().first) * m_freq );
		if(end>max)
			max = end;
	}
	
	return max;
	
}

////////////

AudioBuffer::AudioBuffer(int bytes_in_sample, int freq)
{
	m_data=0;
	m_freq = freq;
	m_bytes_in_sample = bytes_in_sample;
	m_null_sample = (int)( pow(2.0, m_bytes_in_sample*8) )/2;
}

AudioBuffer::~AudioBuffer()
{
	//if(m_data){
		//delete m_data;
	//}
}

bool AudioBuffer::fromFile(QString fname)
{
	m_begin = 0;
	
	QFile file(fname);
	if( !file.open(QIODevice::ReadOnly) ){
		qDebug() << "Error: file not opened\n";
		return false;
	}
	
	m_filedata = file.readAll();
	
	file.close();
	
	
	m_length = m_filedata.size()/m_bytes_in_sample;
	//m_data = new int[m_length];
	
	m_data = (int*)m_filedata.data();
	
	/*
	if(m_bytes_in_sample == 1){
		char * data = (char*) m_data;
		
		for(int i=0; i<m_length; ++i){
			data[i] += 128;
		}
	}
	*/
/*
	for(long long i=0; i<m_length; ++i ){
		m_data[i]=0;
		for(int j=0; j<m_bytes_in_sample; ++j){
			m_data[i] <<= 8;
			m_data[i] += (unsigned char)filedata.at(i*m_bytes_in_sample+j);
		}
	}
	*/
	
	qDebug() << this << "AudioBuffer::fromFile()";
	
	return true;
}

void AudioBuffer::setBegin(long long begin)
{
	m_begin = begin;
}

void AudioBuffer::setFreq(int freq)
{
	m_freq = freq;
}

long long AudioBuffer::getBegin()
{
	return m_begin;
}

long long AudioBuffer::getEnd()
{
	return m_begin+m_length;
}

long double AudioBuffer::getBeginSeconds()
{
	return (long double)(m_begin)/m_freq;
}

long double AudioBuffer::getEndSeconds()
{
	return (long double)(m_begin+m_length)/m_freq;
}

long long AudioBuffer::getNumSamples()
{
	return m_length;
}

int AudioBuffer::getFreq()
{
	return m_freq;
}

int AudioBuffer::getBytesInSample()
{
	return m_bytes_in_sample;
}
/*
int & AudioBuffer::getSample(long long nth)
{
	if(nth-m_begin<m_length && m_data){
		return m_data[nth-m_begin];
		//return ( (unsigned int*)m_filedata.data() )[nth-m_begin];
	}
	else{
		return m_null_sample;
	}
}
*/
void* AudioBuffer::getData(long long *size)
{
	if(size)*size=m_length*m_bytes_in_sample;
	return (void*)m_data;
	//if(size)*size=m_filedata.length();
	//return (void*)m_filedata.data();
}
