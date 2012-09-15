#include "equalizer.h"
#include <string.h>
#include <math.h>

static sIIRCoefficients iir_cf10[]
= {
	{ (float)(9.9723137967e-01), (float)(1.3843101653e-03), (float)(1.9972115835e+00) },	//	 31.2 Hz
	{ (float)(9.9406142155e-01), (float)(2.9692892229e-03), (float)(1.9939823631e+00) },	//	 62.5 Hz
	{ (float)(9.8774277725e-01), (float)(6.1286113769e-03), (float)(1.9874275518e+00) },	//   125.0 Hz
	{ (float)(9.7522112569e-01), (float)(1.2389437156e-02), (float)(1.9739682661e+00) },	//   250.0 Hz
	{ (float)(9.5105628526e-01), (float)(2.4471857368e-02), (float)(1.9461077269e+00) },	//   500.0 Hz
	{ (float)(9.0450844499e-01), (float)(4.7745777504e-02), (float)(1.8852109613e+00) },	//  1000.0 Hz
	{ (float)(8.1778971701e-01), (float)(9.1105141497e-02), (float)(1.7444877599e+00) },	//  2000.0 Hz
	{ (float)(6.6857185264e-01), (float)(1.6571407368e-01), (float)(1.4048592171e+00) },	//  4000.0 Hz
	{ (float)(4.4861333678e-01), (float)(2.7569333161e-01), (float)(6.0518718075e-01) },	//  8000.0 Hz
	{ (float)(2.4201241845e-01), (float)(3.7899379077e-01), (float)(-8.0847117831e-01) },	// 16000.0 Hz
};

static int eq_band_freq10[]
= {
	31, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000
};

static sIIRCoefficients iir_cf31[]
= {
	{ (float)(9.9939854113e-01), (float)(3.0072943419e-04), (float)(1.9993904238e+00) },	//	  20.0 Hz
	{ (float)(9.9940551149e-01), (float)(2.9724425274e-04), (float)(1.9993928282e+00) },	//	  25.0 Hz
	{ (float)(9.9925718616e-01), (float)(3.7140692231e-04), (float)(1.9992370516e+00) },	//	  31.5 Hz
	{ (float)(9.9879758671e-01), (float)(6.0120664727e-04), (float)(1.9987651273e+00) },	//	  40.0 Hz
	{ (float)(9.9849722245e-01), (float)(7.5138877401e-04), (float)(1.9984465124e+00) },	//	  50.0 Hz
	{ (float)(9.9820192714e-01), (float)(8.9903643123e-04), (float)(1.9981214319e+00) },	//	  63.0 Hz
	{ (float)(9.9759663784e-01), (float)(1.2016810790e-03), (float)(1.9974668794e+00) },	//	  80.0 Hz
	{ (float)(9.9699670283e-01), (float)(1.5016485839e-03), (float)(1.9967940174e+00) },	//   100.0 Hz
	{ (float)(9.9608832771e-01), (float)(1.9558361473e-03), (float)(1.9957717788e+00) },	//   125.0 Hz
	{ (float)(9.9488106021e-01), (float)(2.5594698955e-03), (float)(1.9943627488e+00) },	//   160.0 Hz
	{ (float)(9.9368508558e-01), (float)(3.1574572116e-03), (float)(1.9928757293e+00) },	//   200.0 Hz
	{ (float)(9.9187386982e-01), (float)(4.0630650882e-03), (float)(1.9906104476e+00) },	//   250.0 Hz
	{ (float)(9.8977837468e-01), (float)(5.1108126620e-03), (float)(1.9877748003e+00) },	//   315.0 Hz
	{ (float)(9.8709369779e-01), (float)(6.4531511061e-03), (float)(1.9838676311e+00) },	//   400.0 Hz
	{ (float)(9.8381454728e-01), (float)(8.0927263624e-03), (float)(1.9787829025e+00) },	//   500.0 Hz
	{ (float)(9.7966277109e-01), (float)(1.0168614455e-02), (float)(1.9716932308e+00) },	//   630.0 Hz
	{ (float)(9.7404386878e-01), (float)(1.2978065611e-02), (float)(1.9612347544e+00) },	//   800.0 Hz
	{ (float)(9.6758591875e-01), (float)(1.6207040627e-02), (float)(1.9476493009e+00) },	//  1000.0 Hz
	{ (float)(9.5973086514e-01), (float)(2.0134567431e-02), (float)(1.9287337831e+00) },	//  1250.0 Hz
	{ (float)(9.4878629133e-01), (float)(2.5606854334e-02), (float)(1.8983695001e+00) },	//  1600.0 Hz
	{ (float)(9.3627068388e-01), (float)(3.1864658062e-02), (float)(1.8581910087e+00) },	//  2000.0 Hz
	{ (float)(9.2117604381e-01), (float)(3.9411978094e-02), (float)(1.8005882747e+00) },	//  2500.0 Hz
	{ (float)(9.0172987031e-01), (float)(4.9135064847e-02), (float)(1.7133994083e+00) },	//  3150.0 Hz
	{ (float)(8.7697241507e-01), (float)(6.1513792463e-02), (float)(1.5803227133e+00) },	//  4000.0 Hz
	{ (float)(8.4900435915e-01), (float)(7.5497820425e-02), (float)(1.3993428287e+00) },	//  5000.0 Hz
	{ (float)(8.1426291228e-01), (float)(9.2868543858e-02), (float)(1.1311744237e+00) },	//  6300.0 Hz
	{ (float)(7.7194867684e-01), (float)(1.1402566158e-01), (float)(7.4026698287e-01) },	//  8000.0 Hz
	{ (float)(7.2627335006e-01), (float)(1.3686332497e-01), (float)(2.5120594308e-01) },	// 10000.0 Hz
	{ (float)(6.7690084582e-01), (float)(1.6154957709e-01), (float)(-3.4981568641e-01) },	// 12500.0 Hz
	{ (float)(6.2492532874e-01), (float)(1.8753733563e-01), (float)(-1.0577231561e+00) },	// 16000.0 Hz
	{ (float)(6.1776462189e-01), (float)(1.9111768906e-01), (float)(-1.5492495659e+00) },	// 20000.0 Hz
};


//
Equalizer::Equalizer(  ) :
	QObject()
{
	if(EQ_MAX_BANDS == 10){
		m_iir_cf = iir_cf10;
	}
	if(EQ_MAX_BANDS == 31){
		m_iir_cf = iir_cf31;
	}
	
	m_extra_filtering = false;
	for(int i=0; i<EQ_MAX_BANDS; ++i){
		setBandLevel(i, 0.5);
	}
	
	m_preamp = 1.0;
	
	reset();
}
//

void Equalizer::reset()
{
	m_eqi = 0;
	m_eqj = 2;
	m_eqk = 1;
	
	memset(m_data_history, 0, sizeof(sXYData) * EQ_MAX_BANDS * EQ_CHANNELS);
	memset(m_data_history2, 0, sizeof(sXYData) * EQ_MAX_BANDS * EQ_CHANNELS);
}

void Equalizer::setExtraFiltering(bool extra)
{
	m_extra_filtering = extra;
}

void Equalizer::setBandLevel(int band, float factor)
{
	float val;
	
	//val = -0.2f + (factor / 5.125903437963185f);// (1.2 * factor) / 6.151084125555822;
	val = factor;
	
	for(int i=0; i<EQ_CHANNELS; ++i){
		m_gain[band][i] = val;
	}
}

void Equalizer::setPreamp(float preamp)
{
	m_preamp = preamp;
}

int Equalizer::getBandFreq(int band)
{
	return eq_band_freq10[band];
}

int Equalizer::equalizeBlock( void * pcmblock, unsigned long pcmblocksize ) 
{
	
	int length = pcmblocksize;
	short *data = (short *) pcmblock;
	/* Indexes for the history arrays
	 * These have to be kept between calls to this function
	 * hence they are static */
	//n  static magint i = 0, j = 2, k = 1;	

	int index, band, channel, ic;
	int tempgint, halflength;
	float out[EQ_CHANNELS], pcm[EQ_CHANNELS];

	/**
	 * IIR filter equation is
	 * y[n] = 2 * (alpha*(x[n]-x[n-2]) + gamma*y[n-1] - beta*y[n-2])
	 *
	 * NOTE: The 2 factor was introduced in the coefficients to save
	 * 			a multiplication
	 *
	 * This algorithm cascades two filters to get nice filtering
	 * at the expense of extra CPU cycles
	 */
	/* 16bit, 2 bytes per sample, so divide by two the length of
	 * the buffer (length is in bytes)
	 */
	halflength = (length >> 1);
	for(index = 0; index < halflength; index+=2)
	{
		/* For each channel */
		for(channel = 0; channel < EQ_CHANNELS; channel++)
		{
			ic = index + channel;
			/* No need to scale when processing the PCM with the filter */
			pcm[channel] = data[ic];
			/* Preamp gain */
			//n pcm[channel] *= preamp[channel];

			out[channel] = 0;
			
			/* For each band */
			for(band = 0; band < EQ_MAX_BANDS; band++)
			{
				/* Store Xi(n) */
				m_data_history[band][channel].x[m_eqi] = pcm[channel];
				/* Calculate and store Yi(n) */
				m_data_history[band][channel].y[m_eqi] = 
               		   (
               	/* 		= alpha * [x(n)-x(n-2)] */
						m_iir_cf[band].alpha * ( m_data_history[band][channel].x[m_eqi]
               			-  m_data_history[band][channel].x[m_eqk])
               	/* 		+ gamma * y(n-1) */
               			+ m_iir_cf[band].gamma * m_data_history[band][channel].y[m_eqj]
               	/* 		- beta * y(n-2) */
               			- m_iir_cf[band].beta * m_data_history[band][channel].y[m_eqk]
						);
				/* 
				 * The multiplication by 2.0 was 'moved' into the coefficients to save
				 * CPU cycles here */
				 /* Apply the gain  */
				out[channel] += m_data_history[band][channel].y[m_eqi] * m_gain[band][channel]*m_preamp; // * 2.0;
			} /* For each band */

			if(m_extra_filtering)
			{
				/* Filter the sample again */
				for(band = 0; band < EQ_MAX_BANDS; band++)
				{
					/* Store Xi(n) */
					m_data_history2[band][channel].x[m_eqi] = out[channel];
					/* Calculate and store Yi(n) */
					m_data_history2[band][channel].y[m_eqi] = 
            	   		   (
	               	/* y(n) = alpha * [x(n)-x(n-2)] */
							m_iir_cf[band].alpha * (m_data_history2[band][channel].x[m_eqi]
            	   			-  m_data_history2[band][channel].x[m_eqk])
               		/* 	    + gamma * y(n-1) */
	               			+ m_iir_cf[band].gamma * m_data_history2[band][channel].y[m_eqj]
    	           	/* 		- beta * y(n-2) */
        	       			- m_iir_cf[band].beta * m_data_history2[band][channel].y[m_eqk]
							);
					/* Apply the gain */
					out[channel] +=  m_data_history2[band][channel].y[m_eqi]*m_gain[band][channel]*m_preamp;
				} /* For each band */
			}

			/* Volume stuff
			   Scale down original PCM sample and add it to the filters 
			   output. This substitutes the multiplication by 0.25
			 */

			out[channel] += (data[ic] >> 2);

			/* Round and convert to integer */
#if (0) // fails with GCC4 ifdef PPC
			tempgint = round_ppc(out[channel]); // in util.s
#else
 #ifdef __MAWIN__ //n X86
			tempgint = round_trick(out[channel]);
 #else
			tempgint = (int)lroundf(out[channel]);
 #endif
#endif

			/* Limit the output */
			if(tempgint < -32768)
				data[ic] = -32768;
			else if(tempgint > 32767)
				data[ic] = 32767;
			else 
				data[ic] = (short)tempgint;

		} /* For each channel */

		m_eqi++; m_eqj++; m_eqk++;
		
		/* Wrap around the indexes */
		if(m_eqi == 3) m_eqi = 0;
		else if(m_eqj == 3) m_eqj = 0;
		else m_eqk = 0;

		
	}/* For each pair of samples */
	
	return length;

}
