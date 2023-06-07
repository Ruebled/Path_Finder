#include "Algo.h"

// distance of the path
int distance_path = 0;

// cost of the path
float cost_path = 0;

// Global variable for thread status
extern bool thread_active;

// Diagonal calc 
bool diag_checked = false;

inline double get_cell_priority(int value){
	double priority = 0;
	switch(value){
		case Type(empty):
			priority = 1;
			break;
		case Type(sand):
			priority = 3;
			break;
		case Type(woods):
			priority = 5;
			break;
		case Type(water):
			priority = 7;
			break;
		case Type(mountains):
			priority = 9;
			break;
	}
	return priority;
}

void 
BreathFirstSearch(Matrix& matrix){
	// State of the found path
	bool path_found = false;

	// The code
	point start_point = matrix.get_start_point();

	// Create PriorityQueue for storing the neighbors
	//PriorityQueue<point, double> neigh;
	std::priority_queue<CElement, std::vector<CElement>, std::greater<CElement>> neigh;

	neigh.push(CElement{start_point, 0});

	// Queue to store all neighbors
	std::deque<point> neigh_reverse;

	std::map<point, point> came_from;
	std::map<point, double> cost_so_far;

	came_from[start_point] = start_point;
	cost_so_far[start_point] = 0;

	std::map<point, double> cost_stats;
	cost_stats[start_point] = 0;

	// Define in outer scope the current point that is checked
	point current;

	// Create TimeTracker class for time counting
	TimeTracker timeTracker;

	// Start the stopwatch for real time
	timeTracker.rlt_start();

	while(!neigh.empty()){
		// Start time
		timeTracker.cpu_start();

		current = neigh.top().location;
		neigh.pop();

		if(matrix[current.y][current.x] == Type(end)){
			path_found = true;
			cost_path = cost_stats[current];
			break;
		}
		// Get a <point> vector of current location neighbors
		std::vector<point> near = get_neighbors(matrix, current);

		for (int i = 0; i < near.size(); i++) {
			//find if near[i] cell is diagonaly placed 
			//referring current cell
			bool diagonal = (current.y != near[i].y && current.x != near[i].x);
			//find the cost for the next tile(cell)
			double new_cost = cost_so_far[current] + (diagonal?1.4:1);

			//
			double stat_cost = cost_stats[current] + (diagonal?0.4:0) + get_cell_priority(matrix[near[i].y][near[i].x]);

			if (cost_so_far.find(near[i]) == cost_so_far.end() 
					|| new_cost < cost_so_far[near[i]]) {

				if(cost_so_far.find(near[i]) == cost_so_far.end()){
					if(matrix[near[i].y][near[i].x] != Type(end)){
						matrix[near[i].y][near[i].x] = Type(visited) + matrix[near[i].y][near[i].x];
					}
				}
	
				// map to store the cost(plus of the cells)
				cost_stats[near[i]] = stat_cost;

				cost_so_far[near[i]] = new_cost; 
				came_from[near[i]] = current;
				neigh.push(CElement{near[i], new_cost});

				neigh_reverse.push_back(near[i]);
			}
		}

		timeTracker.cpu_pause();

		// Visited delay
		usleep(20*1000);
	}

	timeTracker.cpu_stop();

	// After algorithm processing

	// Reverse the found path
	if(path_found){
		while(current != start_point){
			current = came_from[current];	
			if(matrix[current.y][current.x] != Type(start)){
				matrix[current.y][current.x] = matrix[current.y][current.x] - Type(visited) + Type(path);
			}
			distance_path++;
			// Path delay
			usleep(15*1000);
		}
		// subtrack one(stop point) from distance path
		distance_path--;
	}

	// Clear all visited cells
	point rev;
	while(!neigh_reverse.empty()){
		rev = neigh_reverse.back();
		neigh_reverse.pop_back();
		if(matrix[rev.y][rev.x] >= Type(visited) && matrix[rev.y][rev.x] < Type(path)){
			matrix[rev.y][rev.x] = matrix[rev.y][rev.x] - Type(visited);
			// Remove delay
			usleep(10*1000);
		}
	}

	// Pause the real time stopwatch
	timeTracker.rlt_pause();

	// Time real in miliseconds, till path found
	timeTracker.rlt_stop();

	thread_active = false;
	return;
}

void 
Dijkstra(Matrix& matrix){
	// State of the found path
	bool path_found = false;

	// The code
	point start_point = matrix.get_start_point();

	// Create PriorityQueue for storing the neighbors
	//PriorityQueue<point, double> neigh;
	std::priority_queue<CElement, std::vector<CElement>, std::greater<CElement>> neigh;

	neigh.push(CElement{start_point, 0});

	// Queue to store all neighbors
	std::deque<point> neigh_reverse;

	std::map<point, point> came_from;
	std::map<point, double> cost_so_far;

	came_from[start_point] = start_point;
	cost_so_far[start_point] = 0;

	// Define in outer scope the current point that is checked
	point current;

	// Create TimeTracker class for time counting
	TimeTracker timeTracker;

	// Start the stopwatch for real time
	timeTracker.rlt_start();

	while(!neigh.empty()){
		// Start time
		timeTracker.cpu_start();

		current = neigh.top().location;
		neigh.pop();

		if(matrix[current.y][current.x] == Type(end)){
			path_found = true;
			cost_path = cost_so_far[current];
			break;
		}
		// Get a <point> vector of current location neighbors
		std::vector<point> near = get_neighbors(matrix, current);
		
		for (int i = 0; i < near.size(); i++) {
			//find if near[i] cell is diagonaly placed 
			//referring current cell
			bool diagonal = (current.y != near[i].y && current.x != near[i].x);
			//find the cost for the next tile(cell)
			double new_cost = cost_so_far[current] + 
					(get_cell_priority(matrix[near[i].y][near[i].x]%10)) + (diagonal?0.5:0);

			if (cost_so_far.find(near[i]) == cost_so_far.end() 
					|| new_cost < cost_so_far[near[i]]) {

				if(cost_so_far.find(near[i]) == cost_so_far.end()){
					if(matrix[near[i].y][near[i].x] != Type(end)){
						matrix[near[i].y][near[i].x] = Type(visited) + matrix[near[i].y][near[i].x];

					}
				}

				cost_so_far[near[i]] = new_cost; 
				came_from[near[i]] = current;
				neigh.emplace(CElement{near[i], new_cost});

				neigh_reverse.push_back(near[i]);

			}
		}

		timeTracker.cpu_pause();

		// Visited delay
		usleep(20*1000);
	}

	timeTracker.cpu_stop();

	// After algorithm processing

	// Reverse the found path
	if(path_found){
		while(current != start_point){
			current = came_from[current];	
			if(matrix[current.y][current.x] != Type(start)){
				matrix[current.y][current.x] += Type(path) - Type(visited);
			}
			distance_path++;
			// Path delay
			usleep(15*1000);
		}
		// subtrack one(stop point) from distance path
		distance_path--;
	}

	// Clear all visited cells
	point rev;
	while(!neigh_reverse.empty()){
		rev = neigh_reverse.back();
		neigh_reverse.pop_back();
		if(matrix[rev.y][rev.x] >= Type(visited) && matrix[rev.y][rev.x] < Type(path)){
			matrix[rev.y][rev.x] = matrix[rev.y][rev.x] - Type(visited);
			// Remove delay
			usleep(10*1000);
		}
	}

	// Pause the real time stopwatch
	timeTracker.rlt_pause();

	// Time real in miliseconds, till path found
	timeTracker.rlt_stop();

	thread_active = false;
	return;
}

inline double heuristic(point a, point b) {
	return std::abs((int)a.x - (int)b.x) + std::abs((int)a.y - (int)b.y);
}

void 
AStar(Matrix& matrix){
	// State of the found path
	bool path_found = false;

	// The code
	point start_point = matrix.get_start_point();
	point end_point = matrix.get_end_point();

	// Create PriorityQueue for storing the neighbors
	//PriorityQueue<point, double> neigh;
	std::priority_queue<CElement, std::vector<CElement>, std::greater<CElement>> neigh;

	neigh.push(CElement{start_point, heuristic(start_point, end_point)});

	// Queue to store all neighbors
	std::deque<point> neigh_reverse;

	std::map<point, point> came_from;
	std::map<point, double> cost_so_far;

	came_from[start_point] = start_point;
	cost_so_far[start_point] = 0;

	// Define in outer scope the current point that is checked
	point current;

	// Create TimeTracker class for time counting
	TimeTracker timeTracker;

	// Start the stopwatch for real time
	timeTracker.rlt_start();

	while(!neigh.empty()){
		// Start time
		timeTracker.cpu_start();

		current = neigh.top().location;
		neigh.pop();

		if(matrix[current.y][current.x] == Type(end)){
			path_found = true;
			cost_path = cost_so_far[current];
			break;
		}

		// Get a <point> vector of current location neighbors
		std::vector<point> near = get_neighbors(matrix, current);

		for (int i = 0; i < near.size(); i++) {
			//find if near[i] cell is diagonaly placed 
			//referring current cell
			bool diagonal = (current.y != near[i].y && current.x != near[i].x);
			//find the cost for the next tile(cell)
			double new_cost = cost_so_far[current] + 
					(get_cell_priority(matrix[near[i].y][near[i].x]%10)) + (diagonal?0.5:0);

			if (cost_so_far.find(near[i]) == cost_so_far.end() 
					|| new_cost < cost_so_far[near[i]]) {

				if(cost_so_far.find(near[i]) == cost_so_far.end()){
					if(matrix[near[i].y][near[i].x] != Type(end)){
						matrix[near[i].y][near[i].x] = Type(visited) + matrix[near[i].y][near[i].x];
					}
				}

				cost_so_far[near[i]] = new_cost; 
				double priority = new_cost + heuristic(near[i], end_point);
				neigh.emplace(CElement{near[i], priority});
				came_from[near[i]] = current;

				neigh_reverse.push_back(near[i]);
			}
		}

		timeTracker.cpu_pause();

		// Visited delay
		usleep(20*1000);
	}

	timeTracker.cpu_stop();

	// After algorithm processing

	// Reverse the found path
	if(path_found){
		while(current != start_point){
			current = came_from[current];	
			if(matrix[current.y][current.x] != Type(start)){
				matrix[current.y][current.x] = matrix[current.y][current.x] - Type(visited) + Type(path);
			}
			distance_path++;
			// Path delay
			usleep(15*1000);
		}
		// subtrack one(stop point) from distance path
		distance_path--;
	}

	// Clear all visited cells
	point rev;
	while(!neigh_reverse.empty()){
		rev = neigh_reverse.back();
		neigh_reverse.pop_back();
		if(matrix[rev.y][rev.x] >= Type(visited) && matrix[rev.y][rev.x] < Type(path)){
			matrix[rev.y][rev.x] = matrix[rev.y][rev.x] - Type(visited);
			// Remove delay
			usleep(10*1000);
		}
	}

	// Pause the real time stopwatch
	timeTracker.rlt_pause();

	// Time real in miliseconds, till path found
	timeTracker.rlt_stop();

	thread_active = false;
	return;
}


// Function to extract the neighbors cells
std::vector<point> 
get_neighbors(Matrix& mat, point t_point){
	std::vector<point> neigh;

	int width = mat.width();
	int height = mat.height();

	int px = t_point.x;
	int py = t_point.y;

	// Some magic (half asleep)
	for(int row=-1; row<2; row++){
		int t_y = py + row;
		for(int col=-1; col<2; col++){
			int t_x = px + col;
			if(t_y < 0 || t_y >= height || t_x < 0 || t_x >= width) continue; 

			if(mat[t_y][t_x] == Type(start) || 
				 mat[t_y][t_x] == Type(wall) ||
				  mat[t_y][t_x] == Type(visited)
				) continue;

			//check if it's a diagonal to be pushed back 
			if((t_y != py && t_x != px) && !diag_checked) continue;

			neigh.push_back(point{t_y, t_x});
		}
	}

	std::random_shuffle(neigh.begin(), neigh.end());

	return neigh; 
}
