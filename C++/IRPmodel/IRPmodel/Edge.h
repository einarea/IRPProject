#pragma once
#include "stdafx.h"
#include "Node.h"

class Edge
{
public:
	Edge(Node & , double);
	~Edge();
private:
	double Value;
	Node & endNode;
};



