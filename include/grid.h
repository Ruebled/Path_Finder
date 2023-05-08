#ifndef GRID_H
#define GRID_H

class Grid {
	private:
		int x_size;
		int y_size;
		// Think of replacing with your own definition of matrix
		int matrix[20][20];

	public:
		Grid();
		Grid(int x_size, int y_size);
		~Grid();
		int get(int x, int y);
		void modify(int x, int y);
};

#endif
