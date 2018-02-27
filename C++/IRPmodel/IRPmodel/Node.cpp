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

//Ret
int Node::getEdge(Node & child)
{
	for (int i = 0; i < Edges.size(); i++)
		if (child.getId() == Edges[i].getEndNode()->getId()) {
			return i;
		}
}


Node * Node::getNextNode()
{
	if (Edges.size() >= 0)
		return getEdge(0)->getEndNode();
}

vector<Node::Edge>* Node::getEdges()
{
	return &Edges;
}

Node::Edge::Edge(Node & endNode)
	:
	EndNode(endNode)
{
}

bool Node::operator==(const Node & node) const
{
	if (this->getId() == node.getId())
		return true;
	else
		return false;
}

void Node::addEdge(Node &child)
{
	this->Edges.push_back(Edge(child));
}

void Node::removeEdge(Node & child)
{
	Edges.erase(Edges.begin() + getEdge(child));
}



Node *Node::Edge::getEndNode()
{
	return &EndNode;
}



Node::~Node()
{
}
