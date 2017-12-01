#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <deque>

class Register
{

private:
	double Vertices;       // Number of Vertices
	std::vector<std::pair<int, int>> Edges;  // Edges in current Register
	std::pair <int, int> Paths;

public:
	Register();
	void setVertices(int vert);
	void setEdge(std::vector<std::pair<int, int> > edge);
	void setPath(std::pair <int, int> path);
	double getVertices();
	void printPath();
};
