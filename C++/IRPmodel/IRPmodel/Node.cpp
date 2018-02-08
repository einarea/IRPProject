#include "stdafx.h"
#include "Node.h"



Node::Node(int id, vector<Edge> edges)
	:
	Edges(edges),
	NodeID(id),
	onStack(true)
{
}

Node::Node(int id)
	:
	NodeID(id),
	onStack(false)
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

void Node::addEdge(double value, Node &child)
{
	this->Edges.push_back(Edge(child, value));
}

int Node::getIndex()
{
	return Index;
}

void Node::setIndex(int ind)
{
	Index = ind;
}

void Node::setLowLink(int lnk)
{
	LowLink = lnk;
}

void Node::setOnStack(bool bl)
{
	onStack = bl;
}

int Node::getLowLink()
{
	return LowLink;
}

bool Node::isOnStack()
{
	return onStack;
}

Node::~Node()
{
}

bool Node::operator==(const Node & node) const
{
	if (this->getId() == node.getId())
		return true;
	else
		return false;
}


Node::Edge::Edge(Node & endNode, double value)
	:
	EndNode(endNode),
	Value(value)
{
}

double Node::Edge::getValue()
{
	return Value;
}

Node * Node::Edge::getEndNode()
{
	return &EndNode;
}
