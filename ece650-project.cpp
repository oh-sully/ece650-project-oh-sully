#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
// defined std::unique_ptr
//#include <memory>
// defines Var, Lit, l_True, and l_False
//#include "minisat/core/SolverTypes.h"
// defines Solver
//#include "minisat/core/Solver.h"
// defines vec
//#include "minisat/mtl/Vec.h"

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

class Matrix {
    std::vector<bool> matrix;
private:
    int rows = 0, cols = 0, edgenum = 0;
public:
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

    int num_of_edges(int v){
    	edgenum = 0;
    	for (int i = 0; i < v; i++){
    		if (this->matrix.value(v,i) == true){
    			edgenum++;
    		}
    	}
    	return edgenum;
    }

    void clear_copyrow(int v) {
   		for (int i = 0; i < v; j++) {
    		this->edit(v, i, 0);
    	}
    }

    void copy_all(){
    	for (int i = 0; i < rows; i ++) {
    		for (int j = i + 1; j < cols; j++) {
    			this->edit(j, i, this->value(i,j));
    		}
    	}
    }

};
    
int main() {
    
    std::string edges_str;
    std::string user_input;
    char command;
    int num_vert;
    int num_edges;
    //int cover_size;
    int vert1;
    int vert2;
    unsigned int k;
    unsigned int up_k;
    unsigned int low_k;
    Matrix edges = Matrix(0, 0, 0);
    bool sat_flag;
    std::vector<int> vcov;
    bool sat = false;
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    Minisat::vec<Minisat::Lit> lits;
    Minisat::vec<Minisat::Lit> clause;
    //int num_lits;
    //int num_clause;
    std::vector<int> approx-vc1;
    std::vector<int> approx-vc2;
    bool cleared_flag = 0;
    int most_edges = -1;
    
    while(true){
        
        //num_lits = 0; //*REMOVE* when ready to submit
        //num_clause = 0; //*REMOVE* when ready to submit
        user_input = "";
        command = NULL;
        sat_flag = 0;
        num_edges = 0;
        cleared_flag = 0;
        
        getline(std::cin, user_input);
        if (std::cin.eof()) {
            break;
        }
        std::istringstream iss(user_input);
        iss >> command;
        
        if (command == 'V'){
            iss >> num_vert;
            edges = Matrix(0, 0, 0);
            vcov.resize(num_vert, -1);
            //cover_size = num_vert;
            edges = Matrix(num_vert, num_vert, 0);
        }
        //reads the edges from the user and stores them in the matrix 'edges'
        else if (command == 'E'){
            iss >> edges_str;
            ReplaceStringInPlace(edges_str, "{<", "{< ");
            ReplaceStringInPlace(edges_str, ",", " , ");
            ReplaceStringInPlace(edges_str, "> , <", " >,< ");
            ReplaceStringInPlace(edges_str, ">}", " >}");
            
            std::istringstream isss(edges_str);
            std::string check_str;
            isss >> check_str;
            //If no edges found, print a blank line
            if (check_str == "{" || check_str == "{ " || check_str == "{}" || check_str == "{ }"){
                std::cout << std::endl;
                continue;
            }
            while (check_str != ">}") {
                isss >> vert1;
                isss >> check_str;
                isss >> vert2;
                isss >> check_str;
                /*
                if (vert1 < 0 || vert1 > (num_vert - 1) || vert2 < 0 || vert2 > (num_vert - 1)) {
                    std::cerr << "Error: Vertex out of range" << std::endl;
                    break;
                }
                */
                edges.edit(vert1, vert2, true);
                edges.edit(vert2, vert1, true);
                num_edges++;
            }
            
            //print adjecency matrix
            
            for (int r = 0; r < num_vert; r++){
            	for (int c = 0; c < num_vert; c++){
            		std::cout << edges.value(r,c) << "  ";
            	}
            	std::cout << std::endl;
            }

        	//APPROX-VC-1
        	while (true) {
        		most_edges = -1;
            	for (int v = 0; v < num_vert; v++) {
            		if (edges.num_of_edges(v) > most_edges){
            			most_edges = v;
            		}
            	}
            	if (most_edges == 0){
            		break;
            	}
            	approx-vc1.push_back(most_edges);
            	edges.clear_copyrow(most_edges);
            }
            std::sort(approx-vc1.begin(), approx-vc1.end());
            std::cout << "APPROX-VC-1: "
            for (int s = 0; s < approx-vc1.size(); s++) {
            	std::cout << approx-vc1[s] << " ";
            }
            std::cout << std::endl;
            edges.copy_all();


    		//APPROX-VC-2
            for (int r = 1; a < num_vert; a++){
            	//checks to see if this vertex was already included in an edge
            	for (int s = 0; s < approx-vc2.size(); s++) {
            		if (r == approx-vc2[s]){
            			cleared_flag == 1;
            			break;
            		}
            	}
            	//if a checked edge already had this vertex, skip this vertex
            	if (cleared_flag == 1) {
            		cleared_flag = 0;
            		continue;
            	}
            	//adds both vertices from the edge to 
            	for (int c = 0; c < r; c ++){
            		if (edges.value(r,c) == true) {
            			approx-vc2.push_back(r);
            			approx-vc2.push_back(c);
            			edges.clear_copyrow(r);
            			edges.clear_copyrow(c);
            			break;
            		}
            	}
            }
            //sort and print min vertex cover
            std::sort(approx-vc2.begin(), approx-vc2.end());
            std::cout << "APPROX-VC-2: "
            for (int s = 0; s < approx-vc2.size(); s++) {
            	std::cout << approx-vc2[s] << " ";
            }
            std::cout << std::endl;
            edges.copy_all();


        }
    }

    return 0;
}