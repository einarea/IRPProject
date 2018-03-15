#pragma once
#include "NodeIRP.h"
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
	static void graphAlgorithm::printGraph(vector<Node *>& graph, IRP &, string filename, int weight=0);
	static void getRoutes(vector<Node*> &graph, vector<vector<Node*>> &routes);
	void printToLatex(string * Columns, string * Rows, double ** data, string filename);
	static int getColor(double value);

private:
	static void strongConnect(NodeStrong&, int&, vector <NodeStrong*> &, vector<vector<Node*>> &result, vector<vector<Node *>> &Loop);
	
};

