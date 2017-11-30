#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <pthread.h>
#include <time.h>
// defined std::unique_ptr
#include <memory>
// defines Var, Lit, l_True, and l_False
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
// defines vec
#include "minisat/mtl/Vec.h"

//thread for io
void *io_thread(void *args);