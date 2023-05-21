#include <vector>
#include <string>
#include <fstream>

#include "FileMaps.h"

FileMaps::FileMaps(){
	this->map.resize(dim_y, std::vector<int>(dim_x, 0));
}

FileMaps::FileMaps(int row, int col){
	// Maybe later
}

std::vector<std::vector<int>> FileMaps::getNext(){
	//modify # here to work maybe on windows
	std::ifstream maps_file;
	maps_file.open("../map/source", std::ios::in);

	if(maps_file.is_open()){
		maps_file.seekg(left_pos, std::ios::beg);

		if(maps_file.eof()){
			maps_file.seekg(0, std::ios::beg);
		}

		int line_count = 0;

		std::string line;
		while(getline(maps_file, line)){
			if(line.empty()){
				this->left_pos = maps_file.tellg();
				break;
			}
			
			for(int char_count = 0; char_count<this->dim_x; char_count++){
				this->map[line_count][char_count] = (int)(line[char_count] - '0');
			}
			line_count++;
		}
	}
	maps_file.close();
	return this->map;
}

FileMaps::~FileMaps(){
}
