#include "grid.h"

Grid::Grid(){

}

Grid::Grid(int x_size, int y_size){
	this->x_size = x_size;
	this->y_size = y_size;

	int grid[20][20] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};
}

int Grid::get(int x, int y){
	return this->matrix[x][y];
}

void Grid::modify(int x, int y){
	
}

Grid::~Grid(){

}
