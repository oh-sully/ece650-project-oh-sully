#include <iostream>
#include <string>
#include <vector>
#include "ece650-project.h"
    
int main() {
    
    std::string user_input;
    int result;
    int num_vert;
    Matrix edges = Matrix(0, 0, 0);
    Matrix edges_cpy = Matrix(0, 0, 0);
    std::vector<int> approx_vc1;
    std::vector<int> approx_vc2;
    int most_edges = -1;
    
    while(true){
        
        getline(std::cin, user_input);
        if (std::cin.eof()) {
            break;
        }
        result = parse_input_into_matrix(user_input, edges, num_vert);
        if (result == 1){
            continue;
        }
        else if (result == -1){
            std::cout << std::endl;
            continue;
        }
        else{
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