#include "stdafx.h"
#include "Edge.h"

Edge::Edge(Node & endNode, double value)
:
Value(value),
endNode(endNode)
{
}


Edge::~Edge()
{
}
