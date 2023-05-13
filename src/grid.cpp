#include "grid.h"

Grid::Grid(){
}

Grid::Grid(int y_size, int x_size){
	this->size_x = x_size;
	this->size_y = y_size;

	// take as another matrix class to reset method 
	for(int i = 0; i < this->size_y; i++){
		for(int j = 0; j < this->size_x; j++){
			matrix[i][j] = 0;
		}
	}

	matrix[7][6] = 1;
	matrix[7][14] = 2;

	start_set = 1;
	end_set = 1;
}

int Grid::get_index(int x, int y){
	return this->matrix[x][y];
}

void Grid::modify(int x, int y){
}

int Grid::get_rows(){
	return this->size_y;
}

int Grid::get_cols(){
	return this->size_x;
}

void Grid::on_mouse_event(int row, int col, bool left_click, int mouse_pressed){
	// Case where matrix indexes are out of bounds
	if(this->size_x < col || this->size_y < row){
		return;
	}

	// remember the current mouse position cell
	int &address_ind = this->matrix[row][col];


	if(mouse_pressed == 0){
		// Finish border set...
		border_set = 0;
		if(point_set){
			if(!end_set){
				if(address_ind == 1){
					this->matrix[temp_y][temp_x] = 2;
				}else{
					this->matrix[last_y][last_x] = 2;
				}
				point_set = 0;
				end_set = 1;
				return;
			}
			if(!start_set){
				if(address_ind == 2){
					this->matrix[temp_y][temp_x] = 1;
				}else{
					this->matrix[last_y][last_x] = 1;
				}
				point_set = 0;
				start_set = 1;
				return;
			}
		}
	}

	// if another mouse action is made then do nothing
	if(!left_click) return;

	// if the mouse position changes but cell not then return 
	if(row == this->last_y && col == this->last_x){
		return;
	}
	else
	{
		this->last_y = row;
		this->last_x = col;
	}


	if(mouse_pressed == 1){

		if(!point_set){

			if(address_ind == 3){
				border_set = 1;
				address_ind = 0;	
				return;
			}	

			if(address_ind == 0){
				border_set = 1;
				address_ind = 3;
				return;
			}
		}

		if(border_set){return;}

		if(address_ind == 1){
			if(!end_set){ return; }

			temp_x = col;
			temp_y = row;
			point_set = 1;

			address_ind = 0;
			start_set = 0;
			return;
		}

		if(address_ind == 2){
			if(!start_set){ return; }

			temp_x = col;
			temp_y = row;
			point_set = 1;

			address_ind = 0;
			end_set = 0;
			return;
		}
		return;
	}

	return;
}

void Grid::on_refresh_event(){

	return;
}

void Grid::clear(){
	for(int i = 0; i < this->size_y; i++){
		for(int j = 0; j < this->size_x; j++){
			if(matrix[i][j] == 1 || matrix[i][j] == 2) 
				continue;
			matrix[i][j] = 0;
		}
	}
}

void Grid::reset(){

	for(int i = 0; i < this->size_y; i++){
		for(int j = 0; j < this->size_x; j++){
			matrix[i][j] = 0;
		}
	}

	matrix[7][6] = 1;
	matrix[7][14] = 2;
}


Grid::~Grid(){

}
