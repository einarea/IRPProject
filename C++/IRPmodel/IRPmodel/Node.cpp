#include "stdafx.h"
#include "Node.h"



Node::Node(int id)
	:
	NodeID(id)
{
}

Node::Node(int id, vector<Edge> edges)
	:
	NodeID(id),
	Edges(edges)
{
}

int Node::getId() const {
	return NodeID;
}

Node::Edge * Node::getEdge(int id)
{
	return &Edges[id];
}

vector<Node::Edge>* Node::getEdges()
{
	return &Edges;
}

Node::Edge::Edge(Node & endNode, double value)
	:
	EndNode(endNode),
	Value(value)
{
}

bool Node::operator==(const Node & node) const
{
	if (this->getId() == node.getId())
		return true;
	else
		return false;
}

void Node::addEdge(double value, Node &child)
{
	this->Edges.push_back(Edge(child, value));
}

void Node::Edge::setValue(double v)
{
	Value = v;
}

Node *Node::Edge::getEndNode()
{
	return &EndNode;
}



Node::~Node()
{
}
