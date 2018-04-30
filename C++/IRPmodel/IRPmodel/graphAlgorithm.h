#pragma once
#include "NodeStrong.h"
#include <stack>
#include "IRP.h"
#include "NodeIRP.h"
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"

class graphAlgorithm
{
public:
	graphAlgorithm();
	static void sepByStrongComp(vector<Node*> &graph, vector<vector<Node*>> &result);
	~graphAlgorithm();
	static void graphAlgorithm::printGraph(vector<NodeIRP *>& graph, string filename, int weight=0);
	static void getRoutes(vector<Node*> &graph, vector<vector<Node*>> &routes);
	void printToLatex(string * Columns, string * Rows, double ** data, string filename);
	static int getColor(double value);
	static double getSimiliarity(vector <NodeIRP*> & graph1, vector <NodeIRP*> & graph2);
	void depthFirst(Node::Edge * edge, int &total, int & equal, vector <Node*> & graph1, vector <Node*> & graph2);

private:
	static void strongConnect(NodeStrong&, int&, stack <NodeStrong*> &, vector<vector<Node*>> &result);
	
};

