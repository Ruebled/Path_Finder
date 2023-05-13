#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "Matrix_row.h"

class Matrix {
	private:
		std::vector<std::vector<int>> matrix;

	public:
		Matrix(unsigned int y, unsigned int x);

		Matrix_row operator[](unsigned int x);
};

#endif
