#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
//
#include <QtCore>
#include <AL/al.h>
#include <AL/alc.h>
#include "audiosource.h"
#include "apu_chain.h"
//

class AudioPlayer : public QObject 
{
Q_OBJECT

	ALCdevice *m_al_device;
	ALCcontext *m_al_context;
	ALuint m_al_source_id;
	ALuint m_bufs[2];
	
	short * m_data_buf; 
	
	bool m_playing;
	bool m_looped;
	float m_volume;
	AudioSource * m_audio_source;
	long long m_source_cursor;
	long long m_first_sample;
	long long m_last_sample;
	
	long long m_current_position;
	
	ApuChain * m_apu_chain;
	
	QMap<ALuint, long long> m_buf_pos;

	long long m_start_play_time;//msec

	void update();
	void queueBuffer(ALuint buf_id);
	
public:
	AudioPlayer(char *device_name=0);
	~AudioPlayer();
	bool playing();
	long long currentPosition();
	AudioSource * getAudioSource();
	
public slots:
	void play(AudioSource *au, long long begin, long long end);
	void pause();
	void stop();
	void setLooped(bool looped);
	void setVolume(float volume);
	void setApuChain(ApuChain * apu_chain);
	void jump(float sec);

protected:
	void timerEvent(QTimerEvent *event);
	
};
#endif
