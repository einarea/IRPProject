#pragma once
#include "NodeStrong.h"
#include <stack>
#include "IRP.h"
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"

class graphAlgorithm
{
public:
	graphAlgorithm();
	static void sepByStrongComp(vector<Node*> &graph, vector<vector<Node*>> &result);
	~graphAlgorithm();
	static void graphAlgorithm::printGraph(vector<Node*>& graph, IRP &, string filename);
	static void getRoutes(vector<Node*> &graph, vector<vector<Node*>> &routes);

private:
	static void strongConnect(NodeStrong&, int&, stack <NodeStrong*> &, vector<vector<Node*>> &result);
	
};

