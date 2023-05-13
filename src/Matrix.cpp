#include <vector>

#include "Matrix.h"
#include "Matrix_row.h"


Matrix::Matrix(unsigned int y, unsigned int x) {
	matrix.resize(y, std::vector<int>(x, 0));
}

Matrix_row Matrix::operator[](unsigned int x) {
	return Matrix_row(matrix.at(x));
}
