#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <mutex>
#include "data.h"
#include "fftw3.h"

// custom audio stream that plays a loaded buffer
class CustomPlayer : public sf::SoundStream
{
public:
	CustomPlayer(const sf::SoundBuffer& buffer, float refreshRate = 30.0f);
	CustomPlayer(float refreshRate = 30.0f);
	~CustomPlayer();
	void load(const sf::SoundBuffer& buffer);
	const std::vector<d_real>& getFFT();
	d_real getRMS() const;

private:
	virtual bool onGetData(Chunk& data);
	virtual void onSeek(sf::Time timeOffset);


	/* Audio */
	std::vector<d_real>						_window;
	d_real*									_in;
	fftwf_complex*							_out;
	fftwf_plan								_plan;
	d_real									_rms; // _rms will be calculated according to the last FFT passed on call to getFFT()
	int										_previousSampleCount;

	std::vector<d_real>						_FFTNull;
	std::vector<d_real>						_FFT;

	float _refreshRate;
	std::vector<sf::Int16> _samples;
	std::size_t _currentSample;
	std::size_t _samplesToStream;
};
