#include "stdafx.h"
#include "NodeStrong.h"



NodeStrong::NodeStrong(int id, vector<Edge> edges)
	:
	Node(id, edges),
	onStack(true)
{
}

NodeStrong::NodeStrong(int id)
	:
	Node(id),
	onStack(false)
{
}

NodeStrong::NodeStrong(Node * node)
	:
	Node(*node)
{
}


int NodeStrong::getIndex()
{
	return Index;
}


void NodeStrong::setIndex(int ind)
{
	Index = ind;
}

void NodeStrong::setLowLink(int lnk)
{
	LowLink = lnk;
}

void NodeStrong::setOnStack(bool bl)
{
	onStack = bl;
}

int NodeStrong::getLowLink()
{
	return LowLink;
}

void NodeStrong::addEdge(double value, Node & child)
{
	this->Edges.push_back(EdgeStrong(child, value));
}

NodeStrong * NodeStrong::getStrongNode(Node* node)
{
	NodeStrong * derivedPtr = static_cast <NodeStrong *> (node);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}



bool NodeStrong::isOnStack()
{
	return onStack;
}

NodeStrong::~NodeStrong()
{
}


NodeStrong::EdgeStrong::EdgeStrong(Node & child, double value)
	:
Edge(child),
Value(value)
{
}

double NodeStrong::EdgeStrong::getValue()
{
	return Value;
}

NodeStrong::EdgeStrong * NodeStrong::EdgeStrong::getStrongEdge(Edge * edge)
{
	EdgeStrong * derivedPtr = static_cast <EdgeStrong *> (edge);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}

void NodeStrong::EdgeStrong::setValue(double v)
{
 Value = v;
}
