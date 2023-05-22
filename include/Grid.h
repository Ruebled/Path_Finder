#ifndef GRID_H
#define GRID_H

#include "Matrix.h"
#include "FileMaps.h"

// Cell types enum
enum Type {empty, start, end, wall, visited, path};

class Grid {
	private:
		// Last cell addresses where mouse where positioned
		int last_x = -1;
		int last_y = -1;

		// Starting point reset location
		const unsigned int start_y = 7;
		const unsigned int start_x = 6;

		// Ending point reset location
		const unsigned int end_y = 7;
		const unsigned int end_x = 14;

		// Remember current state
		// default 0
		// border_set 1
		// empty_set 2
		// start_set 3
		// end_set 4
		int set_state = 0;

		bool cell_pos_changed = false;

		// Checkable flags during moving and grid drawing
		int start_set = 1;
		int end_set = 1;

		// Last cell address save for restoring start and end around
		int last_row;
		int last_col;
		int last_cell_state = Type(empty);

		// Matrix class with needed overloading
		Matrix matrix;

		// FileMaps class that reads from file
		FileMaps maps;

	public:
		Grid();
		Grid(unsigned int, unsigned int);
		~Grid();

		int width();
		int height();

		int get_index(unsigned int, unsigned int);
		void set_value(unsigned int, unsigned int, unsigned int value);

		void clear();
		void reset();

		void solve(unsigned int);

		void draw_map();
		void map_save();
		void map_clear();

		void on_mouse_event(int, int, bool, int);
};

#endif
