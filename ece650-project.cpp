#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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
    
int main() {
    
    std::string edges_str;
    std::string user_input;
    char command;
    int num_vert;
    int vert1;
    int vert2;
    Matrix edges = Matrix(0, 0, 0);
    Matrix edges_cpy = Matrix(0, 0, 0);
    std::vector<int> approx_vc1;
    std::vector<int> approx_vc2;
    int most_edges = -1;
    
    while(true){
        
        user_input   = "";
        command      = 'x';
        
        getline(std::cin, user_input);
        if (std::cin.eof()) {
            break;
        }
        std::istringstream iss(user_input);
        iss >> command;
        
        if (command == 'V'){
            iss >> num_vert;
            edges = Matrix(num_vert, num_vert, 0);
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
                edges.edit(vert1, vert2, true);
                edges.edit(vert2, vert1, true);
            }



        	//APPROX-VC-1
            edges_cpy = edges;
        	while (true) {
        		most_edges = 0;
            	for (int v = 0; v < num_vert; v++) {
            		if (edges_cpy.num_of_edges(v) > edges_cpy.num_of_edges(most_edges)) {
            			most_edges = v;
            		}
            	}
            	if (edges_cpy.num_of_edges() == 0){
            		break;
            	}
            	approx_vc1.push_back(most_edges);
                edges_cpy.clear_edges(most_edges);

            }
            vc_output("APPROX-VC-1", approx_vc1);
            approx_vc1.erase(approx_vc1.begin(), approx_vc1.end());




    		//APPROX-VC-2
            edges_cpy = edges;
            for (int r = 0; r < num_vert; r++){
            	//adds both vertices from the edge to 
            	for (int c = 0; c < num_vert; c++){
            		if (edges_cpy.value(r,c) == true) {
            			approx_vc2.push_back(r);
            			approx_vc2.push_back(c);
            			edges_cpy.clear_edges(r);
            			edges_cpy.clear_edges(c);
            			break;
            		}
            	}
            }
            vc_output("APPROX-VC-2", approx_vc2);
            approx_vc2.erase(approx_vc2.begin(), approx_vc2.end());
        }
    }
    return 0;
}