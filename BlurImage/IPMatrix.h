#pragma once
#include <vector>
#include <cmath>
#include <utility>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class IPMatrix
{
	using Vec1D = std::vector<float>;
	using Vec2D =  std::vector<Vec1D>;

protected :
	Vec2D m_matrix;

public :

	IPMatrix() : m_matrix(Vec2D()) {};

	IPMatrix(const IPMatrix& matrix) {
		m_matrix = matrix.m_matrix;
	}

	IPMatrix(const Vec2D matrix) {
		// Checks that matrix is non-empty and that all columns have the same size
		if (matrix.size() != 0) {
			auto size = matrix[0].size();
			for (auto& vec : matrix) {
				if (vec.size() != size) {
					return;
				}
			}
			m_matrix = matrix;
		}
	}
	~IPMatrix() 
	{}

	IPMatrix& operator*(float factor) {
		for (auto& vec : m_matrix)
			for (auto& elem : vec)
				elem *= factor;
		return *this;
	}

	auto begin() {
		return m_matrix.begin();
	}

	auto begin() const {
		return m_matrix.begin();
	}
	auto end() {
		return m_matrix.end();
	}
	auto end() const {
		return m_matrix.end();
	}

	const Vec1D operator[](int i) const {
		return m_matrix[i];
	}

	Vec1D& operator[](int i) {
		return m_matrix[i];
	}

	std::pair<float, float> getSize() const {
		return std::make_pair<float, float>(m_matrix.size(), m_matrix[0].size());
	}

	IPMatrix& normalize() {
		float sum = 0.0;
		for (auto& vec : m_matrix)
			for (auto& elem : vec)
				sum += elem;
		if (std::abs(sum) < 0.001) {
			// Then we normalize with half the sum of absolute value (sobel operator in mind)
			sum = 0.0f;
			for (auto& vec : m_matrix)
				for (auto& elem : vec)
					sum += std::abs(elem);
			sum /= 2.0f;
		}
		float factor = 1.0f / sum;
		for (auto& vec : m_matrix)
			for (auto& elem : vec)
				elem *= factor;
		return *this;
	}

	bool isSquare() const {
		auto size = m_matrix.size();
		for (auto& vec : m_matrix)
			if (vec.size() != size)
				return false;
		return true;
	}

	static IPMatrix createGaussianKernel(int size, float sigma) {
		Vec2D matrix;
		if (size % 2 == 0)
			return IPMatrix();
		int k = size / 2;
		float sigma2 = sigma * sigma;
		matrix.resize(size);
		for (auto& vec : matrix) vec.resize(size);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				float term1 = (i + 1 - (k + 1)) * (i + 1 - (k + 1));
				float term2 = (j + 1 - (k + 1)) * (j + 1 - (k + 1));
				matrix[i][j] = expf(-(term1 + term2) / (2 * sigma2))
					/ (2 * M_PI * sigma2);
			}
		}
		return IPMatrix(matrix);
	}

	void printMatrix(const std::string& message = "") {
		printf("%s : \n", message.c_str());
		for (auto& vec : m_matrix) {
			for (auto& elem : vec)
				printf("%lf ", elem);
			printf("\n");
		}
		printf("\n\n");
	}

};

