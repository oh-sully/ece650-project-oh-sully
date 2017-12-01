#include "vertexAdjList.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <deque>

VertexAdj::VertexAdj()
{
	Color = 0;
	Predecessor = NULL;
	Dist = 1000;
}

void VertexAdj::setColor(int color)
{
	Color = color;
}

void VertexAdj::setPre(int pre_id)
{
	Predecessor = pre_id;
}

void VertexAdj::setDist(int dist)
{
	Dist = dist;
}

void VertexAdj::setAdj(int vert)
{
	Adj.push_back(vert);
}

int VertexAdj::getDist()
{
	return Dist;
}

int VertexAdj::getAdjSize()
{
	return Adj.size();
}

int VertexAdj::getAdj(int n)
{
	return Adj[n];
}

int VertexAdj::getColor()
{
	return Color;
}

int VertexAdj::getPre()
{
	return Predecessor;
}