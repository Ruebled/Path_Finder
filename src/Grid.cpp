#include <thread>
#include <string>
#include <functional>

#include "Grid.h"
#include "Matrix.h"
#include "Algo.h"
#include "FileMaps.h"

// Global variable for thread status
bool thread_active = false;

extern float cpu_time;
extern float real_time;
extern int distance_path;

Grid::Grid() {
	this->matrix = Matrix();
	this->maps = FileMaps();

	matrix.set_start(this->start_y, this->start_x);
	matrix.set_end(this->end_y, this->end_x);

	Grid::reset();
}

Grid::~Grid(){ } // Grid Destructor, just to be

int Grid::cell_width(){
	return this->dim_cell_x;
}

int Grid::cell_height(){
	return this->dim_cell_y;
}

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

void Grid::clear(int type){
	// Clear cells to Type.empty
	for(int row = 0; row < matrix.height(); row++){
		for(int col = 0; col < matrix.width(); col++){
			// Leave start and end point at their positions
			if(matrix[row][col] == Type(start) 
				|| matrix[row][col] == Type(end) ){
				continue;
			}

			if(matrix[row][col] == Type(type)){
				matrix[row][col] = Type(empty);
			}
		}
	}
	// Null benchmark value
	cpu_time = 0;
	real_time = 0;
	distance_path = 0;

	return;
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
	//
	matrix.set_start(start_y, start_x);

	// Set end point
	matrix[end_y][end_x] = Type(end);
	// Null benchmark value
	cpu_time = 0;
	real_time = 0;
	distance_path = 0;
}

void Grid::solve(int choice){
	// Null benchmark value
	cpu_time = 0;
	real_time = 0;
	distance_path = 0;

	for(int row = 0; row < matrix.height(); row++){
		for(int col = 0; col < matrix.width(); col++){
			if(matrix[row][col] == Type(visited) ||
					matrix[row][col] == Type(path)){
				matrix[row][col] = Type(empty);
			}
		}
	}

	// Variable to store the function
	std::function<void(Matrix&)> func;

	switch(choice){
		case 0:
			func = &BreathFirstSearch;
			break;
		case 1:
			func = &Dijkstra;
			break;
		case 2:
			func = &AStar;
			break;
			//add rest of the cases
			//don't forget the brakes
		default:
			func = &BreathFirstSearch;
			break;
	}

	// Start the thread
	std::thread th(func, std::ref(matrix));
	// Detach the thread
	th.detach();

	thread_active = true;

	return;
}

void Grid::map_clear(){
	maps.maps_delete();
}

void Grid::map_save(){
	std::vector<std::vector<int>> map;

	map.resize(matrix.height(), std::vector<int>(matrix.width(), 0));

	for(int row = 0; row < matrix.height(); row++){
		for(int col = 0; col < matrix.width(); col++){
			if(matrix[row][col] == Type(start) || 
			   matrix[row][col] == Type(end) || 
			   matrix[row][col] == Type(wall)){
				map[row][col] = matrix[row][col];
			}else{
				map[row][col] = Type(empty);
			}
		}
	}
	this->maps.save_map(map);		
	return;
}

void Grid::draw_map(){
	std::vector<std::vector<int>> map = this->maps.getNext();	

	// Check for 0 or empty elements of map
	int diff = 0;
	for(std::vector line : map){
		for(int num : line){
			if(num != 0) diff++;
		}
	}
	if(!diff) return;

	int cols = matrix.width();
	int rows = matrix.height();

	for(int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			this->matrix[row][col] = map[row][col];

			if(map[row][col] == Type(start)){
				matrix.set_start(row, col);
			}
			if(map[row][col] == Type(end)){
				matrix.set_end(row, col);
			}
		}
	}

	return;
}

void Grid::on_mouse_event(int row, int col, bool left_click, int mouse_pressed){
	// Case where matrix indexes are out of bounds
	if(Grid::width() <= col || Grid::height() <= row){ 
		this->set_state = 0;
		return; 
	}

	// remember the current mouse position cell
	int& cell = matrix[row][col];

	if(!mouse_pressed){
		this->last_y = -1;
		this->last_x = -1;

		this->set_state = 0;
	}

	if(left_click && mouse_pressed){
		// State if just clicking
		if(this->last_x != col || this->last_y != row){
			this->cell_pos_changed = true;
			this->last_y = row;
			this->last_x = col;
		}
		if(!cell_pos_changed || thread_active){
			this->cell_pos_changed = false;
			return;
		}
		if(set_state == 0){
			switch(cell){
				case Type(empty):
					this->set_state = 1;
					cell = Type(drawing_tile_type);
					break;

				case Type(start):
					this->set_state = 3;

					// Address of the cell
					this->last_row = row;
					this->last_col = col;

					// Value of the cell
					this->last_cell_state = Type(empty); 

					// Clear the path if any
					Grid::clear(Type(path));

					break;

				case Type(end):
					this->set_state = 4;

					// Address of the cell
					this->last_row = row;
					this->last_col = col;

					// Value of the cell
					this->last_cell_state = Type(empty); 

					// Clear the path if any
					Grid::clear(Type(path));

					break;

				default:
					this->set_state = 2;
					cell = Type(empty);
					break;
			}

		}else if(set_state == 1){
			// State if cells(types) is being set(wall, water, sand)
			switch(cell){
				case Type(visited):
				case Type(path):
				case Type(empty):
					if(!cell_pos_changed){
						break;
					}
					this->set_state = 1;
					cell = Type(drawing_tile_type);
					break;
				default:
					break;
			}

		}else if(set_state == 2){
			// State if empty cell is being set
			switch(cell){
				case Type(start):
				case Type(end):
					break;
				default:
					cell = Type(empty);
					break;
			}

		}else if(set_state == 3){
			// State if start point is being set
			switch(cell){
				case Type(end):
					break;
				default:
					matrix[this->last_row][this->last_col] = this->last_cell_state;

					// Save the state of current cell
					this->last_row = row;
					this->last_col = col;
					this->last_cell_state = cell;

					cell = Type(start);
					matrix.set_start(row, col);
					break;
			}
		}else if(set_state == 4){
			// State if end point is being set
			switch(cell){
				case Type(start):
					break;
				default:
					matrix[this->last_row][this->last_col] = this->last_cell_state;

					this->last_row = row;
					this->last_col = col;
					this->last_cell_state = cell;

					cell = Type(end);
					matrix.set_end(row, col);
					break;
			}

		}
	}
	this->cell_pos_changed = false;
	return;
}

void Grid::choose_tile(int index){
	this->drawing_tile_type = index;
	return;
}
