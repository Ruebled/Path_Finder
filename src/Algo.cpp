#include "Algo.h"

// Global variables performance report
// variable cpu_time 
float cpu_time = 0;

// cpu_time variable text
float real_time = 0;

// distance of the path
int distance_path = 0;

// Global variable for thread status
extern bool thread_active;

// Diagonal calc 
bool diag_checked = false;

void BreathFirstSearch(Matrix& matrix){
	//procedure BFS(G, root) is
	//    let Q be a queue
	//    label root as explored
	//    Q.enqueue(root)
	//    while Q is not empty do
	//        v := Q.dequeue()
	//        if v is the goal then
	//            return v
	//        for all edges from v to w in G.adjacentEdges(v) do
	//            if w is not labeled as explored then
	//                label w as explored
	//                w.parent := v
	//                Q.enqueue(w)
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

	std::chrono::duration<double> cpu_chrono_time(0);

	auto real_time_start = std::chrono::steady_clock::now();
	while(!neigh.empty()){
		// Start time
		auto cpu_time_start = std::chrono::steady_clock::now();

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
			// Stop real_time computing
			auto real_time_end = std::chrono::steady_clock::now();
			std::chrono::duration<double> real_elapsed_time = real_time_end - real_time_start;
			// Get real_time in milliseconds
			int real_time_int = std::chrono::floor<std::chrono::milliseconds>(real_elapsed_time).count();

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

			// Time in miliseconds of real till path found
			real_time = (real_time_int)/1000 + 
				((float) (real_time_int%1000))/1000;

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

		auto cpu_time_end = std::chrono::steady_clock::now();
		std::chrono::duration<double> cpu_elapsed_time = cpu_time_end - cpu_time_start;

		cpu_chrono_time += cpu_elapsed_time;

		// Visited delay
		usleep(20*1000);
	}

	int cpu_time_int = std::chrono::floor<std::chrono::nanoseconds>(cpu_chrono_time).count();

	// Time in miliseconds
	cpu_time = (float)(cpu_time_int)/1000000 + ((float) (cpu_time_int%1000000))/1000000;

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
