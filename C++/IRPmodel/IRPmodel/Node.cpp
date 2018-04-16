#include "stdafx.h"
#include "Node.h"



Node::Node(int id)
	:
	NodeID(id)
{
}

Node::Node(int id, vector<Edge*> edges)
	:
	NodeID(id),
	Edges(edges)
{
}

int Node::getId() const {
	return NodeID;
}



Node::Edge * Node::getEdge(Node & child)
{
	for (int i = 0; i < Edges.size(); i++) {
		if (child.getId() == Edges[i]->getEndNode()->getId()) {
			return Edges[i];
		}
		
	}
	exit(111);
}

int Node::getnEdges()
{
	return Edges.size();
}

Node::Edge * Node::getEdge()
{
	return Edges[0];
}



vector<Node::Edge*> Node::getEdges()
{
	return Edges;
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

void Node::addEdge(Edge* edge)
{
	this->Edges.push_back(edge);
}

void Node::addEdge(double value, Node *child)
{
	this->Edges.push_back(new Edge(*child, value));
}

void Node::addEdge(Node * child)
{
	addEdge(1, child);
}

void Node::removeEdge(Node & child)
{
	for (int i = 0; i < Edges.size(); i++) {
		if (child.getId() == Edges[i]->getEndNode()->getId()) {
			Edges.erase(Edges.begin() + i);
		}
	}
}

void Node::removeEdges()
{
	int size = Edges.size();
	for (int i = size-1; i >= 0; i--) {
		Edges.erase(Edges.begin() + i);
		}
}

void Node::deleteEdges()
{
	int size = Edges.size();
	for (int i = size - 1; i >= 0; i--) {
		Edges.erase(Edges.begin() + i);
	}
}

void Node::deleteEdge(Node * node)
{
	for (int i = 0; i < Edges.size(); i++) {
		if(Edges[i]->getEndNode() == node)
			Edges.erase(Edges.begin() + i);
	}
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
