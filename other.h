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

class Matrix {
private:
    int rows = 0, cols = 0, edgenum = 0;
public:
    std::vector<bool> matrix;

    Matrix() {
        rows = 0;
        cols = 0;
        matrix.resize(0, 0);
    }

    Matrix(int n, int k, bool str_value) {
        rows = n;
        cols = k;
        matrix.resize(rows * cols, str_value);
    }
    
    void edit(int n, int k, bool value);
    
    int value(int n, int k);
    //returns the number of edges with vertex 'v'
    int num_of_edges(int v);
    //returns the total number of edges
    int num_of_edges();
    //sets to 0 all edges that contain vertex 'v'
    void clear_edges(int v);
   //prints the adjacency matrix
    void print();
};

//thread for io
void *io_thread(void *args);