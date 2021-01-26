#include "CustomRecorder.h"


CustomRecorder::CustomRecorder()
	:	_FFTNull(200, 0.0f)
	,	_currentFFTsBufferFill(0)
	,	_currentFFTsBufferRead(0)
	,	_FFTDone(0)
	,	_FFTDuration(1.0f)
	,	_rms(0)
{
	_t.restart();
	for (auto& ffts : _FFTsBuffer) {
		ffts.resize(FFT_PER_CHUNK_RECEIVED);
		for (auto& fft : ffts) {
			fft.reserve(getSampleRate() / 2);
		}
	}
}

CustomRecorder::~CustomRecorder() {
	// Make sure to stop the recording thread
	stop();
}

void CustomRecorder::playNewSong() {
	_FFTDone = 0;
	_currentFFTsBufferRead = 0;
	play();
}

/* THE ANCIENT PROBLEM CODE*/
/*

// it seems that sometimes we are being passed a buffer containing the two next seconds to be played. But always overwriting 1st second. That is a problem.
// Thus we must only analyze 2nd second.
// fft duration in seconds. Should always be the same, program want handle otherwise.
_FFTDuration = (1.0f / ((d_real)getSampleRate() / (d_real)sampleCount));
// If it is the problem case
if (std::abs(_FFTDuration - _t.restart().asSeconds()) > IMPRECISION_TOLERANCE) {
// For now only dividing by two
_FFTDuration /= 2.0f;
// Also readjusting pointer so that it point to the right second to be analyzed.
samples += sampleCount;
}
//printf("Sample Rate is %d, samplecount is %d. FFT Duration is %f\n", getSampleRate(), sampleCount, _FFTDuration);

*/



bool CustomRecorder::onGetData(Chunk& data) {

	if (!Music::onGetData(data)) 
		return false;


	// Choosing right buffer
	auto& frequencySpectrums = _FFTsBuffer[_currentFFTsBufferFill++];
	_currentFFTsBufferFill %= NUM_BUFFER;

	// Setting up variables to analyze
	auto channelCount = getChannelCount();
	std::size_t sampleCount = data.sampleCount;
	sampleCount /= FFT_PER_CHUNK_RECEIVED;
	sampleCount /= channelCount;
	d_int* samples = (d_int*)data.samples;

	// FFT duration in seconds. Should always be the same, program want handle otherwise.
	_FFTDuration = (1.0f / ((d_real)getSampleRate() / (d_real)sampleCount));

	/* WINDOW FUNCTION */
	if (_previousSampleCount != sampleCount) {
		_window.resize(sampleCount);
		for (int i = 0; i < sampleCount; ++i) {
			_window[i] = static_cast<d_real>(0.5f * (1.0f - cosf(static_cast<d_real>(2.0f * M_PI * i) / static_cast<d_real>((sampleCount) - 1))));
		}
	}

	/* FFT */
	_in = (d_real*)fftwf_malloc(sizeof(d_real)*sampleCount);
	_out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*sampleCount);
	for (int fs = 0; fs < FFT_PER_CHUNK_RECEIVED; fs++) {
		for (int i = 0; i < sampleCount; i++) {
			// Also applying window function
			// We take the average of two channel samples.
			_in[i] = ((samples[(fs*sampleCount*channelCount) + (i*channelCount)] + samples[(fs*sampleCount*channelCount) + (i*channelCount + channelCount - 1)]) / 2.0f) * _window[i];
		}

		_plan = fftwf_plan_dft_r2c_1d(sampleCount, _in, _out, FFTW_ESTIMATE);
		fftwf_execute(_plan);

		// To scale results
		d_real reducingFactor = 2.0f / (d_real)sampleCount;
		// Because of window function
		reducingFactor *= (1.0f / 0.5f);

		frequencySpectrums[fs].resize(sampleCount / 2);
		for (int i = 0; i < sampleCount / 2; i++) {
			d_real value = sqrtf(_out[i][0] * _out[i][0] + _out[i][1] * _out[i][1])*reducingFactor / (d_real)std::numeric_limits<d_int>::max();
			frequencySpectrums[fs][i] = value;
		}
	}

	_FFTDone += FFT_PER_CHUNK_RECEIVED;

	_previousSampleCount = sampleCount;

	fftwf_free(_in);
	fftwf_free(_out);
	fftwf_destroy_plan(_plan);
	return true;
}


const std::vector<d_real>& CustomRecorder::getFFT() {
	if (getStatus() != Music::Status::Playing || _FFTDone < FFT_PER_CHUNK_RECEIVED) {
		printf("Null FFT has been passed.\n");
		return _FFTNull;
	}
	// PUTAIN MAIS POURQUOI FOIS DEUX ?????? JE COMPRENDS PUTAIN DE PAS !!!!!!!
	printf("Playing offset : %f, FFT Duration : %f\n", getPlayingOffset().asSeconds(), _FFTDuration);
	int numFFTtoGet = /*2**/ getPlayingOffset().asSeconds() / _FFTDuration; // numero de la FFT a récupérer depuis le début du morceau
	int numFFTBegin = _FFTDone - NUM_BUFFER * FFT_PER_CHUNK_RECEIVED; // numéro de la FFT la plus ancienne du triple buffer
	if (numFFTtoGet < numFFTBegin) {
		printf("Problem : numFFTtoGet is %d, numFFTBegin is %d\n", numFFTtoGet, numFFTBegin); 
		return _FFTNull;
	}
	if (numFFTtoGet >= _FFTDone) {
		printf("Problem : numFFTtoGet is %d, _FFTDone is %d\n", numFFTtoGet, _FFTDone);
		return _FFTNull;
	}
	int buffertoGo = (numFFTtoGet - numFFTBegin) / FFT_PER_CHUNK_RECEIVED;
	int FFTtoGo = (numFFTtoGet - numFFTBegin) % FFT_PER_CHUNK_RECEIVED;
	//printf("Total of %d FFTs. FFT to get : %d. FFT Begin : %d. Have to go to buffer %d, FFT %d.\n", _FFTDone, numFFTtoGet, numFFTBegin, buffertoGo, FFTtoGo);
	if (&_FFTsBuffer[buffertoGo][FFTtoGo]) {
		calculateRMSFromFFT(_FFTsBuffer[buffertoGo][FFTtoGo]);
		return _FFTsBuffer[buffertoGo][FFTtoGo];
	}
	else {
		// WARNING : be careful not to divide by this value.
		_rms = 0.0f;
		return _FFTNull;
	}
}

void CustomRecorder::calculateRMSFromFFT(const std::vector<d_real>& fft) {
	_rms = 0.0f;
	for (auto& x : fft) _rms += x*x;
	_rms *= 2.0f;
	_rms = sqrtf(_rms);
}

d_real CustomRecorder::getRMS() const {
	return _rms;
}
