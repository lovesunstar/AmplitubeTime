#ifndef EQUALIZER_H
#define EQUALIZER_H
//
#include <QObject>

/* Coefficient history for the IIR filter */
typedef struct
{
	float x[3]; /* x[n], x[n-1], x[n-2] */
	float y[3]; /* y[n], y[n-1], y[n-2] */
} sXYData;

typedef struct 
{
	float beta;
	float alpha; 
	float gamma;
} sIIRCoefficients;

#define EQ_CHANNELS 1
#define EQ_MAX_BANDS 10

//
class Equalizer : public QObject
{
Q_OBJECT 

	bool m_extra_filtering;
	
	sIIRCoefficients *m_iir_cf;
	float m_gain[EQ_MAX_BANDS][EQ_CHANNELS];
	float m_preamp;
	
	sXYData m_data_history[EQ_MAX_BANDS][EQ_CHANNELS];
	sXYData m_data_history2[EQ_MAX_BANDS][EQ_CHANNELS];
	
	int m_eqi, m_eqj, m_eqk;
	

public:
	Equalizer();
	
	int getBandFreq(int band);
	
	int equalizeBlock(void* pcmblock, unsigned long pcmblocksize);
	void reset();
	
public slots:
	void setBandLevel(int band, float factor);
	void setPreamp(float preamp);
	void setExtraFiltering(bool extra);
};
#endif
