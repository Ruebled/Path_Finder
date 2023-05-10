#ifndef GRID_H
#define GRID_H

class Grid {
	private:
		int size_x;
		int size_y;

		int start_set = 1;
		int end_set = 1;

		// Think of replacing with your own definition of matrix
		int matrix[20][20];

	public:
		Grid();
		Grid(int x_size, int y_size);
		~Grid();

		void on_mouse_event();
		void on_refresh_event();

		int get_rows();
		int get_cols();

		int get_index(int x, int y);
		void modify(int x, int y);

};

#endif
