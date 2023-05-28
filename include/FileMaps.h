#ifndef FILEMAPS_H
#define FILEMAPS_H

#include <vector>

class FileMaps{
	private:
		std::string maps_file_name = "../map/source";

		int left_pos = 0;

		unsigned int dim_x = 20; // cell on x axis of the grid
		unsigned int dim_y = 14; // cell on y axis of the grid

	public:
		FileMaps();

		FileMaps(int row, int col);

		~FileMaps();

		std::vector<std::vector<int>> getNext();

		void save_map(std::vector<std::vector<int>> map);

		void maps_delete();

};


#endif
