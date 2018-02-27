#include "stdafx.h"
#include "NodeStrong.h"



NodeStrong::NodeStrong(int id, vector<Edge*> edges)
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



double NodeStrong::Edge::getValue()
{
	return Value;
}

