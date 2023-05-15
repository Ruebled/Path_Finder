#include <unistd.h>
#include <string>
#include <map>
#include <queue>
#include <algorithm>

#include "Matrix.h"
#include "Grid.h"

std::vector<point> get_neighbors(Matrix& mat, point t_point);
bool in_dict(point point, std::map<struct point, struct point>& t_map);

void BFS(Matrix& matrix){

	// The code
	point start_point = matrix.get_start_point();

	std::queue<point> frontier;
	frontier.push(start_point);

	std::map<point, point> came_from;
	came_from[start_point] = start_point;

	// Define in outer scope the current point that is checked
	point current;

	while(!frontier.empty()){
		current = frontier.front();
		frontier.pop();

		if(matrix[current.y][current.x] == Type(end)){
			// Reverse the found path
			while(current != start_point){
				current = came_from[current];	
				if(matrix[current.y][current.x] != Type(start)){
					matrix[current.y][current.x] = Type(path);
				}
				usleep(15*1000);
			}
			break;
		}

		std::vector<point> near = get_neighbors(matrix, current);
		for (int i = 0; i < near.size(); i++)
		{
			if (!in_dict(point{near[i].y,near[i].x}, came_from)){
				frontier.push(near[i]);
				came_from[near[i]] = current;

				if(matrix[near[i].y][near[i].x] != Type(end)){
					matrix[near[i].y][near[i].x] = Type(visited);
				}
			}
		}
		usleep(20*1000);
	}

	return;
}

std::vector<point> get_neighbors(Matrix& mat, point t_point){
	std::vector<point> neigh;

	unsigned int width = mat.width();
	unsigned int height = mat.height();

	unsigned int px = t_point.x;
	unsigned int py = t_point.y;

	if(px+1 < width){
		if(mat[t_point.y][px+1] == Type(empty) ||
			mat[t_point.y][px+1] == Type(end))

		{
			neigh.push_back(point{t_point.y, px+1});
		}
	}

	if(px > 0){
		if(mat[t_point.y][px-1] == Type(empty) ||
		   mat[t_point.y][px-1] == Type(end))
		{
			neigh.push_back(point{t_point.y, px-1});
		}
	}

	if(py+1 < height){
		if(mat[py+1][t_point.x] == Type(empty) ||
			mat[py+1][t_point.x] == Type(end))

		{
			neigh.push_back(point{py+1, t_point.x});
		}
	}

	if(py > 0){
		if(mat[py-1][t_point.x] == Type(empty) ||
			mat[py-1][t_point.x] == Type(end))
		{
			neigh.push_back(point{py-1, t_point.x});
		}
	}

	std::random_shuffle(neigh.begin(), neigh.end());
	return neigh;
}

bool in_dict(point point, std::map<struct point, struct point>& t_map){
	auto res = t_map.find(point);
	if(res != t_map.end()) return true;
	return false;
}
