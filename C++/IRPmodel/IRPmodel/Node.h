#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;
class Node
{
public:
	class Edge {
	public:
		Edge(Node& endNode, double value);
		double getValue();
		Node *getEndNode();
	private:
		double Value;
		Node &EndNode;
	};

	Node(int id, vector <Edge> edges);
	Node(int id);
	int getId() const;
	Edge * getEdge(int);
	vector <Edge> * getEdges();
	void addEdge(double value, Node & child);
	int getIndex();
	void setIndex(int);
	void setLowLink(int);
	void setOnStack(bool);
	int getLowLink();
	bool isOnStack();
	~Node();


	bool operator==(const Node &) const; 
	bool operator!=(const Node &node) const
	{
		return !(*this == node); // invokes Array::operator==
	}
	

private:
	int NodeID;
	
	vector <Edge> Edges;
	int Index = -1;
	int LowLink = -1;
	bool onStack;
	vector <Node> Nodes;
};

