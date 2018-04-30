#include "stdafx.h"
#include "NodeIRP.h"

//Copy constructor
NodeIRP::NodeIRP(const NodeIRP & cpNode)
	:
	Node(cpNode),
	NodeData(cpNode.NodeData)
{
	Quantity = cpNode.Quantity;
	TimeServed = cpNode.TimeServed;
	Inventory = cpNode.Inventory;
}



NodeIRP * NodeIRP::getNextNode()
{
	return getEdge()->getEndNode();
}

NodeIRP & NodeIRP::operator=(const NodeIRP & cpNode)
{
	//Clear edges
	for (auto edge : getEdges())
		delete edge;

	return *new NodeIRP(cpNode);
}

bool NodeIRP::inArcSet(const NodeIRP * n) const
{
	return NodeData.inArcSet(&n->NodeData);
}

bool NodeIRP::inArcSet(const NodeInstance * n) const
{
	return NodeData.inArcSet(n);
}

NodeIRP::NodeIRP(const NodeInstance& data)
	:
	NodeData(data),
	Node(data.getId())
{
	//
	Quantity = -1;
	TimeServed = -1;
	Inventory = -1;
}


NodeIRP::~NodeIRP()
{
	//Calls parent which delete edges
}

bool NodeIRP::isDelivery()
{
	return NodeData.isDelivery();
}

NodeIRP::EdgeIRP::EdgeIRP(Node * child, double loadDel, double loadPick, double value = 1)
	:
	Edge(*child, value),
	LoadDel(loadDel),
	LoadPick(loadPick)
{
}
NodeIRP::EdgeIRP::~EdgeIRP()
{
}
void NodeIRP::addEdge(double loadDel, double loadPick, NodeIRP * child, double value = 1)
{
	Node * nodePtr = child;
	EdgeIRP * edge = new EdgeIRP(nodePtr, loadDel, loadPick, value);
	Edge * edgePtr = edge;
	Node::addEdge(edgePtr);
}

void NodeIRP::copyEdge(NodeIRP::EdgeIRP * edge, NodeIRP * nextNode)
{
	addEdge(edge->LoadDel, edge->LoadPick, nextNode, edge->getValue());
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

int NodeIRP::getPosX()
{
	return NodeData.PosX;
}

int NodeIRP::getPosY()
{
	return NodeData.PosY;
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

double NodeIRP::getPosX() const
{
	return NodeData.PosX;
}

double NodeIRP::getPosY() const
{
	return NodeData.PosY;
}

double NodeIRP::getHoldCost()
{
	return NodeData.HoldingCost;
}

const NodeInstance & NodeIRP::getData() const
{
	return NodeData;
}



NodeIRP * NodeIRP::EdgeIRP::getEndNode()
{
	Node * node = Node::Edge::getEndNode();
	NodeIRP * derivedPtr = static_cast <NodeIRP *> (node);
	if (derivedPtr != 0) {
		return derivedPtr;
	}
}


