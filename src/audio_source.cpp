#include "audio_source.h"
//
AudioSource::AudioSource( DataStream * stream, QObject * parent ) 
	: Source(parent)
{
	m_data_stream = stream;
}
//
AudioSource::~AudioSource()
{
}
//
