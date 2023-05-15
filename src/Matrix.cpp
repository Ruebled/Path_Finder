#include <vector>

#include "Matrix.h"
#include "Matrix_row.h"

Matrix::Matrix(){
	matrix.resize(dim_y, std::vector<int>(dim_x, 0));
}

Matrix::Matrix(unsigned int y, unsigned int x):dim_y(y), dim_x(x) {
	matrix.resize(y, std::vector<int>(x, 0));
}

Matrix_row Matrix::operator[](unsigned int x) {
	return Matrix_row(matrix.at(x));
}

void Matrix::set_start(unsigned int _syPos, unsigned int _sxPos){
	this->syPos = _syPos;	
	this->sxPos = _sxPos;

	return;
}

point Matrix::get_start_point(){
	point temp;
	temp.y = this->syPos;
	temp.x = this->sxPos;
	return temp;
}

unsigned int Matrix::width(){
	return this->dim_x;
}

unsigned int Matrix::height(){
	return this->dim_y;
}
