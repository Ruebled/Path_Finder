#include "Algo.h"

// Global variables performance report
// variable cpu_time 
float cpu_time = 0;

// cpu_time variable text
float real_time = 0;

// Global variable for thread status
extern bool thread_active;

void BFS(Matrix& matrix){
	// The code
	point start_point = matrix.get_start_point();

	std::queue<point> frontier;
	frontier.push(start_point);

	std::map<point, point> came_from;
	came_from[start_point] = start_point;

	// Define in outer scope the current point that is checked
	point current;

	std::chrono::duration<double> cpu_chrono_time(0);

	auto real_time_start = std::chrono::steady_clock::now();
	while(!frontier.empty()){
		// Start time
		auto cpu_time_start = std::chrono::steady_clock::now();

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
			auto real_time_end = std::chrono::steady_clock::now();
			std::chrono::duration<double> real_elapsed_time = real_time_end - real_time_start;

			int real_time_int = std::chrono::floor<std::chrono::milliseconds>(real_elapsed_time).count();

			// Time in miliseconds
			real_time = (real_time_int)/1000 + 
						((float) (real_time_int%1000))/1000;
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

		auto cpu_time_end = std::chrono::steady_clock::now();
		std::chrono::duration<double> cpu_elapsed_time = cpu_time_end - cpu_time_start;

		cpu_chrono_time += cpu_elapsed_time;

		usleep(20*1000);
	}

	int cpu_time_int = std::chrono::floor<std::chrono::nanoseconds>(cpu_chrono_time).count();

	// Time in miliseconds
	cpu_time = (float)(cpu_time_int)/1000000 + ((float) (cpu_time_int%1000000))/1000000;

	thread_active = false;
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