#include <vector>

#include "Matrix.h"
#include "Matrix_row.h"

Matrix::Matrix() {
  matrix.resize(cells_y, std::vector<int>(cells_x, 0));
}

Matrix_row Matrix::operator[](unsigned int x) {
  return Matrix_row(matrix.at(x));
}

void Matrix::set_start(point start) {
  this->curr_start_point = start;
  return;
}

void Matrix::set_end(point end) {
  this->curr_end_point = end;
  return;
}
// change to make shorter
point Matrix::get_start_point() {
  return curr_start_point;
}

point Matrix::get_end_point() {
  return curr_end_point;
}

unsigned int Matrix::width() {
  return this->cells_x;
}

unsigned int Matrix::height() {
  return this->cells_y;
}
