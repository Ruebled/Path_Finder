#ifndef ALGO_H
#define ALGO_H

#include <unistd.h>
#include <string>
#include <map>
#include <queue>
#include <chrono>

#include "Matrix.h"
#include "Grid.h"

void BreathFirstSearch(Matrix& matrix);
void AStart(Matrix& matrix);

std::vector<point> get_neighbors(Matrix& mat, point t_point);
bool in_dict(point point, std::map<struct point, struct point>& t_map);

#endif
