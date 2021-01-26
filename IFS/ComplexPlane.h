#pragma once
#include <SFML\Graphics.hpp>
#include <complex>
#include "Vue.h"
#include "Homothetie.h"

using namespace sf;
using namespace std;

#define MAX_SIZE (1 << 27)

class ComplexPlane
{
public:
	ComplexPlane(RenderWindow& window, vector<complex<float>> initialPoints, Vue v = Vue());
	~ComplexPlane();

	void draw();

	void setWindow(Vue v);

	void setInitialPoints(vector<complex<float>> points);
	void addFractalFunction(Homothetie h); // Transformation : a*z + b

	void placePoints();
	void iterate(int n);
	void reset();

private :
	Vue m_vue;
	Image m_plane;
	Texture m_texture;
	Sprite m_sprite;
	RenderWindow& m_window;

	vector<Homothetie> m_h;

	vector<complex<float>> m_initialPoints;
	vector<complex<float>> m_points;
};

