#include "FrequencySpectrum.h"

FrequencySpectrum::FrequencySpectrum(int numBins, int width, int height)
	:	_width(width)
	,	_height(height)
	,	_fftBins(numBins)
	,	_resizingFactor(1.0f)
{
	setGraph(numBins, GraphScale::Linear);
	_shader.loadFromFile(SHADER_PATH, Shader::Fragment);
}

FrequencySpectrum::~FrequencySpectrum()
{
}

void FrequencySpectrum::setGraph(int numBins, GraphScale graphScale) {
	if (numBins == 0) {
		_fftBins.clear();
	}
	else {
		_fftBins.resize(numBins);
		if (graphScale == GraphScale::Linear) {
			for (int i = 0; i < numBins; i++) {
				_fftBins[i].setPosition(Vector2f((d_real)i * (d_real)_width / (d_real)numBins, _height));
				_fftBins[i].setSize(Vector2f((d_real)_width / (d_real)numBins - DEFAULT_SPACE, 0.0f));
				_fftBins[i].setFillColor(Color::White);
			}
		}
		else {
			// Position for bin x determined by log10(10*x) here.

			// Because log function is too brutal on first terms, lets define an index offset and a position offset
			int indexOffset = 20;
			d_real posOffset;
			// We want everything filled : resizing factor then is
			d_real resizingFactor1 = (d_real)_width / log10(10*(numBins + indexOffset));
			posOffset = log10f(10 * indexOffset)*resizingFactor1;
			d_real resizingFactor2 = (d_real)_width / ((d_real)_width - posOffset);
			printf("numBin is %d, log10(10*numBins) is %f ; resizingFactor is %f \n", numBins, log10(10 * numBins), resizingFactor1);
			// Setting position
			for (int i = 0; i < numBins; i++) {
				_fftBins[i].setPosition(Vector2f((log10f(10 * (i + indexOffset))*resizingFactor1 - posOffset)*resizingFactor2, _height));
				_fftBins[i].setFillColor(Color::White);
			}

			// Setting size
			for (int i = 0; i < numBins - 1; i++) {
				_fftBins[i].setSize(Vector2f(_fftBins[i + 1].getPosition().x - _fftBins[i].getPosition().x, 0.0f));
			}
			_fftBins[numBins - 1].setSize(Vector2f(_width - _fftBins[numBins - 1].getPosition().x, 0.0f));

			for (int i = 0; i < numBins; i++) {
				printf("FFT bin %d position is %f , size is %f\n", i, _fftBins[i].getPosition().x, _fftBins[i].getSize().x);
			}
		}
	}
}


void FrequencySpectrum::scale(d_real factor) {
	_resizingFactor *= factor;
}

void FrequencySpectrum::resetScale() {
	_resizingFactor = 1.0f;
}

d_real FrequencySpectrum::getScale() const {
	return _resizingFactor;
}


void FrequencySpectrum::setFFT(std::vector<d_real> const& fft, GraphType graphType, bool logScale) {
	d_real (*fftFunc)(const d_real&) = logScale ? amplitude_to_dB : identity<d_real>;

	int size = std::min(fft.size(), _fftBins.size());
	if (graphType == GraphType::TotalTypes) printf("Wrong value passed for graph type (GraphType::TotalTypes). Default Type is Downward (last value of enum).\n");

	if (graphType == GraphType::Centred) {
		// No smoothing with type centred
		for (int i = 1; i < size; i++) {
			d_real binHeight = fftFunc(fft[i]) * (d_real)_height * _resizingFactor;
			// Now setting fft
			_fftBins[i].setSize(Vector2f(_fftBins[i].getSize().x, -binHeight));
			_fftBins[i].setPosition(_fftBins[i].getPosition().x, ((_height + binHeight) / 2.0f));
		}
	}
	else if (graphType == GraphType::Upward) {
		for (int i = 1; i < size; i++) {
			#ifdef SMOOTH_FFT
			d_real binHeight = fftFunc(fft[i]) * (d_real)_height * _resizingFactor;
			binHeight = (binHeight + (-_fftBins[i].getSize().y)) / 2.0f;
			#else
			d_real binHeight = amplitude_to_dB(fft[i]) * (d_real)_height * _resizingFactor;
			#endif
			_fftBins[i].setSize(Vector2f(_fftBins[i].getSize().x, -binHeight));
			_fftBins[i].setPosition(_fftBins[i].getPosition().x, _height * 7 / 9);
		}
	}
	else {
		for (int i = 1; i < size; i++) {
			#ifdef SMOOTH_FFT
			d_real binHeight = fftFunc(fft[i]) * (d_real)_height * _resizingFactor;
			binHeight = (binHeight + _fftBins[i].getSize().y) / 2.0f;
			#else
			d_real amplitude = amplitude_to_dB(fft[i]) * (d_real)_height * _resizingFactor;
			#endif
			_fftBins[i].setSize(Vector2f(_fftBins[i].getSize().x, binHeight));
			_fftBins[i].setPosition(_fftBins[i].getPosition().x, _height * 2 / 9);
		}
	}
}

void FrequencySpectrum::draw(RenderWindow& window) {
	float maxHeight = 0.0f;
	int maxIndex = 0;
	for (int i = 0; i < _fftBins.size(); i++) {
		if (-_fftBins[i].getSize().y > maxHeight) {
			maxIndex = i;
			maxHeight = -_fftBins[i].getSize().y;
		}
	}

	_shader.setUniform("maxHeight", maxHeight / (d_real)_height);
	_shader.setUniform("strongestFrequency", (d_real)maxIndex / (d_real)_fftBins.size());


	for (int i = 1; i < _fftBins.size() - 1; i++) {
		_shader.setUniform("height", -(d_real)_fftBins[i].getSize().y / _height);
		_shader.setUniform("frequency", (d_real)i / (d_real)_fftBins.size()); // Sort of
		_shader.setUniform("previousHeight", -(d_real)_fftBins[i - 1].getSize().y / (d_real)_height);
		_shader.setUniform("nextHeight", -(d_real)_fftBins[i + 1].getSize().y / (d_real)_height);

		window.draw(_fftBins[i], &_shader);
	}
}

void FrequencySpectrum::reloadShader() {
	_shader.loadFromFile(SHADER_PATH, Shader::Fragment);
}


inline d_real amplitude_to_dB(const d_real &amplitude) {
	return std::max((20 * log10f(amplitude) + FFT_DB_OFFSET) / 80.0f, 0.0f);
}