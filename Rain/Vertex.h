#pragma once

struct Position {
	float x;
	float y;
} position;


struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} color;

/*
struct UV {
	float u;
	float v;
}; */


struct Vertex {
	Position position;

	Color color;

	//UV uv;

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	/*void setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}*/

	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}
};