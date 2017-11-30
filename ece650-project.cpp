#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <pthread.h>

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

class Matrix {
private:
    int rows = 0, cols = 0, edgenum = 0;
public:
    std::vector<bool> matrix;

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

    void clear_edges(int v) {
        for (int c = 0; c < cols; c++) {
            this->edit(v, c, 0);
        }
        for (int r = 0; r < rows; r++){
            this->edit(r, v, 0);
        }
    }
   
    void print() {        
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++){
                std::cout << this->value(r,c) << "  ";
            }
            std::cout << std::endl;
        }
    }
};

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

class ioArgsClass {
public:
    std::string* user_input;
    Matrix* edges;
    int* num_vert;
    int* result;
};

int parse_input_into_matrix(ioArgsClass ioArgs){
    
    int vert1;
    int vert2;
    char command;
    std::string edges_str;
    std::istringstream iss(ioArgs.user_input);
    iss >> command;    
    if (command == 'V') {
        iss >> ioArgs.num_vert;
        ioArgs.edges = Matrix(ioArgs.num_vert, ioArgs.num_vert, 0);
        return 1;
    }
    else if (command == 'E'){
        iss >> edges_str;
        ReplaceStringInPlace(edges_str, "{<", "{< ");
        ReplaceStringInPlace(edges_str, ",", " , ");
        ReplaceStringInPlace(edges_str, "> , <", " >,< ");
        ReplaceStringInPlace(edges_str, ">}", " >}");
        std::istringstream isss(edges_str);
        std::string check_str;
        isss >> check_str;
        if (check_str == "{" || check_str == "{ " || check_str == "{}" || check_str == "{ }"){
            return -1;
        }
        while (check_str != ">}") {
            isss >> vert1;
            isss >> check_str;
            isss >> vert2;
            isss >> check_str;
            ioArgs.edges.edit(vert1, vert2, true);
            ioArgs.edges.edit(vert2, vert1, true);
        }
        return 0;
    }
    return 1;
}

void* io_thread(ioArgsClass ioArgs){

    ioArgs.result = 2;

    while(ioArgs.result != 0){

        if (graphs.is_open()){
            getline(graphs, ioArgs.user_input);
            if (graphs.eof()) {
                ioArgs.result = -2;
                break;
            }
        }
        else{
            std::cerr << "Error: unable to open file" << std::endl;
        }
        /* replace the above with the below when ready to submit
        getline(std::cin, ioArgs.user_input);
        if (std::cin.eof()) {
            break;
        }
        */
        ioArgs.result = parse_input_into_matrix(ioArgs);
        if (ioArgs.result == 2){
            std::cerr << "Error: result failed" << std::endl;
            break;
        }
    }

}
    
int main() {
    
    std::string user_input;
    int result = 2, num_vert, most_edges = -1, create_thread;
    Matrix edges = Matrix(0, 0, 0), edges_cpy1 = Matrix(0, 0, 0), edges_cpy2 = Matrix(0, 0, 0);
    std::vector<int> approx_vc1, approx_vc2;
    std::ifstream graphs ("graphs-input.txt"); //to remove when ready to submit
    std::ofstream datafile ("datafile.dat");//to remove when ready to submit
    pthread_t io_pid, VC1_pid, VC2_pid;

    ioArgsClass ioArgs;
    ioArgs.num_vert = num_vert;
    ioArgs.edges = edges;
    ioArgs.user_input = user_input;
    ioArgs.graphs = graphs;


    
    while(result != -2){
        /*
        if (graphs.is_open()){
            getline(graphs, user_input);
            if (graphs.eof()) {
                break;
            }
        }
        else{
            std::cerr << "Error: unable to open file" << std::endl;
        }
        /* replace the above with the below when ready to submit
        getline(std::cin, user_input);
        if (std::cin.eof()) {
            break;
        }
        * /
        result = parse_input_into_matrix(user_input, edges, num_vert);
        */
        //if command 'V' or unknown command

        create_thread = pthread_create(&io_pid, NULL, io_thread, ioArgs);
        if (create_thread != 0){
            std::cerr << "Error: Couldn't create io thread; error #" << create_thread << std::endl;
        }
        pthread_join(io_pid, NULL);

        if (result == -2 || result == 2){
            break;
        }
        else if (result == 1){ //if command 'V'
            continue;
        }
        //if command 'E' with no edges
        else if (result == -1){
            std::cout << std::endl;
            continue;
        }
        //command 'E' or unknown return value?
        else{
            //APPROX-VC-1
            edges_cpy1 = edges;
            while (true) {
                most_edges = 0;
                for (int v = 0; v < num_vert; v++) {
                    if (edges_cpy1.num_of_edges(v) > edges_cpy1.num_of_edges(most_edges)) {
                        most_edges = v;
                    }
                }
                if (edges_cpy1.num_of_edges() == 0){
                    break;
                }
                approx_vc1.push_back(most_edges);
                edges_cpy1.clear_edges(most_edges);
            }
            vc_output("APPROX-VC-1", approx_vc1);
            approx_vc1.erase(approx_vc1.begin(), approx_vc1.end());


            //APPROX-VC-2
            edges_cpy2 = edges;
            for (int r = 0; r < num_vert; r++){
                //adds both vertices from the edge to 
                for (int c = 0; c < num_vert; c++){
                    if (edges_cpy2.value(r,c) == true) {
                        approx_vc2.push_back(r);
                        approx_vc2.push_back(c);
                        edges_cpy2.clear_edges(r);
                        edges_cpy2.clear_edges(c);
                        break;
                    }
                }
            }
            vc_output("APPROX-VC-2", approx_vc2);
            approx_vc2.erase(approx_vc2.begin(), approx_vc2.end());
        }
    }
    graphs.close();
    datafile.close();
    return 0;
}