#include <thread>
#include <string>

#include "Grid.h"
#include "Matrix.h"
#include "Algo.h"
#include "FileMaps.h"

// Global variable for thread status
bool thread_active = false;

extern float cpu_time;
extern float real_time;

Grid::Grid(){ 
	this->matrix = Matrix();
	this->maps = FileMaps();

	matrix.set_start(this->start_y, this->start_x);
	matrix.set_end(this->end_y, this->end_x);

	Grid::reset();
} 

Grid::Grid(unsigned int rows, unsigned int cols):
									matrix(Matrix(rows, cols))
									//,map(FileMaps(rows, cols))
{
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
	// Null benchmark value
	cpu_time = 0;
	real_time = 0;

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
}

void Grid::solve(unsigned int choice){
	for(int row = 0; row < matrix.height(); row++){
		for(int col = 0; col < matrix.width(); col++){
			if(matrix[row][col] == Type(visited) ||
					matrix[row][col] == Type(path)){
				matrix[row][col] = Type(empty);
			}
		}
	}
	
	auto func = &BFS;

	//switch(choice){
	//	case 0:
	//		func = &BFS;
	//		break;
	//	default:
	//		break;
	//}

	// Start the thread
	std::thread th(func, std::ref(matrix));
	// Detach the thread
	th.detach();

	thread_active = true;

	return;
}

void Grid::draw_map(){
	std::vector<std::vector<int>> map = this->maps.getNext();	

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
	if(Grid::width() < col || Grid::height() < row){ return; }

	// remember the current mouse position cell
	int& cell = matrix[row][col];

	if(!mouse_pressed){
		if(set_state == 3){
			matrix.set_start(row, col);
		}else if(set_state == 4){
			matrix.set_end(row, col);
		}

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
					cell = Type(wall);
					break;

				case Type(wall):
				case Type(visited):
				case Type(path):
					this->set_state = 2;
					cell = Type(empty);
					break;

				case Type(start):
					this->set_state = 3;

					// Address of the cell
					this->last_row = row;
					this->last_col = col;

					// Value of the cell
					this->last_cell_state = Type(empty); 
					break;

				case Type(end):
					this->set_state = 4;

					// Address of the cell
					this->last_row = row;
					this->last_col = col;

					// Value of the cell
					this->last_cell_state = Type(empty); 
					break;

				default:
					break;
			}

		}else if(set_state == 1){
			// State if border is being set
			switch(cell){
				case Type(visited):
				case Type(path):
				case Type(empty):
					if(!cell_pos_changed){
						break;
					}
					this->set_state = 1;
					cell = Type(wall);
					break;
				default:
					break;
			}

		}else if(set_state == 2){
			// State if empty cell is being set
			switch(cell){
				case Type(empty):
				case Type(wall):
				case Type(visited):
				case Type(path):
					cell = Type(empty);
					break;
				default:
					break;
			}

		}else if(set_state == 3){
			// State if start point is being set
			switch(cell){
				case Type(empty):
				case Type(wall):
				case Type(visited):
				case Type(path):
					matrix[this->last_row][this->last_col] = this->last_cell_state;

					// Save the state of current cell
					this->last_row = row;
					this->last_col = col;
					this->last_cell_state = cell;

					cell = Type(start);
					break;
				default:
					break;
			}
		}else if(set_state == 4){
			// State if end point is being set
			switch(cell){
				case Type(empty):
				case Type(wall):
				case Type(visited):
				case Type(path):
					matrix[this->last_row][this->last_col] = this->last_cell_state;

					this->last_row = row;
					this->last_col = col;
					this->last_cell_state = cell;

					cell = Type(end);
					break;
				default:
					break;
			}

		}
	}
	this->cell_pos_changed = false;
	return;
}
