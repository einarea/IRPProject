#include "stdafx.h"
#include "Node.h"


Node::Node(int id, vector<Edge*> edges)
	:
	NodeID(id),
	Edges(edges)
{
}

Node::Node(const Node & cpNode)
	:
	NodeID(cpNode.NodeID),
	State(cpNode.State)
{
	//Do not copy the edges
}



Node::Node(int id)
	:
	NodeID(id)
{

}

Node & Node::operator=(const Node &cpNode)
{
	for (Edge *edge: Edges)
		delete edge;

	return * new Node(cpNode);
}
	

void Node::setId(int id)
{
	NodeID = id;
}

int Node::getState()
{
	return State;
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

Node::Edge::~Edge()
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

bool Node::hasEdge(Edge * checkEdge)
{
	for (auto edge : getEdges()) {
		if (edge->getEndNode()->getId() == checkEdge->getEndNode()->getId())
			return true;
	}

	return false;
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

void Node::setState(int s)
{
	State = s;
}

void Node::deleteEdges()
{
	for(Node::Edge* edge : Edges){
		delete edge;
	}
	Edges.resize(0);
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
	//Release dynamic memory
	deleteEdges();
}
