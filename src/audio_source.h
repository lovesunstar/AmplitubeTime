#ifndef __AUDIO_SOURCE_H
#define __AUDIO_SOURCE_H
//
#include <QtGui>
#include "source.h"
#include "data_stream.h"
//
class AudioSource : public Source
{
Q_OBJECT
	DataStream * m_data_stream;

protected:

public:
	AudioSource(DataStream * stream, QObject * parent=0);
	virtual ~AudioSource();
	
	
};
#endif
