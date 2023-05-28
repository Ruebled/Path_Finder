#include <vector>

#include "Matrix.h"
#include "Matrix_row.h"

Matrix::Matrix(){
	matrix.resize(cells_y, std::vector<int>(cells_x, 0));
}

Matrix_row Matrix::operator[](unsigned int x) {
	return Matrix_row(matrix.at(x));
}

void Matrix::set_start(unsigned int _syPos, unsigned int _sxPos){
	this->syPos = _syPos;	
	this->sxPos = _sxPos;

	return;
}

void Matrix::set_end(unsigned int _eyPos, unsigned int _exPos){
	this->eyPos = _eyPos;	
	this->exPos = _exPos;

	return;
}
//change to make shorter
point Matrix::get_start_point(){
	return point{this->syPos, this->sxPos};
}

point Matrix::get_end_point(){
	return point{this->eyPos, this->exPos};
}

unsigned int Matrix::width(){
	return this->cells_x;
}

unsigned int Matrix::height(){
	return this->cells_y;
}
