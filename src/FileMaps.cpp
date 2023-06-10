#include <fstream>
#include <string>
#include <vector>

#include "FileMaps.h"

FileMaps::FileMaps() {}

FileMaps::FileMaps(int row, int col) {
  // Maybe later
}

std::vector<std::vector<int>> FileMaps::getNext() {
  std::ifstream maps_file;
  maps_file.open(this->maps_file_name, std::ios::in);

  // Define a map
  std::vector<std::vector<int>> map;
  map.resize(dim_y, std::vector<int>(dim_x));

  if (maps_file.is_open()) {
    maps_file.seekg(left_pos, std::ios::beg);

    std::string line;
    if (left_pos == 0) {
      getline(maps_file, line);
    }

    if (maps_file.eof()) {
      maps_file.seekg(0, std::ios::beg);
    }

    int line_count = 0;

    while (getline(maps_file, line)) {
      if (line.empty()) {
        this->left_pos = maps_file.tellg();
        maps_file.close();
        return map;
      }

      for (int char_count = 0; char_count < this->dim_x; char_count++) {
        map[line_count][char_count] = (int)(line[char_count] - '0');
      }
      line_count++;
    }
  }
  this->left_pos = 0;
  maps_file.close();
  return map;
}

void FileMaps::save_map(std::vector<std::vector<int>> map) {
  std::ofstream maps_file;
  maps_file.open(this->maps_file_name, std::ios::app);

  if (maps_file.is_open()) {
    // Create the necessary line
    maps_file.put('\n');

    // Iterate through every item and put in file
    for (std::vector line : map) {
      for (int ch : line) {
        maps_file.put(ch + '0');
      }
      maps_file.put('\n');
    }

    maps_file.close();
  }
  return;
}

void FileMaps::maps_delete() {
  std::ofstream maps_file;
  maps_file.open(this->maps_file_name, std::ios::trunc);

  if (maps_file.is_open()) {
    maps_file.close();
  }
  return;
}

FileMaps::~FileMaps() {}
