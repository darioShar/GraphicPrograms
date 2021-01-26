#include "CustomPlayer.h"


CustomPlayer::CustomPlayer(const sf::SoundBuffer& buffer, float refreshRate)
	:	_refreshRate(refreshRate)
	,	_rms(0.0f)
{
	load(buffer);
}

CustomPlayer::CustomPlayer(float refreshRate)
	: _refreshRate(refreshRate)
	, _rms(0.0f)
{
}

CustomPlayer::~CustomPlayer() {

}


void CustomPlayer::load(const sf::SoundBuffer& buffer)
{
	// extract the audio samples from the sound buffer to our own container
	_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

	// reset the current playing position 
	_currentSample = 0;

	// Defining how many samples to stream according to refresh rate
	_samplesToStream = (buffer.getSampleRate() / _refreshRate) * buffer.getChannelCount();

	// initialize the base class
	initialize(buffer.getChannelCount(), buffer.getSampleRate());
}

bool CustomPlayer::onGetData(Chunk& data)
{
	// set the pointer to the next audio samples to be played
	data.samples = &_samples[_currentSample];

	// have we reached the end of the sound?
	if (_currentSample + _samplesToStream <= _samples.size())
	{
		// end not reached: stream the samples and continue
		data.sampleCount = _samplesToStream;
		_currentSample += _samplesToStream;

		// Resetting rms
		_rms = 0.0f;

		// Setting up variables to analyze
		auto channelCount = getChannelCount();
		std::size_t sampleCount = data.sampleCount;
		sampleCount /= channelCount;
		d_int* samples = (d_int*)data.samples;

		/* WINDOW FUNCTION */
		if (_previousSampleCount != sampleCount) {
			_window.resize(sampleCount);
			for (int i = 0; i < sampleCount; ++i) {
				_window[i] = static_cast<d_real>(0.5f * (1.0f - cosf(static_cast<d_real>(2.0f * M_PI * i) / static_cast<d_real>((sampleCount)-1))));
			}
		}

		/* FFT AND RMS*/
		_in = (d_real*)fftwf_malloc(sizeof(d_real)*sampleCount);
		_out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*sampleCount);

		for (int i = 0; i < sampleCount; i++) {
			// Also applying window function and RMS
			// We take one channel
			_in[i] = samples[i*channelCount] * _window[i];
			_rms += (static_cast<d_real>(samples[i*channelCount]) / static_cast<d_real>(std::numeric_limits<d_int>::max()))
				   *(static_cast<d_real>(samples[i*channelCount]) / static_cast<d_real>(std::numeric_limits<d_int>::max()));
		}

		_plan = fftwf_plan_dft_r2c_1d(sampleCount, _in, _out, FFTW_ESTIMATE);
		fftwf_execute(_plan);

		// To scale results, taking into account window function
		d_real reducingFactor = 2.0f / (d_real)sampleCount;
		reducingFactor *= (1.0f / 0.5f);
		_FFT.resize(sampleCount / 2);
		for (int i = 0; i < sampleCount / 2; i++) {
			d_real value = sqrtf(_out[i][0] * _out[i][0] + _out[i][1] * _out[i][1])*reducingFactor / (d_real)std::numeric_limits<d_int>::max();
			_FFT[i] = value;
		}

		_rms /= sampleCount;
		_rms = sqrtf(_rms);

		_previousSampleCount = sampleCount;

		// Free ressources
		fftwf_free(_in);
		fftwf_free(_out);
		fftwf_destroy_plan(_plan);

		return true;
	}
	else
	{
		// end of stream reached: stream the remaining samples and stop playback
		data.sampleCount = _samples.size() - _currentSample;
		_currentSample = _samples.size();
		return false;
	}
}

void CustomPlayer::onSeek(sf::Time timeOffset)
{
	// compute the corresponding sample index according to the sample rate and channel count
	_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
}


const std::vector<d_real>& CustomPlayer::getFFT() {
	return _FFT;
}

d_real CustomPlayer::getRMS() const {
	return _rms;
}