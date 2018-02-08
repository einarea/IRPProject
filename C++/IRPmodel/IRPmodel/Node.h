#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;
class Node
{
public:
	class Edge;
	Node(int id, vector <Edge&> edges);
	Node(int id);
	int getId();
	void addEdge(double value, Node child);
	~Node();
	bool operator==(const Node &) const; 
	bool operator!=(const Node &node) const
	{
		return !(*this == node); // invokes Array::operator==
	}
	

private:
	int NodeID;
	bool onStack;
	vector <Edge&> Edges;
	int Index;
	int LowIndex;
	vector <Node> Nodes;
};

