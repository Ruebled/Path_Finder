#ifndef ALGO_H
#define ALGO_H

#include <unistd.h>
#include <string>
#include <map>
#include <queue>
#include <chrono>
#include <unordered_map>

#include "Matrix.h"
#include "Grid.h"
#include "TimeTracker.h"

void BreathFirstSearch(Matrix& matrix);


void Dijkstra(Matrix& matrix);
void AStart(Matrix& matrix);

std::vector<point> get_neighbors(Matrix& mat, point t_point);

struct CElement {
	point location;
	double value;

	friend bool operator<(const CElement& l, const CElement& r) {
        return	l.value < r.value; 
    }

	friend bool operator>(const CElement& l, const CElement& r) {
        return	l.value > r.value; 
    }
};



#endif
