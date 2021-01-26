#pragma once

typedef double DATA_TYPE;

struct Vue {
	DATA_TYPE xmin = -1.0f;
	DATA_TYPE xmax = 1.0f;
	DATA_TYPE ymin = -1.0f;
	DATA_TYPE ymax = 1.0f;

	Vue(DATA_TYPE aspectRatio = 1.0f) {
		DATA_TYPE middle = (xmax + xmin) / 2.0f;
		DATA_TYPE dx = middle - xmin;
		dx *= aspectRatio;
		xmin = middle - dx;
		xmax = middle + dx;
	}

	void zoom(DATA_TYPE factor) {
		DATA_TYPE middle = (xmax + xmin) / 2.0f;
		DATA_TYPE dx = middle - xmin;
		dx /= factor;
		xmin = middle - dx;
		xmax = middle + dx;

		middle = (ymax + ymin) / 2.0f;
		DATA_TYPE dy = middle - ymin;
		dy /= factor;
		ymin = middle - dy;
		ymax = middle + dy;
	}

	void move(DATA_TYPE x, DATA_TYPE y) {
		xmin += x;
		xmax += x;
		ymin += y;
		ymax += y;
	}
};
