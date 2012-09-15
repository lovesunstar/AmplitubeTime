#include "audioplayer.h"
#include <iostream>
#include "config.h"

//
using namespace std;
//
bool al_check_error(bool print=true)
{
	ALenum err_code;
	bool no_err=true;

	while ((err_code = alGetError()) != AL_NO_ERROR){
		if(print) cout <<  "OpenAL error: " << (char *)alGetString(err_code) << endl;
		no_err = false;
	}

	return no_err;
}
//
AudioPlayer::AudioPlayer(char *device_name)
{
	
	m_al_device = alcOpenDevice(device_name);
	m_al_context = alcCreateContext(m_al_device, 0);
	alcMakeContextCurrent(m_al_context);
	
	const ALchar *pDeviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	if (pDeviceList)
	{
		cout << "\nAvailable Devices are:" << endl;

		while (*pDeviceList)
		{
			cout << pDeviceList << endl;
			pDeviceList += strlen(pDeviceList) + 1;
		}
	}
	cout << endl;
	
	cout << "Extensions: " << alcGetString(m_al_device, ALC_EXTENSIONS) << endl;
	cout << endl;
	
	m_playing = false;
	m_looped = false;
	
	//m_buffer_time=1.0;
	al_check_error();
	
	m_volume = 1.0;
	//m_equalizer_enabled = false;
	//memset(m_null_data, 0, );
	
	m_apu_chain = 0;
	m_data_buf = new short[AUDIO_BUFFER_SAMPLES];
}

AudioPlayer::~AudioPlayer()
{
	alDeleteSources(1, &m_al_source_id);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_al_context);
	alcCloseDevice(m_al_device);
	
	delete[] m_data_buf;
}
//
bool AudioPlayer::playing()
{
	return m_playing;
}
//
long long AudioPlayer::currentPosition()
{
	update();
	ALint current_buffer_offset;
	ALuint current_buffer;
	alGetSourcei(m_al_source_id, AL_BUFFER, (ALint*)&current_buffer);
	alGetSourcei(m_al_source_id, AL_SAMPLE_OFFSET, &current_buffer_offset);
	m_current_position = m_buf_pos[current_buffer] + current_buffer_offset;
	return m_current_position;
}
//
AudioSource * AudioPlayer::getAudioSource()
{
	return m_audio_source;
}
//
void AudioPlayer::timerEvent(QTimerEvent *event)
{
	if(m_playing){
		update();
	}
}
//
void AudioPlayer::setLooped(bool looped)
{
	m_looped = looped;
}
//
void AudioPlayer::pause()
{
	alSourcePause(m_al_source_id);
	m_playing = false;
}
//
void AudioPlayer::stop()
{
	if( alIsSource(m_al_source_id) ){
		alSourceStop(m_al_source_id);
		alDeleteSources(1, &m_al_source_id);
		alDeleteBuffers(2, m_bufs);
		
		m_apu_chain->reset();
	}
	
	m_playing = false;
	
	al_check_error();
}
//
void AudioPlayer::setVolume(float volume)
{
	m_volume = volume;
	alSourcef (m_al_source_id, AL_GAIN, m_volume);
}
//
void AudioPlayer::jump(float sec)
{
//	qDebug() << "jumping " << (long long)(sec*m_audio_source->getFreq());

	long long cursor = m_current_position + (long long)(sec*m_audio_source->getFreq());

	play(m_audio_source, m_first_sample, m_last_sample);

	m_source_cursor = cursor;
	if(m_source_cursor<m_first_sample) m_source_cursor = m_first_sample;
	if(m_source_cursor>m_last_sample) m_source_cursor = m_last_sample;
}
//
void AudioPlayer::setApuChain(ApuChain * apu_chain)
{
	m_apu_chain = apu_chain;
}
//

void AudioPlayer::queueBuffer(ALuint buf_id)
{
	int bps = m_audio_source->getBytesInSample();
	m_buf_pos[buf_id] = m_source_cursor;
	
	long long end_sample = qMin(m_last_sample, m_source_cursor + AUDIO_BUFFER_SAMPLES);
	//m_audio_source->getData(m_source_cursor, end_sample, m_data_buf);
	
	long long num_samples;
	
	char *data = (char*)m_audio_source->getDataPointer(m_source_cursor, end_sample, &num_samples);
	
	
	ALenum state;
	alGetSourcei(m_al_source_id, AL_SOURCE_STATE, &state);
	if(state!=AL_PLAYING){
		if(m_source_cursor>=m_last_sample){
			stop();
			
			if(m_looped){
				play(m_audio_source, m_first_sample, m_last_sample);
				return;
			}
		}
		else{
			alSourcePlay(m_al_source_id);
		}
	}
	
	//if(m_source_cursor>=m_last_sample) return;//no data, just wait playing
	if(num_samples<0){
		return;
	}

	//int data_size = (end_sample-m_source_cursor)*bps;
	
	
	if(!data){
		if(num_samples>AUDIO_BUFFER_SAMPLES || num_samples==0){
			num_samples = AUDIO_BUFFER_SAMPLES;
		}
		
		memset(m_data_buf, 0, num_samples * sizeof(short) );
	}else
	if(bps == 1){
		for(int i=0; i<num_samples; ++i){
			m_data_buf[i] = short(data[i]) << 8;
		}
	}else
	if(bps == 2){
		memcpy( m_data_buf, data, num_samples * bps );
	}else{
		qDebug() << "bps = " << bps;
	}
	
	
	if(m_apu_chain){
		long long start_time;
		start_time = (long long) (m_source_cursor-m_first_sample)*1000.0/m_audio_source->getFreq()+m_start_play_time;//+QDateTime::currentDateTime().time().msec();
		//qDebug() << "start_time " << start_time;
		m_apu_chain->setFreq(m_audio_source->getFreq());
		m_apu_chain->processData(m_data_buf, AUDIO_BUFFER_SAMPLES, start_time );
	}
	
	alBufferData(buf_id, AL_FORMAT_MONO16, m_data_buf, num_samples*bps, m_audio_source->getFreq() );
	alSourceQueueBuffers(m_al_source_id, 1, &buf_id);
	
	m_source_cursor += num_samples;
	
}
//
void AudioPlayer::update()
{
	if(!m_playing)return;
	
	if(m_current_position > m_last_sample){
		stop();
			
		if(m_looped){
			play(m_audio_source, m_first_sample, m_last_sample);
			return;
		}
	}
	
	int processed_num, queued_num;
	alGetSourcei(m_al_source_id, AL_BUFFERS_PROCESSED, &processed_num);
	alGetSourcei(m_al_source_id, AL_BUFFERS_QUEUED, &queued_num);
	
	ALuint buf_id;
	if( processed_num>0 ){
		alSourceUnqueueBuffers(m_al_source_id, 1, &buf_id);
		queueBuffer(buf_id);
	}
	
	if(processed_num+queued_num==0){
		queueBuffer(m_bufs[0]);
		queueBuffer(m_bufs[1]);
	}
	
	//TODO: if source not playing - stop() // WTF?
	
	al_check_error();
}
//
void AudioPlayer::play(AudioSource *au, long long begin, long long end) 
{
	//qDebug() << "AudioPlayer::play()";
	if(m_playing){
		stop();
	}
	
	if(end-begin < AUDIO_BUFFER_SAMPLES){
		return;
	}
	
	m_audio_source = au;
	m_source_cursor = begin;
	m_first_sample = begin;
	m_last_sample = end;
	m_current_position = begin;
	
	m_start_play_time = (long long) QDateTime::currentDateTime().toTime_t()*1000+QDateTime::currentDateTime().time().msec();
	
	alGenSources(1, &m_al_source_id);
	
	ALfloat al_pos[3];
	ALfloat al_vel[3];
	al_pos[0]=0.0;al_pos[1]=0.0;al_pos[2]=0.0;
	al_vel[0]=0.0;al_vel[1]=0.0;al_vel[2]=0.0;
	
	alSourcef (m_al_source_id, AL_PITCH,	1.0f);
	alSourcef (m_al_source_id, AL_GAIN,		m_volume);
	alSourcefv(m_al_source_id, AL_POSITION,	al_pos);
	alSourcefv(m_al_source_id, AL_VELOCITY,	al_vel);
	alSourcei (m_al_source_id, AL_LOOPING,	false);
	alSourcei (m_al_source_id, AL_SOURCE_RELATIVE,	AL_TRUE);
	alSourcef (m_al_source_id, AL_ROLLOFF_FACTOR,	0.0);
	
	alGenBuffers(2, m_bufs);
	//update();
	
	startTimer(50);
	m_playing = true;
	
	al_check_error();
}
