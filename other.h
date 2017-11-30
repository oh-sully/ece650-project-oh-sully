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

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);


//prints msg and the cpu time
static void pclock(char *msg, clockid_t cid);


//stores the information for the vertices relationships (e.g. the edges)
class Matrix {
private:
    int rows = 0, cols = 0, edgenum = 0;
public:
    std::vector<bool> matrix;

    Matrix();

    Matrix(int n, int k, bool str_value);
    
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


//outputs "PROGRAM_NAME: # # # # # #"
void vc_output(std::string algorithm, std::vector<int> vc);


//Struct for passing arguments to the threads
struct ArgsStruct {
    std::string user_input;
    Matrix edges;
    int num_vert, num_edges;
    std::vector<int> vc_list;
};


//thread for VC1 algorithm
void *VC1_thread(void *args);


//Thread for VC2 algorithm
void *VC2_thread(void *args);


//Thread for VCSAT algorithm(a4)
void *VCSAT_thread(void *args);


//thread for io
void *io_thread(void *args);