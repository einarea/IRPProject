#include "stdafx.h"
#include "graphAlgorithm.h"
#include <algorithm>



graphAlgorithm::graphAlgorithm()
{
}

void graphAlgorithm::printGraph(vector<Node*>& graph, IRP &instance, string filename) {

	static int i = 0;
	
	FILE *gnuplotPipe = _popen("C:\\Octave\\3.2.4_gcc-4.4.0\\bin\\gnuplot", "w");
	Gnuplot gp(gnuplotPipe);

	vector < pair<int, int>> custPoints;
	vector < pair<int, int>> coPoints;
	vector < pair<int, int>> Depot;
	vector<boost::tuple<int, int, int, int> > arcs;

	int x1;
	int y1;
	int	x2;
	int y2;

	int xLoc;
	int yLoc;

	for (Node *node : graph) {

		xLoc = instance.getMap()->getX(node->getId());
		yLoc = instance.getMap()->getY(node->getId());

		if (xLoc == 0 && yLoc == 0) {
			Depot.push_back(make_pair(xLoc, yLoc));
		}

		else {
		custPoints.push_back(make_pair(
			xLoc,
			yLoc));
		}
	}
	for (Node *node : graph) {
		x1 = instance.getMap()->getX(node->getId());
		y1 = instance.getMap()->getY(node->getId());
		for (Node::Edge &edgePtr : *node->getEdges()) {
			NodeStrong::EdgeStrong * edge = NodeStrong::EdgeStrong::getStrongEdge(&edgePtr);
			if (edge->getValue() != -1) {
				x2 = instance.getMap()->getX(edge->getEndNode()->getId());
				y2 = instance.getMap()->getY(edge->getEndNode()->getId());

				if (x1 == x2 && y1 == y2) { //Co-located node
					coPoints.push_back(make_pair(x1, y1));
					}
				else{
					arcs.push_back(boost::make_tuple(
							x1,
							y1,
							x2 - x1,
							y2 - y1
							));
					}
			}
		}
	}

	string file = "set term pngcairo size 700, 700\nset output '"+filename+ to_string(i) + ".png'\n";
	gp << file;
	gp << "set xrange [-110:110]\nset yrange [-110:110]\n";
	gp << "unset tics\n";
	gp << "set size ratio -1\n";
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	
	if (!Depot.empty()) {
		gp << "plot '-' with vectors notitle lw 2 lt rgb '#2980b9' filled ,\
		 '-' with points lt rgb '#e74c3c' pointtype 7 pointsize 2.5 notitle,\
		'-' with points lt rgb '#e67e22' pointtype 7 pointsize 2.5 notitle,\
		'-' with points lt rgb '#16a085' pointtype 7 pointsize 3.3 notitle \n";	
		gp.send1d(arcs);
		gp.send1d(custPoints);
		gp.send1d(custPoints);
		gp.send1d(Depot);
	}
	else {
		gp << "plot '-' with vectors notitle lw 2 lt rgb '#2980b9' filled ,\
		 '-' with points lt rgb '#e74c3c' pointtype 7 pointsize 2.5 notitle,\
		'-' with points lt rgb '#e67e22' pointtype 7 pointsize 2.5 notitle\n";
		gp.send1d(arcs);
		gp.send1d(custPoints);
		gp.send1d(custPoints);
	}
	
	i++;

}

void graphAlgorithm::getRoutes(vector<Node*>& graph, vector<vector<Node*>>& routes)
{
	// Do a depth-first-search to identify routes.
	Node *depot = graph[0];
	vector <Node::Edge> *edges = depot->getEdges();


	//Initialize edges  from depot

	vector<Node *> route;
	for (Node::Edge edge : *edges) {
		Node * u = new Node(0);
		route.push_back(u);
		Node * v = edge.getEndNode();
		u->addEdge(*v);
		route.push_back(v);
		while (v->getId() != 0) {
			Node * newNode = new Node(*v);
			route.push_back(newNode);
			Node * w = v->getEdge(0)->getEndNode();
			v	=  w;
		}

		routes.push_back(route);
		route.clear();
	}
}

//Tarjans strongly connected components algorithm, sets value of edges not in strong components to -1.
void graphAlgorithm::sepByStrongComp(vector<Node*>& graph, vector<vector<Node*>> & result)
{	
	//Initialize graph
	for (Node *node : graph) {
		NodeStrong * nodeStr = NodeStrong::getStrongNode(node);
		nodeStr->setIndex(-1);
	}

	int index = 0;

	stack <NodeStrong *> S;
	for (Node *node : graph) {
		NodeStrong *nodeStr = NodeStrong::getStrongNode(node);
		if (nodeStr->getIndex() == -1) {
			strongConnect(*nodeStr, index, S, result);
		}
	}
}


void graphAlgorithm::strongConnect(NodeStrong & node, int &index, stack <NodeStrong*> &S, vector<vector<Node*>> & result)
{
	NodeStrong *endNode;
	node.setIndex(index);
	node.setLowLink(index);
	index += 1;
	S.push(&node);
	node.setOnStack(true);
	
	for (NodeStrong::Edge &edge : (*node.getEdges())) {
		endNode = NodeStrong::getStrongNode(edge.getEndNode());
		if (endNode->getIndex() == -1) {
			strongConnect(*endNode, index, S, result);
			node.setLowLink(min(node.getLowLink(), endNode->getLowLink()));

			if (!endNode->isOnStack()) { //End node is finished and part of strong component. No edge between strong components
				NodeStrong::EdgeStrong * edgePtr = NodeStrong::EdgeStrong::getStrongEdge(&edge);
				edgePtr->setValue(-1);
			}
		}
		else if (endNode->isOnStack())
		{
			node.setLowLink(min(node.getLowLink(), endNode->getIndex()));
		}

		else {//Cross edge in tree. Set edge value to -1
			NodeStrong::EdgeStrong * edgePtr = NodeStrong::EdgeStrong::getStrongEdge(&edge);
			edgePtr->setValue(-1);
		}
	}


	if (node.getLowLink() == node.getIndex()) {
	vector <Node*> StrongComponent;
	NodeStrong * NodePtr;
		do{
			//Add strong component to result
			NodePtr = S.top();
			StrongComponent.push_back(NodePtr);
			S.pop();
			NodePtr->setOnStack(false);
			int a = 1;
			//printf("%d	", NodePtr->getId());
		} while ((*NodePtr) != node);
		//printf("\n");
		result.push_back(StrongComponent);
		//printf("\n");
		
	}

}



graphAlgorithm::~graphAlgorithm()
{
}


