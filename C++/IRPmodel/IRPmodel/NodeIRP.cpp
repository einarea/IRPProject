#include "stdafx.h"
#include "NodeIRP.h"


NodeIRP::NodeIRP(int id)
	:
	Node(id)
{
	//
	Quantity.resize(Instance.getNumOfPeriods());
	TimeServed.resize(Instance.getNumOfPeriods());
	Inventory.resize(Instance.getNumOfPeriods());
}


NodeIRP::~NodeIRP()
{
}

NodeIRP::EdgeIRP::EdgeIRP(Node * child, double load, int t, double value = 1)
	:
	Edge(*child, value),
	Load(load),
	Period(t)
{
}
void NodeIRP::addEdge(double load, NodeIRP * child, int period, double value = 1)
{
	Node * nodePtr = child;
	EdgeIRP * edge = new EdgeIRP(nodePtr, load, period, value);
	Edge * edgePtr = edge;
	Node::addEdge(edgePtr);
}

NodeIRP::EdgeIRP * NodeIRP::getEdge(int period)
{
	Edge * edge = Node::getEdge();
	EdgeIRP * derivedPtr = static_cast <EdgeIRP *> (edge);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}

vector<NodeIRP::EdgeIRP*> NodeIRP::getEdges()
{
	vector<Edge*> edges = Node::getEdges();
	vector<EdgeIRP*> edgesIRP;
	edgesIRP.resize(edges.size);
	for (int i = 0; i < edges.size(); i++) {
		EdgeIRP * derivedPtr = static_cast <EdgeIRP *> (edges[i]);
		edgesIRP[i] = derivedPtr;
	}
	return edgesIRP;
}

bool NodeIRP::isDelivery()
{
	return Instance.map.isDelivery(id);
}



NodeIRP * NodeIRP::EdgeIRP::getEndNode()
{
	Node * node = Node::Edge::getEndNode();
	NodeIRP * derivedPtr = static_cast <NodeIRP *> (node);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}
