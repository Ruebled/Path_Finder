#ifndef MATRIX_ROW_H
#define MATRIX_ROW_H

#include <vector>

class Matrix_row {
	private:
		std::vector<int>& row;
	public:
		Matrix_row(std::vector<int>& r) : row(r) {
		}

		int& operator[](unsigned int y) {
			return row.at(y);
		}
};

#endif
