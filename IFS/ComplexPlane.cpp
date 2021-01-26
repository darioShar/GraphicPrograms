#include "stdafx.h"
#include "ComplexPlane.h"


ComplexPlane::ComplexPlane(RenderWindow& window, vector<complex<float>> initialPoints, Vue v)
	:	m_window(window)
	,	m_initialPoints(initialPoints)
	,	m_points(initialPoints)
	,	m_vue(v)
{
	m_plane.create(m_window.getSize().x, m_window.getSize().y);
	reset();
}

ComplexPlane::~ComplexPlane()
{
}

void ComplexPlane::draw() {
	m_window.draw(m_sprite);
}

void ComplexPlane::setWindow(Vue v) {
	m_vue = v;
}

void ComplexPlane::setInitialPoints(vector<complex<float>> points) {
	m_initialPoints = points;
	m_points = points;
}

void ComplexPlane::addFractalFunction(Homothetie h) {
	m_h.push_back(h);
}

void ComplexPlane::placePoints() {
	auto& size = m_plane.getSize();
	m_plane.create(size.x, size.y);

	for (auto& p : m_points) {
		if (   p.real() <= m_vue.xmax
			&& p.real() >= m_vue.xmin
			&& p.imag() <= m_vue.ymax
			&& p.imag() >= m_vue.ymin) {
			float x = (p.real() - m_vue.xmin) / (m_vue.xmax - m_vue.xmin);
			float y = (p.imag() - m_vue.ymin) / (m_vue.ymax - m_vue.ymin);
			float xx = x * size.x;
			float yy = y * size.y;
			m_plane.setPixel(xx, yy, Color::White);
		}
	}

	m_texture.loadFromImage(m_plane);
	m_sprite.setTexture(m_texture);
}

void ComplexPlane::iterate(int n) {
	auto size = m_points.size();
	if (size >= MAX_SIZE) {
		int newSize = (MAX_SIZE / m_h.size()) / 2;
		vector<complex<float>> buffer;
		buffer.resize(newSize);
		for (int i = 0; i < newSize; i++) {
			buffer[i] = m_points[i];
		}
		m_points = buffer;
		size = newSize;
	}
	for (int i = 0; i < size; i++) {
		auto p = m_points[i];
		for (int k = 0; k < m_h.size() - 1; k++) {
			p = m_points[i];
			for (int j = 0; j < n; j++) {
				p = m_h[k](p);
			}
			m_points.push_back(p);
		}
		p = m_points[i];
		for (int j = 0; j < n; j++) {
			p = m_h[m_h.size() - 1](p);
		}
		m_points[i] = p;
	}

	placePoints();
}

void ComplexPlane::reset() {
	m_points = m_initialPoints;
	placePoints();
}