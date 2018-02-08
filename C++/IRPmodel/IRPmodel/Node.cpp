#include "stdafx.h"
#include "Node.h"



Node::Node(int id, vector<Edge&> edges)
	:
	Edges(edges),
	NodeID(id),
	onStack(true)
{
}

Node::Node(int id)
	:
	NodeID(id),
	onStack(true)
{
}

int Node::getId() {
	return NodeID;
}

void Node::addEdge(double value, Node child)
{
	Edges.push_back(Edge(child, value));
}

Node::~Node()
{
}

bool Node::operator==(const Node & node) const
{
	if(this->getId == node.getId)
	return true;
}

class Node::Edge
{
public:
	Edge(Node& endNode, double value);
private:
	double Value;
	vector <Node&> Children;

};

Node::Edge::Edge(Node & endNode, double value)
{
	Value = value;
	Children.push_back(endNode);
}
