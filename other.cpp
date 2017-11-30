#include "other.hpp"

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}


//prints msg and the cpu time
static void pclock(char *msg, clockid_t cid){
    struct timespec ts;

    printf("%s", msg);
    if (clock_gettime(cid, &ts) == -1){
        std::cout << "Error with gettime" << std::endl;
    }
    printf("%4ld.%06ld\n", ts.tv_sec, ts.tv_nsec / 1000);
}


//stores the information for the vertices relationships (e.g. the edges)
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
    
    void edit(int n, int k, bool value) {
        this->matrix[cols * n + k] = value;
    }
    
    int value(int n, int k) {
        return this->matrix[cols * n + k];
    }
    //returns the number of edges with vertex 'v'
    int num_of_edges(int v){
        edgenum = 0;
        for (int c = 0; c < cols; c++){
            if (this->value(v,c) == true){
                edgenum++;
            }
        }
        return edgenum;
    }
    //returns the total number of edges
    int num_of_edges(){
        for (int r = 0; r < rows; r++){
            for (int c = 0; c < cols; c++){
                if (this->value(r,c) == true){
                    edgenum++;
                }
            }
        }
        edgenum = edgenum / 2;
        return edgenum;
    }
    //sets to 0 all edges that contain vertex 'v'
    void clear_edges(int v) {
        for (int c = 0; c < cols; c++) {
            this->edit(v, c, 0);
        }
        for (int r = 0; r < rows; r++){
            this->edit(r, v, 0);
        }
    }
   //prints the adjacency matrix
    void print() {        
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++){
                std::cout << this->value(r,c) << "  ";
            }
            std::cout << std::endl;
        }
    }
};


//outputs "PROGRAM_NAME: # # # # # #"
vc_output(std::string algorithm, std::vector<int> vc){
    std::sort(vc.begin(), vc.end());
    std::cout << algorithm << ": ";
    for (unsigned int s = 0; s < vc.size(); s++) {
        std::cout << vc[s];
        if (s != vc.size() - 1){
            std::cout << ",";
        }
        else{
            std::cout << std::endl;
        }
    }
}

//thread for VC1 algorithm
void *VC1_thread(void *args){
    struct ArgsStruct *VC1Args;
    VC1Args = (struct ArgsStruct *) args;
    int most_edges;
    Matrix edges_cpy = VC1Args->edges;

    while (true) {
        most_edges = 0;
        for (int v = 0; v < VC1Args->num_vert; v++) {
            if (edges_cpy.num_of_edges(v) > edges_cpy.num_of_edges(most_edges)) {
                most_edges = v;
            }
        }
        if (edges_cpy.num_of_edges() == 0){
            break;
        }
        VC1Args->vc_list.push_back(most_edges);
        edges_cpy.clear_edges(most_edges);
    }
    vc_output("APPROX-VC-1", VC1Args->vc_list);
    VC1Args->vc_list.erase(VC1Args->vc_list.begin(), VC1Args->vc_list.end());
    

    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode){
        std::cerr << "Error with the damn retcode SAT" << std::endl;
    }
    else{
        pclock("VCSAT CPU Time:   ", cid);
    }


    pthread_exit(NULL);
}


//Thread for VC2 algorithm
void *VC2_thread(void *args){
    struct ArgsStruct *VC2Args;
    VC2Args = (struct ArgsStruct *) args;
    Matrix edges_cpy = VC2Args->edges;

    edges_cpy = VC2Args->edges;
    for (int r = 0; r < VC2Args->num_vert; r++){
        //adds both vertices from the edge to 
        for (int c = 0; c < VC2Args->num_vert; c++){
            if (edges_cpy.value(r,c) == true) {
                VC2Args->vc_list.push_back(r);
                VC2Args->vc_list.push_back(c);
                edges_cpy.clear_edges(r);
                edges_cpy.clear_edges(c);
                break;
            }
        }
    }
    vc_output("APPROX-VC-2", VC2Args->vc_list);
    VC2Args->vc_list.erase(VC2Args->vc_list.begin(), VC2Args->vc_list.end());
    

    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode){
        std::cerr << "Error with the damn retcode SAT" << std::endl;
    }
    else{
        pclock("VC2 CPU Time:   ", cid);
    }


    pthread_exit(NULL);
}


//Thread for VCSAT algorithm(a4)
void *VCSAT_thread(void *args){
    struct ArgsStruct *VCSATArgs;
    VCSATArgs = (struct ArgsStruct *) args;

    
    unsigned int k, up_k, low_k;
    bool sat_flag = false, sat = false;
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    Minisat::vec<Minisat::Lit> lits;
    Minisat::vec<Minisat::Lit> clause;

    //if VCSATArgs only has one edge, either vertex is a correct vertex cover
    //if (VCSATArgs->num_edges == 1){
    //    VCSATArgs->vc_list.push_back(vert1) //but figure out proper implimentation since vert1 isn't in scope
    //    continue;
    //}
    
    up_k = VCSATArgs->num_vert;
    low_k = 1;
    k = (up_k - low_k) / 2 + low_k - 1; //Int division, if num_vert is odd, then division is num_vert/2 - 0.5
    if (k == 1){
        k = 2;
    }
    while(true){

        //creates num_vert positive literals for each i in [1,k] (lits.size() = VCSAT->num_vert*k)
        for(int x = 0; x < VCSATArgs->num_vert * k; x++){
            lits.push(Minisat::mkLit(solver->newVar()));
        }
        // Adds the clauses for contraint 1
        for (int c = 0; c < k; c++){
            for (int n = 0; n < VCSATArgs->num_vert; n++){
                clause.push(lits[n*k + c]);
            }
            //std::cout << std::endl;
            solver->addClause(clause);
            while(clause.size() > 0){
                clause.pop();
            }
        }
        // Adds the clauses for contraint 2
        // No vertex can appear twice in a vertex cover (!clause[0] || !clause[1])
        for (int n = 0; n < VCSATArgs->num_vert; n++){
            for (int c = 0; c < (k-1); c++){
                for (int i = c+1; i < k; i++){
                    solver->addClause(~lits[n*k + c], ~lits[n*k + i]);
                }
            }
        }
        // Adds the clauses for contraint 3
        // No more than one vertex appears in the same position in a vertex cover (!clause[0] || !clause[1])
        for (int c = 0; c < k; c++){
            for (int n = 0; n < (VCSATArgs->num_vert-1); n++){
                for (int i = n+1; i < VCSATArgs->num_vert; i++){
                    solver->addClause(~lits[n*k + c], ~lits[i*k + c]);
                }
            }
        }
        // Adds the clauses for contraint 4
        // Adds edges contraints; At least one vertex in every edge must be in the vertex cover
        for (int row = 0; row < (VCSATArgs->num_vert - 1); row++){
            for (int col = (row + 1); col < VCSATArgs->num_vert; col++){ //skips comparing the same vertex
                
                if (VCSATArgs->edges.value(row,col) == true){
                    for (int c = 0; c < k; c++){
                        clause.push(lits[row*k + c]);
                        clause.push(lits[col*k + c]);
                    }
                    solver->addClause(clause);
                    while(clause.size() > 0){
                        clause.pop();
                    }
                }
            }
        }

        sat = solver->solve();

        //binary search; changing k depending on whether k was satisfiable or not
        if (sat == 1){
            //if sat, make sure previous vc_list is of size 'k' filled with '-1's
            VCSATArgs->vc_list.resize(0);
            VCSATArgs->vc_list.resize(k, -1);

            for (int c = 0; c < k; c++){
                for (int n = 0; n < VCSATArgs->num_vert; n++){
                    //if true, add vertex 'n' to the vertex cover
                    if (solver->modelValue(lits[n*k+c]) == Minisat::l_True){
                        VCSATArgs->vc_list.push_back(n);
                    }
                    else if (solver->modelValue(lits[n*k+c]) == Minisat::l_False){
                        continue;
                    }
                }
            }

            sat_flag = 1;

            //if we reached the end of the binary search, break out of while loop
            if (up_k == k || low_k == k){
                break;
            }
            else{
                up_k = k;
                k = (up_k - low_k) / 2 + low_k; //Int division
            }
        }
        else if (sat == 0){
           //if we reached the end of the binary search, break out of while loop
            if (up_k == k || low_k == k){
                break;
            }
            else{
                low_k = k;
                k = (up_k - low_k) / 2 + low_k;
            }
        }
    } //end of reduction/binary search
    //if no sat conditions were found for any 'k' attempted, set vertex cover to all vertices
    if (sat_flag == 0){
        VCSATArgs->vc_list.resize(0);
        VCSATArgs->vc_list.resize(VCSATArgs->num_vert);
        for (int j = 0; j < VCSATArgs->num_vert; j++){
            VCSATArgs->vc_list[j] = j;
        }
    }
    //print outout
    vc_output("CNF-SAT-VC", VCSATArgs->vc_list);
    //clear vectors and reset solver
    while(lits.size() > 0){
        lits.pop();
    }
    solver.reset(new Minisat::Solver());
    VCSATArgs->vc_list.erase(VCSATArgs->vc_list.begin(), VCSATArgs->vc_list.end());
    


    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode){
        std::cerr << "Error with the damn retcode SAT" << std::endl;
    }
    else{
        pclock("VCSAT CPU Time:   ", cid);
    }

    pthread_exit(NULL);
}