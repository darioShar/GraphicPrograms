#pragma once
#include <SFML\Audio.hpp>
#include <fftw3.h>
#include "data.h"

#define REFRESH_RATE 30.0f
#define BUFFER_SIZE getSampleRate() / REFRESH_RATE

#define NUM_BUFFER 3

#define FFT_PER_CHUNK_RECEIVED 30

#define IMPRECISION_TOLERANCE 0.010f

using namespace sf;

class CustomRecorder : public Music
{
	Clock _t;
	/* Audio */
	std::vector<d_real>						_window;
	d_real*									_in;
	fftwf_complex*							_out;
	fftwf_plan								_plan;
	d_real									_rms; // _rms will be calculated according to the last FFT passed on call to getFFT()

	std::vector<d_real>						_FFTNull;
	// Since class Music uses a triple buffer internally, we must do the same in order to catch all audio information.
	std::vector<std::vector<d_real>>		_FFTsBuffer[NUM_BUFFER];
	int										_currentFFTsBufferFill;
	int										_currentFFTsBufferRead;
	d_real									_FFTDuration;
	int										_FFTDone;

	int										_previousSampleCount;

protected :
	virtual bool onGetData(Chunk& data);
	using Music::play;
	void calculateRMSFromFFT(const std::vector<d_real>&);
	
public:
	CustomRecorder();
	~CustomRecorder();

	void playNewSong();
	const std::vector<d_real>& getFFT();
	d_real getRMS() const;
};
