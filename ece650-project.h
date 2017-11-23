#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

//Replaces strings with other strings
void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);

//takes the user input and puts it into the matrix 'edges'
int parse_input_into_matrix(std::string &user_input, Matrix &edges, int &num_vert);

//sorts and sends to std::cout the min vertex cover
void vc_output(std::string algorithm, std::vector<int> vc);


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

    int num_of_edges(int v){
    	edgenum = 0;
    	for (int c = 0; c < cols; c++){
    		if (this->value(v,c) == true){
    			edgenum++;
    		}
    	}
    	return edgenum;
    }

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

int parse_input_into_matrix(std::string &user_input, Matrix &edges, int &num_vert){
    
    int vert1;
    int vert2;
    char command;
    std::string edges_str;
    std::istringstream iss(user_input);
    iss >> command;    
    if (command == 'V') {
        iss >> num_vert;
        edges = Matrix(num_vert, num_vert, 0);
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
            edges.edit(vert1, vert2, true);
            edges.edit(vert2, vert1, true);
        }
        return 0;
    }
    return -1;
}