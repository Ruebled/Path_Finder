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

		std::string line;
		getline(maps_file, line);

		if(maps_file.eof()){
			maps_file.seekg(0, std::ios::beg);
		}

		int line_count = 0;

		while(getline(maps_file, line)){
			if(line.empty()){
				this->left_pos = maps_file.tellg();
				maps_file.close();
				return this->map;	
			}
			
			for(int char_count = 0; char_count<this->dim_x; char_count++){
				this->map[line_count][char_count] = (int)(line[char_count] - '0');
			}
			line_count++;
		}
	}
	this->left_pos = 0;
	maps_file.close();
	return this->map;
}

FileMaps::~FileMaps(){
}
