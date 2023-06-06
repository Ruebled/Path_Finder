#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#include "Matrix_row.h"

struct point{
	int y;
	int x;

	friend bool operator<(const point& l, const point& r) {
        return std::tie(l.y, l.x) < std::tie(r.y, r.x);
    }
	friend bool operator>(const point& l, const point& r) {
        return std::tie(l.y, l.x) > std::tie(r.y, r.x);
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
		point curr_start_point;

		// Ending point location
		point curr_end_point;

		// Cells sizes
		int cells_x = 20; // cell on x axis of the grid
		int cells_y = 14; // cell on y axis of the grid

		std::vector<std::vector<int>> matrix;

	public:
		Matrix();

		Matrix(int y, int x);

		Matrix_row operator[](unsigned int x);

		void set_start(point);
		void set_end(point);

		unsigned int width();
		unsigned int height();

		point get_start_point();
		point get_end_point();
};

#endif
