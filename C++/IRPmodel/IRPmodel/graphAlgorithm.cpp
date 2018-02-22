#include "stdafx.h"
#include "graphAlgorithm.h"
#include <algorithm>



graphAlgorithm::graphAlgorithm()
{
}

void graphAlgorithm::printGraph(vector<Node>& graph, IRP &instance) {
	
	FILE *gnuplotPipe = _popen("C:\\Octave\\3.2.4_gcc-4.4.0\\bin\\gnuplot", "w");
	Gnuplot gp(gnuplotPipe);

	vector < pair<int, int>> custPoints;
	vector < pair<int, int>> coPoints;
	vector<boost::tuple<int, int, int, int> > arcs;

	int x1;
	int y1;
	int	x2;
	int y2;

	int xLoc;
	int yLoc;

	for (Node node : graph) {

		xLoc = instance.getMap()->getX(node.getId());
		yLoc = instance.getMap()->getY(node.getId());

		custPoints.push_back(make_pair(
			xLoc,
			yLoc));
	}
	for (Node node : graph) {
		x1 = instance.getMap()->getX(node.getId());
		y1 = instance.getMap()->getY(node.getId());
		for (Node::Edge &edge : *node.getEdges()) {
			if (edge.getValue() != -1) {
				x2 = instance.getMap()->getX(edge.getEndNode()->getId());
				y2 = instance.getMap()->getY(edge.getEndNode()->getId());

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

	gp << "set xrange [-110:110]\nset yrange [-110:110]\n";
	gp << "unset tics\n";
	gp << "set size ratio -1\n";
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	
	gp << "plot '-' with vectors notitle lt rgb '#2980b9' filled ,\
		 '-' with points lt rgb '#e74c3c' pointtype 7 pointsize 2 notitle,\
		'-' with points lt rgb '#e67e22' pointtype 7 pointsize 2 notitle \n";	//gp << "plot '-' with lines title 'cubic', '-' with points lt rgb 'violet'   pointtype 7 pointsize 2 title 'circle'\n";
	gp.send1d(arcs);
	gp.send1d(custPoints);
	gp.send1d(custPoints);


}

//Tarjans strongly connected components algorithm, sets value of edges not in strong components to -1.
void graphAlgorithm::sepByStrongComp(vector<Node>& graph, vector<vector<Node>> & result)
{
	
	int index = 0;

	stack <Node *> S;
	for (Node &node : graph) {
		if (node.getIndex() == -1) {
			strongConnect(node, index, S, result);
		}
	}
}


void graphAlgorithm::strongConnect(Node & node, int &index, stack <Node*> &S, vector<vector<Node>> & result)
{
	Node *endNode;
	node.setIndex(index);
	node.setLowLink(index);
	index += 1;
	S.push(&node);
	node.setOnStack(true);
	
	for (Node::Edge &edge : (*node.getEdges())) {
		endNode = edge.getEndNode();
		if (endNode->getIndex() == -1) {
			strongConnect(*endNode, index, S, result);
			node.setLowLink(min(node.getLowLink(), endNode->getLowLink()));

			if (!endNode->isOnStack()) { //End node is finished and part of strong component. No edge between strong components
				
				edge.setValue(-1);
			}
		}
		else if (endNode->isOnStack())
		{
			node.setLowLink(min(node.getLowLink(), endNode->getIndex()));
		}

		else {//Cross edge in tree. Set edge value to -1
			edge.setValue(-1);
		}
	}


	if (node.getLowLink() == node.getIndex()) {
	vector <Node> StrongComponent;
	Node * NodePtr;
		do{
			//Add strong component to result
			NodePtr = S.top();
			StrongComponent.push_back(*NodePtr);
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


