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
#include "ece650-a4.hpp"
#include <cstdlib>
#include <deque>
#include <iomanip>
#include <locale>
//for sqrt()
#include <math.h>
#include <typeinfo>

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

//prints msg and the cpu time
double pclock(/*char *msg, */clockid_t cid){
    struct timespec ts;
    char buffer[20];
    double CPUtime;

    if (clock_gettime(cid, &ts) == -1){
        std::cout << "Error with gettime" << std::endl;
    }
    sprintf(buffer, "%4ld.%06ld\n", ts.tv_sec, ts.tv_nsec / 1000);
    CPUtime = std::stod(buffer);
    //std::cout << "pclock = " << buffer << std::endl;
    return CPUtime;
}

double vecvectomean(std::vector< std::vector<double> > data){
    double mean = 0;
    int N = 0;
    for (unsigned int aa = 0; aa < data.size(); aa++){
        for (unsigned int ab = 0; ab < data[aa].size(); ab++){
            mean += data[aa][ab];
            N++;
        }
    }
    mean = mean / N;
    return mean;
}

double vectomean(std::vector<double> data){
    double mean = 0;
    int N = 0;
    for (unsigned int aa = 0; aa < data.size(); aa++){
        mean += data[aa];
        N++;
    }
    mean = mean / N;
    return mean;
}

double vecvectosd(std::vector< std::vector<double> > data){
    double u = vecvectomean(data);
    double sd = 0;
    int N = 0;
    for (unsigned int aa = 0; aa < data.size(); aa++){
        for (unsigned int ab = 0; ab < data[aa].size(); ab++){
            sd += (data[aa][ab] - u) * (data[aa][ab] - u);
            N++;
        }
    }
    sd = sd / N;
    sd = sqrt(sd);
    return sd;
}

double vectosd(std::vector<double> data){
    double u = vectomean(data);
    double sd = 0;
    int N = 0;
    for (unsigned int aa = 0; aa < data.size(); aa++){
        sd += (data[aa] - u) * (data[aa] - u);
        N++;
    }
    sd = sd / N;
    sd = sqrt(sd);
    return sd;
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
};


//outputs "PROGRAM_NAME: # # # # # #"
void vc_output(std::string algorithm, std::vector<int> vc){
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


//Struct for passing arguments to the threads
struct ArgsStruct {
    std::string user_input;
    Matrix edges;
    int num_vert;
    int num_edges;
    std::vector<int>* vc_list;
    std::vector< std::pair<int,int> > Edge;
    std::vector<double>* CPUtimes; //[V # graph][CPUtimes]
};


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
        (*(VC1Args->vc_list)).push_back(most_edges);
        edges_cpy.clear_edges(most_edges);
    }
    

    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode){
        std::cerr << "Error with the damn retcode SAT" << std::endl;
    }
    else{
        (*(VC1Args->CPUtimes)).push_back(pclock(cid));
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
                (*(VC2Args->vc_list)).push_back(r);
                (*(VC2Args->vc_list)).push_back(c);
                edges_cpy.clear_edges(r);
                edges_cpy.clear_edges(c);
                break;
            }
        }
    }
    

    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode){
        std::cerr << "Error with the damn retcode SAT" << std::endl;
    }
    else{
        (*(VC2Args->CPUtimes)).push_back(pclock(cid));
    }
    pthread_exit(NULL);
}

//Thread for VCSAT algorithm(a4)
void *VCSAT_thread(void *args){
    struct ArgsStruct *VCSATArgs;
    VCSATArgs = (struct ArgsStruct *) args;

    const std::vector< std::pair<int,int> > E_arg = VCSATArgs->Edge;
    const double n = (double)VCSATArgs->num_vert;

    int final_k = n;

    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    double upper_bound = n;
    double lower_bound = 0;
    //while((upper_bound - lower_bound)>=1){
    bool flag = true;

    while(flag){
        double k = ceil(((upper_bound-lower_bound)/2)+lower_bound);

        std::vector<std::vector<Minisat::Lit>> literals(k, std::vector<Minisat::Lit>(n));

        // populate atomic proposition matrix
        for (int position_col = 0; position_col < k; ++position_col){
            std::vector<Minisat::Lit> vars(n);
            for(int vertex_row = 0; vertex_row < n; ++vertex_row){
                //Minisat::Lit l1 = Minisat::mkLit(solver->newVar());
                //Minisat::Lit literals[position_col][vert_row];
                literals[position_col][vertex_row] = Minisat::mkLit(solver->newVar());
            }
        }

        // add first clause - at least one vertex is in ith poisiton of vertex cover
        for (int position_col = 0; position_col < k; ++position_col){
            Minisat::vec<Minisat::Lit> constr;
            for (int vertex_row = 0; vertex_row < n; ++vertex_row){
                constr.push(literals[position_col][vertex_row]);
            }
            solver->addClause(constr);
            }

        // add second clause - no one vertex can appear twice in a vertex cover
        for (int vertex_row = 0; vertex_row < n; ++vertex_row ){
            for (int position_col = 0; position_col < (k-1); ++position_col ){
                for (int comb = (position_col+1); comb < k; ++comb){
                    Minisat::vec<Minisat::Lit> constr;
                    constr.push(~literals[position_col][vertex_row]);
                    constr.push(~literals[comb][vertex_row]);
                    solver->addClause(constr);
                }
            }

        }

        // add third clause - no more than one vertex appears in the mth position of the vertex cover
        for (int position_col = 0; position_col < k; ++position_col ){
            for (int vertex_row = 0; vertex_row < (n-1); ++vertex_row ){
                for (int comb = (vertex_row+1); comb < n; ++comb){
                    Minisat::vec<Minisat::Lit> constr;
                    constr.push(~literals[position_col][vertex_row]);
                    constr.push(~literals[position_col][comb]);
                    solver->addClause(constr);
                }
            }

        }

        // add fourth clause - every edge is incident to at least one vertex in the vertex cover
        int num_edges = E_arg.size();
        for (int edge_index = 0; edge_index < num_edges; ++edge_index){
            Minisat::vec<Minisat::Lit> constr;
            int vertex1 = E_arg[edge_index].first;
            int vertex2 = E_arg[edge_index].second;
            for (int vertex1_pos = 0; vertex1_pos < k; ++vertex1_pos)
            {
                constr.push(literals[vertex1_pos][vertex1]);
            }
            for (int vertex2_pos = 0; vertex2_pos < k; ++vertex2_pos)
            {
                constr.push(literals[vertex2_pos][vertex2]);
            }
            solver->addClause(constr);
        }

        bool res = solver->solve();

        if ((upper_bound-lower_bound)<=1 ){
            std::vector<int> output;
            for (int position = 0; position < final_k; ++position){
                for (int vertex = 0; vertex < n; ++vertex){
                    int vert_cover = Minisat::toInt(solver->modelValue(literals[position][vertex]));
                    if (vert_cover == 0){
                        output.emplace_back(vertex);
                        flag = false;
                    }
                }
            }
            *(VCSATArgs->vc_list) = output;
        }

        if (res==1){
        // current k creates satisfiable vertex secondVert
            upper_bound = k;
            final_k = k;
        }
        else if (res==0){
            lower_bound = k;
            upper_bound = final_k;
        }
        solver.reset (new Minisat::Solver());
    }


    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);
    if(retcode){
        std::cerr << "Error with the damn retcode SAT" << std::endl;
    }
    else{
        (*(VCSATArgs->CPUtimes)).push_back(pclock(cid));
    }

    pthread_exit(NULL);
}

//thread for io
void *io_thread(void *args){

    struct ArgsStruct ioArgs;
    std::vector<int> vc_list;
    std::vector<double> CPUtimes;
    std::vector< std::vector<double> > totVC1times;
    std::vector< std::vector<double> > totVC2times;
    std::vector< std::vector<double> > totSATtimes;
    std::vector<std::pair<int,int>> Edge;
    ioArgs.user_input = "V 0";
    ioArgs.edges = Matrix(0,0,0);
    ioArgs.num_vert = 0;
    ioArgs.vc_list = &vc_list;
    ioArgs.CPUtimes = &CPUtimes;
    std::ifstream graphs ("../graphs-input.txt"); //to remove when ready to submit
    int vert1, vert2, count = 0;
    char command;
    std::string edges_str, check_str;

    pthread_t VCSAT_pid, VC1_pid, VC2_pid;
    struct ArgsStruct VCSATArgs, VC1Args, VC2Args;
    int create_VCSAT, create_VC1, create_VC2;

    std::vector< std::vector<double> > means;
    std::vector<double> SATmeans;
    std::vector<double> VC1means;
    std::vector<double> VC2means;
    std::vector< std::vector<double> > stddev; //[algorithm][vertex]
    std::vector<double> SATstddev;
    std::vector<double> VC1stddev;
    std::vector<double> VC2stddev;

    double vcstd = 0;
    std::vector< std::vector<double> > ratios;
    std::vector<double> VC1rmeans;
    std::vector<double> VC2rmeans;
    std::vector<double> VC1ratios;
    std::vector<double> VC2ratios;
    double VC1ratio;
    double VC2ratio;
    std::vector< std::vector<double> > rsd;
    std::vector<double> VC1rsd;
    std::vector<double> VC2rsd;

    while(true){
        
        if (graphs.is_open()){
            getline(graphs, ioArgs.user_input);
        }
        else{
            std::cerr << "Error: unable to open file" << std::endl;
        }
        // replace the above with the below when ready to submit
        //getline(std::cin, ioArgs->user_input);
        //if (std::cin.eof()) {
        //    break;
        //}
        std::istringstream iss(ioArgs.user_input);
        iss >> command;    
        if (command == 'V') {
            iss >> ioArgs.num_vert;
            ioArgs.edges = Matrix(ioArgs.num_vert, ioArgs.num_vert, 0);
            continue;
        }
        else if (command == 'E'){
            iss >> edges_str;
            //edges_str is {<#,#>,<#,#>,<#,#>}
            ReplaceStringInPlace(edges_str, "{<", "{< ");
            ReplaceStringInPlace(edges_str, ",", " , ");
            ReplaceStringInPlace(edges_str, "> , <", " >,< ");
            ReplaceStringInPlace(edges_str, ">}", " >}");
            //edges_str is now {< # , # >,< # , # >,< # , # >}
            std::istringstream isss(edges_str);
            isss >> check_str;
            //if no edges print a blank line
            if (check_str == "{}" || check_str == "{ }"){
                std::cout << std::endl;
                continue;
            }
            //while you haven't found '>}' aka then end of the input, add the edges to the matrix
            while (check_str != ">}") {
                isss >> vert1;
                isss >> check_str;
                isss >> vert2;
                isss >> check_str;
                ioArgs.edges.edit(vert1, vert2, true);
                ioArgs.edges.edit(vert2, vert1, true);
                ioArgs.num_edges++;
                ioArgs.Edge.push_back(std::make_pair(vert1,vert2));
            }
        }

        VCSATArgs.user_input = ioArgs.user_input;
        VCSATArgs.edges = ioArgs.edges;
        VCSATArgs.num_vert = ioArgs.num_vert;
        VCSATArgs.num_edges = ioArgs.num_edges;
        VCSATArgs.Edge = ioArgs.Edge;
        VCSATArgs.vc_list = ioArgs.vc_list;
        VCSATArgs.CPUtimes = ioArgs.CPUtimes;
        ioArgs.Edge.erase(ioArgs.Edge.begin(), ioArgs.Edge.end());
        count++;
        //set range to run_number < 10 when you want the 10 runs
        for (int run_number = 0; run_number < 10; run_number++){
            
            create_VCSAT = pthread_create(&VCSAT_pid, NULL, VCSAT_thread, (void *)&VCSATArgs);
            if (create_VCSAT != 0){
                std::cerr << "Error: Couldn't create VCSAT thread; error #" << create_VCSAT << std::endl;
            }
            pthread_join(VCSAT_pid, NULL);
        }
        vc_output("CNF-SAT-VC", vc_list);
        vcstd = (double)vc_list.size();
        vc_list.erase(vc_list.begin(), vc_list.end());
        totSATtimes.push_back(CPUtimes);
        CPUtimes.clear();

        //sets the arguments for the threads in their respective structs
        VC1Args.user_input = ioArgs.user_input;
        VC1Args.edges = ioArgs.edges;
        VC1Args.num_vert = ioArgs.num_vert;
        VC1Args.vc_list = ioArgs.vc_list;
        VC1Args.CPUtimes = ioArgs.CPUtimes;
        for (int run_number = 0; run_number < 10; run_number++){
            create_VC1 = pthread_create(&VC1_pid, NULL, VC1_thread, (void *)&VC1Args);
            if (create_VC1 != 0){
                std::cerr << "Error: Couldn't create VC1 thread; error #" << create_VC1 << std::endl;
            }
            pthread_join(VC1_pid, NULL);
            if (run_number == 9){
                vc_output("APPROX-VC-1", vc_list);
                VC1ratio =((double)vc_list.size()) / vcstd;
            }
            vc_list.erase(vc_list.begin(), vc_list.end());
        }
        totVC1times.push_back(CPUtimes);
        CPUtimes.clear();

        VC2Args.user_input = ioArgs.user_input;
        VC2Args.edges = ioArgs.edges;
        VC2Args.num_vert = ioArgs.num_vert;
        VC2Args.vc_list = ioArgs.vc_list;
        VC2Args.CPUtimes = ioArgs.CPUtimes;
        for (int run_number = 0; run_number < 10; run_number++){
            create_VC2 = pthread_create(&VC2_pid, NULL, VC2_thread, (void *)&VC2Args);
            if (create_VC2 != 0){
                std::cerr << "Error: Couldn't create VC2 thread; error #" << create_VC2 << std::endl;
            }
            pthread_join(VC2_pid, NULL);
            if (run_number == 9){
                vc_output("APPROX-VC-2", vc_list);
                VC2ratio = ((double)vc_list.size()) / vcstd;
            }
            vc_list.erase(vc_list.begin(), vc_list.end());
        }
        totVC2times.push_back(CPUtimes);
        CPUtimes.clear();
        
        VC1ratios.push_back(VC1ratio);
        VC2ratios.push_back(VC2ratio);

        if((count % 10) == 0){
            SATmeans.push_back(vecvectomean(totSATtimes));
            VC1means.push_back(vecvectomean(totVC1times));
            VC2means.push_back(vecvectomean(totVC2times));
            SATstddev.push_back(vecvectosd(totSATtimes));
            VC1stddev.push_back(vecvectosd(totVC1times));
            VC2stddev.push_back(vecvectosd(totVC2times));
            VC1rmeans.push_back(vectomean(VC1ratios));
            VC2rmeans.push_back(vectomean(VC2ratios));
            VC1rsd.push_back(vectosd(VC1ratios));
            VC2rsd.push_back(vectosd(VC2ratios));

            std::cout << "----------------------------------------------------------------------" << std::endl;
            for (int cc = 0; cc < totSATtimes.size(); cc++){
                for (int cd = 0; cd < totSATtimes[cc].size(); cd++){
                    std::cout << "totSATtimes[" << cc << "][" << cd << "] = " << totSATtimes[cc][cd] << std::endl;
                }
            }
            for (int cc = 0; cc < totVC1times.size(); cc++){
                for (int cd = 0; cd < totVC1times[cc].size(); cd++){
                    std::cout << "totVC1times[" << cc << "][" << cd << "] = " << totVC1times[cc][cd] << std::endl;
                }
            }

            std::cout << "----------------------------------------------------------------------" << std::endl;
            std::cout << "mean SATtimes: " << vecvectomean(totSATtimes) << std::endl;
            std::cout << "mean VC1times: " << vecvectomean(totVC1times) << std::endl;
            //std::cout << "mean VC2times: " << vecvectomean(totVC2times) << std::endl;
            std::cout << "mean SATstddev: " << vecvectosd(totSATtimes) << std::endl;
            std::cout << "mean VC1stddev: " << vecvectosd(totVC1times) << std::endl;
            //std::cout << "mean VC2stddev: " << vecvectosd(totVC2times) << std::endl;
            std::cout << "----------------------------------------------------------------------" << std::endl;

            totSATtimes.clear();
            totVC1times.clear();
            totVC2times.clear();
        }
        if (graphs.eof()) {
            break;
        }
    }
    graphs.close();

    means.push_back(SATmeans);
    means.push_back(VC1means);
    means.push_back(VC2means);
    stddev.push_back(SATstddev);
    stddev.push_back(VC1stddev);
    stddev.push_back(VC2stddev);
    ratios.push_back(VC1rmeans);
    ratios.push_back(VC2rmeans);
    rsd.push_back(VC1rsd);
    rsd.push_back(VC2rsd);


    std::vector<int> X;
    X.clear();
    //X.push_back(5);
    //X.push_back(7);
    //X.push_back(9);
    //X.push_back(11);
    //X.push_back(13);
    //X.push_back(15);
    X.push_back(14);
    std::ofstream datafile ("../datafile.dat");//to remove when ready to submit
    datafile << "#X      SATtime        SATsd        VC1time        VC1sd        VC2time       VC2sd     r1  rsd1  r2  rsd2" << std::endl;//remove when ready to submit
    datafile.close();//remove when ready to submit

    datafile.open("../datafile.dat", std::ios::out | std::ios::app);//to remove when ready to submit

    for(unsigned int bb = 0; bb < X.size(); bb++){
        datafile << X[bb];
        for (unsigned int bc = 0; bc < means.size(); bc++){
            datafile << "    " << means[bc][bb] << "    " << stddev[bc][bb];
        }
        for (unsigned int bc = 0; bc < ratios.size(); bc++){
            datafile << "    " << ratios[bc][bb] << "    " << rsd[bc][bb];
        }
        datafile << std::endl;
    }

    datafile.close();
    pthread_exit(NULL);
}


//main function, duh    
int main() {

    pthread_t io_pid;
    int create_io;

    create_io = pthread_create(&io_pid, NULL, io_thread, NULL);
    if (create_io != 0){
        std::cerr << "Error: Couldn't create io thread; error #" << create_io << std::endl;
    }
    pthread_join(io_pid, NULL);
    
    return 0;
}