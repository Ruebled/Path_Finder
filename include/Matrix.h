#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#include "Matrix_row.h"

struct point{
	unsigned int y;
	unsigned int x;

	friend bool operator<(const point& l, const point& r) {
        return std::tie(l.y, l.x) < std::tie(r.y, r.x);
    }

    friend bool operator==(const point& l, const point& r) {
        return std::tie(l.y, l.x) == std::tie(r.y, r.x);
    }

    friend bool operator!=(const point& l, const point& r) {
        return std::tie(l.y, l.x) != std::tie(r.y, r.x);
    }
};

class Matrix {
	private:
		// Starting point location
		unsigned int sxPos;
		unsigned int syPos;

		// Ending point location
		unsigned int exPos;
		unsigned int eyPos;

		unsigned int cells_x = 20; // cell on x axis of the grid
		unsigned int cells_y = 14; // cell on y axis of the grid

		std::vector<std::vector<int>> matrix;

	public:
		Matrix();

		Matrix(unsigned int y, unsigned int x);

		Matrix_row operator[](unsigned int x);

		void set_start(unsigned int syPos, unsigned int sxPos);
		void set_end(unsigned int eyPos, unsigned int exPos);

		unsigned int width();
		unsigned int height();

		point get_start_point();
		point get_end_point();
};

#endif
