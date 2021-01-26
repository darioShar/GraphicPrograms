#pragma once
#include <complex>

struct Homothetie
{
public:
	Homothetie(std::complex<float> a, std::complex<float> b) 
	:	m_a(a)
	,	m_b(b)
	{}
	~Homothetie() {}

	std::complex<float> operator()(std::complex<float> z) {
		return m_a * z + m_b;
	}

	std::complex<float> m_a, m_b;
};

