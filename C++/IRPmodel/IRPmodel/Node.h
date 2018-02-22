#pragma once
#include <vector>;

using namespace std;
class Node
{

public:
	class Edge {
	public:
		Edge(Node& endNode, double value);
		double getValue();
		void setValue(double);
		Node *getEndNode();

	private:
		double Value;
		Node &EndNode;

	};


	Node(int id);
	Node(int id, vector<Edge> edges);
	Edge * getEdge(int);
	vector <Edge> * getEdges();
	void addEdge(double value, Node & child);
	int getId() const;
	~Node();

	bool operator==(const Node &) const;
	bool operator!=(const Node &node) const
	{
		return !(*this == node); // invokes Array::operator==
	}



	

private:
	int NodeID;
	vector <Edge> Edges;

};

