#include "stdafx.h"
#include "NodeIRP.h"




bool NodeIRP::inArcSet(NodeIRP * n)
{
	return nodeData.inArcSet(&n->nodeData);
}

bool NodeIRP::inArcSet(NodeInstance * n)
{
	return nodeData.inArcSet(n);
}

NodeIRP::NodeIRP(NodeInstance& data)
	:
	nodeData(data),
	Node(nodeData.getId())
{
	//
	Quantity = -1;
	TimeServed = -1;
	Inventory = -1;
}


NodeIRP::~NodeIRP()
{
}

bool NodeIRP::isDelivery()
{
	return DELIVERY;
}

NodeIRP::EdgeIRP::EdgeIRP(Node * child, double loadDel, double loadPick, double value = 1)
	:
	Edge(*child, value),
	LoadDel(loadDel),
	LoadPick(loadPick)
{
}
void NodeIRP::addEdge(double loadDel, double loadPick, NodeIRP * child, double value = 1)
{
	Node * nodePtr = child;
	EdgeIRP * edge = new EdgeIRP(nodePtr, loadDel, loadPick, value);
	Edge * edgePtr = edge;
	Node::addEdge(edgePtr);
}

NodeIRP::EdgeIRP * NodeIRP::getEdge()
{
	vector<EdgeIRP *> edge = getEdges();
	EdgeIRP * derivedPtr = edge[0];
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}

NodeIRP * NodeIRP::getNode(Node * n)
{
	NodeIRP * derivedPtr = static_cast <NodeIRP *> (n);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}

vector<NodeIRP::EdgeIRP*> NodeIRP::getEdges()
{
	vector<Edge*> edges = Node::getEdges();
	vector<EdgeIRP*> edgesIRP;
	for (int i = 0; i < edges.size(); i++) {
		EdgeIRP * derivedPtr = static_cast <EdgeIRP *> (edges[i]);
		edgesIRP.push_back(derivedPtr);
	}
	return edgesIRP;
}

double NodeIRP::getOutflow()
{
	double outflow = 0;
	vector<EdgeIRP*> edges = getEdges();
	for (EdgeIRP * edge : edges) {
		outflow += edge->getValue();
	}
	return outflow;
}

double NodeIRP::getPosX()
{
	return nodeData.PosX;
}

double NodeIRP::getPosY()
{
	return nodeData.PosY;
}

double NodeIRP::getHoldCost()
{
	return nodeData.HoldingCost;
}

NodeInstance & NodeIRP::getData() const
{
	return nodeData;
}


NodeIRP * NodeIRP::EdgeIRP::getEndNode()
{
	Node * node = Node::Edge::getEndNode();
	NodeIRP * derivedPtr = static_cast <NodeIRP *> (node);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}