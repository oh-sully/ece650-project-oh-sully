#include "parse_line.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <deque>

bool parse_line(const std::string &line,
	char &cmd, double &V_arg, std::vector <std::pair<int, int> >&E_arg, std::pair <int, int> &S_arg, std::string &err_msg) {

	std::istringstream input(line);

	// remove whitespace
	ws(input);

	if (input.eof()) {
		err_msg = "Empty command";
		return false;
	}

	char ch;
	input >> ch;

	if (input.fail()) {
		err_msg = "Failed to read input";
		return false;
	}


	if (ch == 'V') {
		ws(input);
		double num;
		input >> num;
		if (input.fail()) {
			err_msg = "Missing or bad argument";
			return false;
		}
		ws(input);
		if (!input.eof()) {
			err_msg = "Unexpected argument";
			return false;
		}
		cmd = ch;
		V_arg = num;
		return true;
	}
	else if (ch == 'E') {
		std::vector<std::pair<int, int>> Edge;
		std::string delimiter = "{<";
		int pos = 0;
		int value1, value2;
		int first = 1, second = 0;
		std::string stringLength = line;

		while ((pos = stringLength.find(delimiter)) != std::string::npos) {

			if (delimiter != "{<") {
				std::string token = stringLength.substr(0, pos);
				if (first == 1 && second != 1) {
					value1 = std::atoi(token.c_str());
					first = 0;
					second = 1;
				}
				else if (first == 0 && second != 0) {
					value2 = std::atoi(token.c_str());
					first = 1;
					second = 0;
					Edge.push_back(std::make_pair(value1, value2));
				}

			}

			stringLength.erase(0, pos + delimiter.length());
			if (delimiter == "{<") {
				delimiter = ",";
			}
			else if (delimiter == "," && stringLength.length() != 3) {
				delimiter = ">,<";
			}
			else if (delimiter == "," && stringLength.length() == 3) {
				delimiter = ">}";
			}
			else if (delimiter == ">,<") {
				delimiter = ",";
			}

		}

		cmd = ch;
		if (Edge.empty()){
			return false;
		}
		else {
			E_arg = Edge;
			return true;
		}
	}
	else if (ch == 's') {
		ws(input);
		int s1, s2;
		input >> s1 >> s2;
		cmd = ch;
		S_arg = std::make_pair(s1, s2);
		return true;
	}
	else {
		err_msg = "Unknown command";
		return false;
	}

}
