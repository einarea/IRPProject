#pragma once
#include <vector>;

using namespace std;
class Node
{

public:
	class Edge {
	public:
		Edge(Node& endNode);
		Node *getEndNode();

	private:
		Node &EndNode;
	};


	Node(int id);
	Node(int id, vector<Edge> edges);
	Edge * getEdge(int);
	int getEdge(Node & child);

	//Used when linked to one other node. i.e. a route
	Node * getNextNode();

	vector <Edge> * getEdges();
	void addEdge(Node & child);
	void removeEdge(Node & child);
	int getId() const;
	~Node();

	bool operator==(const Node &) const;
	bool operator!=(const Node &node) const
	{
		return !(*this == node); // invokes Array::operator==
	}



	

protected:
	int NodeID;
	vector <Edge> Edges;

};

