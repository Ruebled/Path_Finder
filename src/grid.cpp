#include "grid.h"

Grid::Grid(){
}

Grid::Grid(int y_size, int x_size){
	this->size_x = x_size;
	this->size_y = y_size;

	for(int i = 0; i < this->size_y; i++){
		for(int j = 0; j < this->size_x; j++){
			matrix[i][j] = 0;
		}
	}

	matrix[5][4] = 1;
	matrix[5][8] = 2;

	matrix[4][6] = 3;
	matrix[5][6] = 3;
	matrix[6][6] = 3;

	start_set = 1;
	end_set = 1;
}

int Grid::get_index(int x, int y){
	return this->matrix[x][y];
}

void Grid::modify(int x, int y){
	if(this->matrix[x][y] == 0){
		//if(!start_set){
		//	this->matrix[x][y] = 1;
		//	start_set = 1;
		//	return;	
		//}
		//if(!end_set){
		//	this->matrix[x][y] = 2;
		//	end_set = 1;
		//	return;	
		//}
		this->matrix[x][y] = 3;
	//} else if(this->matrix[x][y] == 1){
	}
	//	if(!end_set){
	//		return;	
	//	}
	//	this->matrix[x][y] = 0;
	//	start_set = 0;
	//	return;
	//} else if(this->matrix[x][y] == 2){
	//	if(!start_set){
	//		return;
	//	}
	//	this->matrix[x][y] = 0;
	//	end_set = 0;
	//	return;
	//}
	return;	
}

int Grid::get_rows(){
	return this->size_y;
}

int Grid::get_cols(){
	return this->size_x;
}

void Grid::on_mouse_event(){

	return;
}

void Grid::on_refresh_event(){

	return;
}

Grid::~Grid(){

}
