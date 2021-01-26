#pragma once
#include "utils.h"
#include "IPMatrix.h"
#include <cstdio>
#include <SFML\Graphics.hpp>

using namespace sf;
using namespace std;

extern vector<vector<float>> DefaultBlurMatrix;
extern vector<vector<float>> DefaultHorizontalEdgeMatrix;
extern vector<vector<float>> DefaultVerticalEdgeMatrix;

class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();

	bool loadImageToProcess(const string& sourceImagePath);
	bool saveProcessedImage(const string& destImagePath);
	const Image& getImage();

	bool setBlurMatrix(const IPMatrix& blurMatrix);
	bool setEdgeMatrices(const IPMatrix& horizontalEdgeMatrix
						,const IPMatrix& verticalEdgeMatrix);

	void setEdgeDetetctionThreshold(float value);

	void applyBrightnessFilter();
	void applyBlurFilter(bool showSteps);
	void applyEdgeFilter(bool showSteps);

protected :
	bool prepareBeforeFilter();
	void applyFilter(IPMatrix matrixMask, bool showSteps);

	void createWindow();
	void updateWindow(const Image& image);
	void closeWindow();

private :
	Image m_image;
	Image m_imageBuffer;

	IPMatrix m_blurMatrix;
	IPMatrix m_horizontalEdgeMatrix;
	IPMatrix m_verticalEdgeMatrix;

	float m_edgeDetectionThreshold;

	// To show processing steps
	RenderWindow m_window;
	Event m_evnt;
	Texture m_texture;
	Sprite m_sprite;
};