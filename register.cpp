/// Class Definitions - Register
#include "register.hpp"
#include "vertexAdjList.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <deque>

Register::Register()
{
	Vertices = 0;
	Edges.clear();
	Paths.first = 0;
	Paths.second = 0;
}

void Register::setVertices(int vert)
{
	Vertices = vert;
}

void Register::setEdge(std::vector<std::pair<int, int> > edge)
{
	Edges = edge;
}

void Register::setPath(std::pair <int, int> path)
{
	Paths = path;
}

double Register::getVertices()
{
	return Vertices;
}

void Register::printPath()
{

	std::vector<VertexAdj> selectedVertices(Vertices, VertexAdj());
	for (size_t i = 0; i < Edges.size(); ++i)
	{
		int vert1 = Edges[i].first;				// Populate Adjacency list with the Edges vector of Register
		int vert2 = Edges[i].second;

		selectedVertices[vert1].setAdj(vert2);
		selectedVertices[vert2].setAdj(vert1);
	}

	int source = Paths.first;					// Pull source vertex
	int sink = Paths.second;					// Pull sink vertex

	selectedVertices[source].setColor(1);		// Set parameters of the source vertex to that of a source vertex
	selectedVertices[source].setDist(0);
	selectedVertices[source].setPre(-1);

	deque<int> queue;
	int u, v;

	queue.push_back(source);

	while (queue.size() != 0)
	{

		u = queue.front();
		queue.pop_front();

		for (int n = 0; n < selectedVertices[u].getAdjSize(); ++n)					// for each v in the adjacency list of u
		{
			v = selectedVertices[u].getAdj(n);										// set v to the nth vertex in adjacency list of u
			if (selectedVertices[v].getColor() == 0)								// if v is white
			{
				selectedVertices[v].setColor(1);									// set v to to gray
				selectedVertices[v].setDist(selectedVertices[u].getDist() + 1);		// set distance to distance of u + 1
				selectedVertices[v].setPre(u);										// set predecessor of v to u
				queue.push_back(v);													// move v to queue
			}
		}
		selectedVertices[u].setColor(2);											// after iterating through adjacency list, set u to black
	}

	int disconnect = 0;
	if (selectedVertices[sink].getColor() == 0)
	{
		disconnect = 1;
	}

	if (disconnect == 0)
	{
		std::vector <int> finalPath;
		int current = sink;

		for (int m = 0; m < selectedVertices[sink].getDist(); ++m)
		{
			finalPath.push_back(current);
			current = selectedVertices[current].getPre();
		}
		finalPath.push_back(source);

		for (int k = finalPath.size(); k > 0; --k)
		{
			if (k != 1)
			{
				cout << finalPath[k - 1] << "-";
			}
			else
			{
				cout << finalPath[k - 1] << endl;
			}
		}
	}
	else
	{
		std::cerr << "Error: There is no path from source to sink" << "\n";
	}

}
