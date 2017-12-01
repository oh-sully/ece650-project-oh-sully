#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <deque>

bool parse_line(const std::string &line,
	char &cmd, double &V_arg, std::vector <std::pair<int, int> > &E_arg, std::pair <int, int> &S_arg, std::string &err_msg);
