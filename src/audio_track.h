#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H
//
#include <QtGui>
#include "track.h"
#include "audio_source.h"
//
class AudioTrack : public Track
{
Q_OBJECT
	AudioSource * m_source;

	void drawEnvelope(QPainter * painter, QRect rect);
	void drawSelection(QPainter * painter);

protected:
	void paintEvent(QPaintEvent *event);

public:
	AudioTrack(AudioSource * source, QWidget * parent=0);
	virtual ~AudioTrack();

};
#endif
