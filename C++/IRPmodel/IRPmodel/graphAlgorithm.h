#pragma once
#include "Node.h"
#include <stack>

class graphAlgorithm
{
public:
	graphAlgorithm();
	static void sepByStrongComp(vector<Node> &graph, vector<vector<Node>> &result);
	~graphAlgorithm();

private:
	static void strongConnect(Node&, int&, stack <Node*> &, vector<vector<Node>> &result);
	
};

