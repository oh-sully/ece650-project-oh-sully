#pragma once
using namespace std;

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <deque>

class VertexAdj
{
private:
	int Color; // 0 = white, 1 = gray, 2 = black
	int Predecessor;
	int Dist;
	vector <int> Adj;


public:
	VertexAdj();
	void setColor(int color);
	void setPre(int pre_id);
	void setDist(int dist);
	void setAdj(int id);
	int getDist();
	int getAdjSize();
	int getAdj(int n);
	int getColor();
	int getPre();

};