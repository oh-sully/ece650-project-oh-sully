#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

//Replaces strings with other strings
void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);

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
};

//takes the user input and puts it into the matrix 'edges'
int parse_input_into_matrix(std::string &user_input, Matrix &edges, int &num_vert);

//sorts and sends to std::cout the min vertex cover
void vc_output(std::string algorithm, std::vector<int> vc);