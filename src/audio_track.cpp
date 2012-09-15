#include "audio_track.h"
//
AudioTrack::AudioTrack(AudioSource * source, QWidget * parent) 
	: Track(parent)
{
	m_source = source;
}
//
AudioTrack::~AudioTrack()
{
}
//

void AudioTrack::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, false);

	QRect envelope_rect(1, 1, width()-2, height()-2);
	qDrawShadePanel(&painter, envelope_rect, QPalette() );
	drawEnvelope(&painter, envelope_rect);
	drawSelection(&painter);

	painter.drawRect(0, 0, width()-1, height()-1);
}

void AudioTrack::drawEnvelope(QPainter * painter, QRect rect)
{
//	long long first_sample = m_source->timeToSamples(m_begin);
//	long long samples_num = m_source->timeToSamples(m_range);
//	signed int * samples = new signed int[samples_num];
//	m_source->fillData(samples, first_sample, samples_num);
//
//	for(int i=0; i<samples_num; ++i){
//	}
//
//	delete[] samples;
}

void AudioTrack::drawSelection(QPainter * painter)
{
}
