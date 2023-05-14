#include <thread>

#include "Grid.h"
#include "Matrix.h"
#include "Algo.h"

Grid::Grid(){ 
	matrix = Matrix();

	matrix.set_start(this->start_y, this->start_x);

	Grid::reset();
} 

Grid::Grid(unsigned int rows, unsigned int cols):matrix(Matrix(rows, cols)){
	Grid::reset();
}

Grid::~Grid(){ } // Grid Destructor, just to be

int Grid::width(){
	return matrix.width();
}

int Grid::height(){
	return matrix.height();
}

int Grid::get_index(unsigned int y, unsigned int x){
	return matrix[y][x];
}

void Grid::set_value(unsigned int y, unsigned int x, unsigned int value){
	matrix[y][x] = Type(value);	
	return;
}

void Grid::clear(){
	// Clear cells to Type.empty
	for(int row = 0; row < matrix.height(); row++){
		for(int col = 0; col < matrix.width(); col++){
			// Leave start and end point at their positions
			if(matrix[row][col] == Type(start) || matrix[row][col] == Type(end)){
				continue;
			}
			matrix[row][col] = Type(empty);
		}
	}
}

void Grid::reset(){
	// Clear/Set cells to Type.empty
	for(int row = 0; row < matrix.height(); row++){
		for(int col = 0; col < matrix.width(); col++){
			matrix[row][col] = Type(empty);
		}
	}

	// Set start point using
	matrix[start_y][start_x] = Type(start);

	// Set end point
	matrix[end_y][end_x] = Type(end);
}

void Grid::solve(unsigned int choice){
	auto func = &A_star;

	switch(choice){
		case 0:
			func = &A_star;
			break;
		default:
			break;
	}

	// Start the thread
	std::thread th(func, std::ref(matrix));
	// Detach the thread
	th.detach();

	return;
}

void Grid::on_mouse_event(int row, int col, bool left_click, int mouse_pressed){
	// Case where matrix indexes are out of bounds
	if(Grid::width() < col || Grid::height() < row){
		return;
	}

	// remember the current mouse position cell
	int &address_ind = matrix[row][col];

	if(!mouse_pressed){
		// Finish border set...
		border_set = 0;
		if(point_set){
			if(!end_set){
				if(address_ind == Type(start)){
					matrix[temp_y][temp_x] = 2;
				}else{
					matrix[last_y][last_x] = 2;
				}
				point_set = 0;
				end_set = 1;
				return;
			}
			if(!start_set){
				if(address_ind == Type(end)){
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


	if(mouse_pressed){

		if(!point_set){

			if(address_ind == Type(wall)){
				border_set = 1;
				address_ind = Type(empty);	
				return;
			}	

			if(address_ind == Type(empty)){
				border_set = 1;
				address_ind = Type(wall);
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

