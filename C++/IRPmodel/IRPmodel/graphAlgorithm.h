#pragma once
#include "Node.h"
#include <stack>
#include "IRP.h"
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"

class graphAlgorithm
{
public:
	graphAlgorithm();
	static void sepByStrongComp(vector<Node> &graph, vector<vector<Node>> &result);
	~graphAlgorithm();
	static void graphAlgorithm::printGraph(vector<Node>& graph, IRP &);

private:
	static void strongConnect(Node&, int&, stack <Node*> &, vector<vector<Node>> &result);
	
};

