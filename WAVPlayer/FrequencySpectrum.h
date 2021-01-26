#pragma once
#include <SFML\Graphics.hpp>
#include "data.h"

#define SMOOTH_FFT
#define DEFAULT_SPACE 1.0f
#define SHADER_PATH "Shaders\\fft.frag"

#define FFT_DB_OFFSET 50.0f

using namespace sf;

inline d_real amplitude_to_dB(const d_real&);

template<typename T>
inline T identity(const T& d) { return d; }


enum GraphScale : bool {
	Linear,
	Logarithmic
};

enum GraphType {
	Centred,
	Upward,
	Downward,
	TotalTypes
};

class FrequencySpectrum
{
	int _width;
	int _height;
	std::vector<RectangleShape> _fftBins;
	d_real _resizingFactor;
	Shader _shader;

public:
	FrequencySpectrum(int, int, int);
	~FrequencySpectrum();

	void setGraph(int, GraphScale);

	void reloadShader();

	void scale(d_real);
	void resetScale();
	d_real getScale() const;
	void setFFT(std::vector<d_real> const&, GraphType, bool logScale = true);
	void draw(RenderWindow& window);
};

