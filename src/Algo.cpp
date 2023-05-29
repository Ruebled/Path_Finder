#include "Algo.h"


// distance of the path
int distance_path = 0;

// Global variable for thread status
extern bool thread_active;

// Diagonal calc 
bool diag_checked = false;

void BreathFirstSearch(Matrix& matrix){
	// The code
	point start_point = matrix.get_start_point();

	// Queue to store neighbors for further processing
	std::queue<point> neigh;
	neigh.push(start_point);

	// Queue to store all neighbors
	std::deque<point> neigh_reverse;

	std::map<point, point> came_from;
	came_from[start_point] = start_point;

	// Define in outer scope the current point that is checked
	point current;

	// Create TimeTracker class for time counting
	TimeTracker timeTracker;

	// Start the stopwatch for real time
	timeTracker.rlt_start();

	while(!neigh.empty()){
		// Start time
		timeTracker.cpu_start();

		current = neigh.front();
		neigh.pop();

		if(matrix[current.y][current.x] == Type(end)){
			// Reverse the found path
			while(current != start_point){
				current = came_from[current];	
				if(matrix[current.y][current.x] != Type(start)){
					matrix[current.y][current.x] = Type(path);
				}
				distance_path++;
				// Path delay
				usleep(15*1000);
			}

			// Pause the real time stopwatch
			timeTracker.rlt_pause();

			// subtrack one(stop point) from distance path
			distance_path--;

			// In case of a path clear all visited cells
			point rev;
			while(!neigh_reverse.empty()){
				rev = neigh_reverse.back();
				neigh_reverse.pop_back();
				if(matrix[rev.y][rev.x] == Type(visited)){
					matrix[rev.y][rev.x] = Type(empty);
					// Remove delay
					usleep(10*1000);
				}
			}

			// Time real in miliseconds, till path found
			timeTracker.rlt_stop();
			break;
		}

		// Get a <point> vector of current location neighbors
		std::vector<point> near = get_neighbors(matrix, current);

		for (int i = 0; i < near.size(); i++)
		{
			if (!in_dict(point{near[i].y,near[i].x}, came_from)){
				neigh.push(near[i]);
				neigh_reverse.push_back(near[i]);

				came_from[near[i]] = current;

				if(matrix[near[i].y][near[i].x] != Type(end)){
					matrix[near[i].y][near[i].x] = Type(visited);
				}
			}
		}

		timeTracker.cpu_pause();

		// Visited delay
		usleep(20*1000);
	}

	timeTracker.cpu_stop();

	thread_active = false;
	return;
}

void AStar(Matrix& matrix){

}


std::vector<point> get_neighbors(Matrix& mat, point t_point){
	std::vector<point> neigh;

	unsigned int width = mat.width();
	unsigned int height = mat.height();

	unsigned int px = t_point.x;
	unsigned int py = t_point.y;

	// Some magic (half asleep)
	for(int row=-1; row<2; row++){
		int t_y = py + row;
		for(int col=-1; col<2; col++){
			int t_x = px + col;
			if(t_y < 0 || t_y >= height || t_x < 0 || t_x >= width) continue; 

			if(mat[t_y][t_x] != Type(empty) && mat[t_y][t_x] != Type(end)) continue;

			//check if it's a diagonal to be pushed back 
			if((t_y != py && t_x != px)){
				if(diag_checked){
					neigh.push_back(point{(unsigned int)t_y, (unsigned int)t_x});
				}
				continue;
			}

			//creating begin_address to copy digonal point to
			auto b_a = neigh.begin();
			//neigh.push_back(point{(unsigned int)t_y, (unsigned int)t_x});
			b_a = neigh.insert(b_a, point{(unsigned int)t_y, (unsigned int)t_x});
		}
	}

	return neigh;
}

bool in_dict(point point, std::map<struct point, struct point>& t_map){
	auto res = t_map.find(point);
	if(res != t_map.end()) return true;
	return false;
}
