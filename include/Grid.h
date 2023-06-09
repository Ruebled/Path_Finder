#ifndef GRID_H
#define GRID_H

#include "Matrix.h"
#include "FileMaps.h"

// Cell types enum
enum Type {empty, start, end, wall, sand, woods, water, mountains, visited = 10, path = 20};

class Grid {
	private:
		// Grid cells dimensions in braille points
		int dim_cell_x = 15;
		int dim_cell_y = 12;

		// Last cell addresses where mouse where positioned
		point last_pos = {-1, -1};

		// Starting point reset location
		const point rs_start_point = {7, 6};
		// Ending point reset location
		const point rs_end_point = {7, 14};

		// Remember current state
		// default 0
		// border_set 1
		// empty_set 2
		// start_set 3
		// end_set 4
		int set_state = 0;

		bool cell_pos_changed = false;

		// Checkable flags during moving and grid drawing
		bool start_set = true;
		bool end_set = true;

		// Last cell address save for restoring start and end around
		int last_row;
		int last_col;
		int last_cell_state = Type(empty);

		// Matrix class with needed overloading
		Matrix matrix;

		// FileMaps class that reads from file
		FileMaps maps;

		int drawing_tile_type = Type(wall);

	public:
		Grid();
		Grid(unsigned int, unsigned int);
		~Grid();

		int cell_width();
		int cell_height();

		int width();
		int height();

		int get_index(unsigned int, unsigned int);

		void clear_path();
		void clear_all();

		void reset();

		void solve(int);

		void draw_map();
		void map_save();
		void map_clear();

		void on_mouse_event(int, int, bool, int);

		void choose_tile(int);
};

#endif
