#include "stdafx.h"
#include "ImageProcessor.h"

vector<vector<float>> DefaultBlurMatrix
{
	{ 1, 2, 1 },
	{ 2, 4, 2 },
	{ 1, 2, 1 }
};

vector<vector<float>> DefaultHorizontalEdgeMatrix
{
	{ 1,  2, 0,  -2, -1 },
	{ 4,  8, 0,  -8, -4 },
	{ 6, 12, 0, -12, -6 },
	{ 4,  8, 0,  -8, -4 },
	{ 1,  2, 0,  -2, -1 }
};

vector<vector<float>> DefaultVerticalEdgeMatrix
{
	{ -1, -4,  -6, -4, -1 },
	{ -2, -8, -12, -8, -2 },
	{ 0,  0,   0,  0,  0 },
	{ 2, 8, 12, 8, 2 },
	{ 1, 4,  6, 4, 1 }
};

ImageProcessor::ImageProcessor()
	:	m_blurMatrix(DefaultBlurMatrix)
	,	m_horizontalEdgeMatrix(DefaultHorizontalEdgeMatrix)
	,	m_verticalEdgeMatrix(DefaultVerticalEdgeMatrix)
	,	m_edgeDetectionThreshold(100.0f)
{
	DefaultBlurMatrix = {
		{ 1, 2, 1 },
		{ 2, 4, 2 },
		{ 1, 2, 1 }
	};

	DefaultHorizontalEdgeMatrix =
	{
		{ 1,  2, 0,  -2, -1 },
		{ 4,  8, 0,  -8, -4 },
		{ 6, 12, 0, -12, -6 },
		{ 4,  8, 0,  -8, -4 },
		{ 1,  2, 0,  -2, -1 }
	};

	DefaultVerticalEdgeMatrix = 
	{
		{ -1, -4,  -6, -4, -1 },
		{ -2, -8, -12, -8, -2 },
		{ 0,  0,   0,  0,  0 },
		{ 2, 8, 12, 8, 2 },
		{ 1, 4,  6, 4, 1 }
	};
	m_blurMatrix = IPMatrix(DefaultBlurMatrix);
	m_horizontalEdgeMatrix = IPMatrix(DefaultHorizontalEdgeMatrix);
	m_verticalEdgeMatrix = IPMatrix(DefaultVerticalEdgeMatrix);

	m_blurMatrix.normalize();
	m_horizontalEdgeMatrix.normalize() * 0.1f;
	m_verticalEdgeMatrix.normalize() * 0.1f;
}


ImageProcessor::~ImageProcessor()
{
}


bool ImageProcessor::loadImageToProcess(const string& sourceImagePath) {
	if (!m_image.loadFromFile(sourceImagePath)) {
		printf("Failed to load image from path %s...\n", sourceImagePath.c_str());
		return false;
	}
	printf("Loaded image from %s...\n", sourceImagePath.c_str());
	return true;
}

bool ImageProcessor::saveProcessedImage(const string& destImagePath) {
	if (!m_image.saveToFile(destImagePath)) {
		printf("Failed to save processed image to %s\n\n", destImagePath.c_str());
		return false;
	}
	printf("Saved processed image to %s\n\n", destImagePath.c_str());
	return true;
}

const Image& ImageProcessor::getImage() {
	return m_image;
}


bool ImageProcessor::setBlurMatrix(const IPMatrix& blurMatrix) {
	if (!blurMatrix.isSquare()) {
		printf("Failed to set new blur matrix : non-square\n");
		return false;
	}
	if (blurMatrix.getSize().first == 0) {
		printf("Failed to set new blur matrix : empty matrix (is kernel size odd ?)\n");
		return false;
	}
	m_blurMatrix = blurMatrix;
	m_blurMatrix.normalize();
	//m_blurMatrix.printMatrix("Blur matrix : ");
	return true;
}


bool ImageProcessor::setEdgeMatrices(const IPMatrix& horizontalEdgeMatrix
								    ,const IPMatrix& verticalEdgeMatrix) {

	if (!horizontalEdgeMatrix.isSquare() || !verticalEdgeMatrix.isSquare()) {
		printf("Failed to set new edge matrices : non-square\n");
		return false;
	}

	m_horizontalEdgeMatrix = horizontalEdgeMatrix;
	m_verticalEdgeMatrix = verticalEdgeMatrix;
	m_horizontalEdgeMatrix.normalize()/*.printMatrix("Horizontal edge matrix : ")*/;
	m_verticalEdgeMatrix.normalize()/*.printMatrix("Vertical edge matix : ")*/;
	return true;
}

void ImageProcessor::applyBrightnessFilter() {
	printf("\tApplying brightness filter...\n");
	if (!prepareBeforeFilter()) return;
	Vector2u size = m_image.getSize();
	const unsigned char* ptr = m_image.getPixelsPtr();
	// Each pixel is 4 bytes
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			int r = static_cast<int>(*(ptr));
			int g = static_cast<int>(*(ptr + 1));
			int b = static_cast<int>(*(ptr + 2));
			int brightness = gray(r, g, b);
			m_imageBuffer.setPixel(x, y, Color(brightness, brightness, brightness));
			ptr += 4;
		}
	}
	m_image = m_imageBuffer;
}

void ImageProcessor::applyBlurFilter(bool showSteps) {
	printf("\tApplying blur filter...\n");
	applyFilter(m_blurMatrix, showSteps);
}


void ImageProcessor::applyEdgeFilter(bool showSteps) {
	Image originalImage = m_image;

	printf("\tApplying horizontal edge filters...\n");
	applyFilter(m_horizontalEdgeMatrix, showSteps);
	Image horizontalFilteredImage = m_image;

	m_image = originalImage;

	printf("\tApplying vertical edge filters...\n");
	applyFilter(m_verticalEdgeMatrix, showSteps);
	Image& verticalFilteredImage = m_image;

	// Not necessary
	prepareBeforeFilter();

	if (showSteps)
		createWindow();

	printf("\tApplying final edge filter..\n");
	for (int y = 0; y < m_image.getSize().y; y++) {
		for (int x = 0; x < m_image.getSize().x; x++) {
			float Gx = static_cast<float>(horizontalFilteredImage.getPixel(x, y).r) / 255.0f;
			float Gy = static_cast<float>(verticalFilteredImage.getPixel(x, y).r) / 255.0f;
			float G = sqrtf(Gx*Gx + Gy*Gy) * 255.0f;
			G > m_edgeDetectionThreshold ? G = 255.0f : G = 0.0f;
			m_imageBuffer.setPixel(x, y, Color(G, G, G));
		}
		if (showSteps && (y % 42 == 0))
			updateWindow(m_imageBuffer);
	}
	if (showSteps)
		closeWindow();
	m_image = m_imageBuffer;
}

void ImageProcessor::setEdgeDetetctionThreshold(float value) {
	m_edgeDetectionThreshold = value;
}


bool ImageProcessor::prepareBeforeFilter()
{
	Vector2u size = m_image.getSize();
	if (size == Vector2u(0, 0)) {
		printf("No image to filter");
		return false;
	}
	m_imageBuffer.create(size.x, size.y, Color::Black);
	return true;
}


void ImageProcessor::applyFilter(IPMatrix matrixMask, bool showSteps) {
	if (!prepareBeforeFilter()) return;

	auto matrixSize = matrixMask.getSize().first;
	auto matrixHalfSize = matrixSize / 2;
	auto imageSize = m_image.getSize();

	if (showSteps) {
		createWindow();
	}

	for (int y = 0; y < imageSize.y - matrixSize; y++) {
		for (int x = 0; x < imageSize.x - matrixSize; x++) {
			float result = 0.0f;
			for (int i = 0; i < matrixSize; i++)
				for (int j = 0; j < matrixSize; j++) {
					// We take the red channel, but we could have taken any other.
					result += matrixMask[i][j] * static_cast<float>(m_image.getPixel(x + j, y + i).r);
				}
			m_imageBuffer.setPixel(x + matrixHalfSize, y + matrixHalfSize, Color(result, result, result));
		}
		if (showSteps && (y % 42 == 0)) {
			updateWindow(m_imageBuffer);
		}
	}
	if (showSteps) {
		closeWindow();
	}

	m_image = m_imageBuffer;
}


void ImageProcessor::createWindow() {
	m_window.create(VideoMode(m_image.getSize().x, m_image.getSize().y), "Filtering Image");
}

void ImageProcessor::updateWindow(const Image& image) {
	while (m_window.pollEvent(m_evnt)) {
		if (m_evnt.type == Event::Closed) {
			m_window.close();
			return;
		}
	}
	m_texture.loadFromImage(image);
	m_sprite.setTexture(m_texture, true);
	m_window.draw(m_sprite);
	m_window.display();
}

void ImageProcessor::closeWindow() {
	m_window.close();
}