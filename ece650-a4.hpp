#include <memory>

#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <deque>
#include <algorithm>

void cnf_sat(const double n, const std::vector <std::pair<int,int>>E_arg);
