#include "stdafx.h"
#include "graphAlgorithm.h"
#include <algorithm>



graphAlgorithm::graphAlgorithm()
{
}




/*void graphAlgorithm::printGraph(vector<Node*>& graph, IRP &instance, string filename, int weight) {


	FILE *gnuplotPipe = _popen("C:\\Octave\\3.2.4_gcc-4.4.0\\bin\\gnuplot", "w");
	Gnuplot gp(gnuplotPipe);

	vector < pair<int, int>> nodePoints;
	vector < pair<int, int>> Depot;
	vector<vector<boost::tuple<int, int, int, int> >> arcs(9);

	int x1;
	int y1;
	int	x2;
	int y2;

	int xLoc;
	int yLoc;

	int pickup = 0;
	int nextPickup = 0;
	for (Node *node : graph) {

		xLoc = instance.getMap()->getX(node->getId());
		yLoc = instance.getMap()->getY(node->getId());

		if (xLoc == 0 && yLoc == 0) {
			Depot.push_back(make_pair(xLoc, yLoc));
		}

		else {
			nodePoints.push_back(make_pair(
				xLoc,
				yLoc));
			nodePoints.push_back(make_pair(
				xLoc + 4,
				yLoc));
		}
	}
	for (Node *node : graph) {
		
		x1 = instance.getMap()->getX(node->getId());
		y1 = instance.getMap()->getY(node->getId());

		if (!instance.getMap()->isDelivery(node->getId()))
			//Move outgoing arc from pickup
		{
			x1 += 4;
		}

		for (Node::Edge* edge : node->getEdges()) {
			if (edge->getValue() != -1) {
				x2 = instance.getMap()->getX(edge->getEndNode()->getId());
				y2 = instance.getMap()->getY(edge->getEndNode()->getId());

				if (!instance.getMap()->isDelivery(edge->getEndNode()->getId()))
					//Move outgoing arc from pickup
					{
						x2 += 4;
					}
				int id;
				IRP::NodeIRP::EdgeIRP * edgePtr;
				switch (weight) {
				case 0:
					id = getColor(1);
				case ModelParameters::LOAD:
					edgePtr = static_cast<IRP::NodeIRP::EdgeIRP*> (edge);
					id = getColor((edgePtr->LoadDel + edgePtr->LoadPick) / instance.getCapacity());
					break;

				case ModelParameters::X:
					id = id = getColor(edge->getValue());
					break;
				}
				arcs[id].push_back(boost::make_tuple(
						x1,
						y1,
						x2 - x1,
						y2 - y1
						));
				}
				
			}
		} //end node
	
	string color[] = {"#DDEEF9",  "#AAD5F0","#8DC6EB", "#72B8E7", "#5FADE2","#3498db", "#2384C5", "#1C689B", "#124567" };
	string file = "set term pngcairo size 700, 700\nset output '" + filename + ".png'\n";
	gp << file;
	gp << "set xrange [-55:55]\nset yrange [-55:55]\n";
	gp << "unset tics\n";
	gp << "set size ratio -1\n";
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.

	if (!nodePoints.empty()) {
		if (!Depot.empty() && !arcs.empty()) {

			string str = "plot";
			for (int i = 0; i < arcs.size(); i++)
				if (!arcs[i].empty())
					str = str + "'-' with vectors notitle lw 2 lt rgb '" + color[i] + "' filled, ";

			if (!nodePoints.empty())
				str = str + "'-' with points lt rgb '#e74c3c' pointtype 7 pointsize 2.5 notitle, ";

			str = str + "'-' with points lt rgb '#16a085' pointtype 7 pointsize 3.3 notitle \n";

			gp << str;

			for (int i = 0; i < arcs.size(); i++) {
				if (!arcs[i].empty())
					gp.send1d(arcs[i]);

			}
			if (!nodePoints.empty())
				gp.send1d(nodePoints);
			gp.send1d(Depot);
		}
		else if (!nodePoints.empty()) {
			string str = "plot";
			for (int i = 0; i < arcs.size(); i++)
				if (!arcs[i].empty())
					str = str + "'-' with vectors notitle lw 2 lt rgb '" + color[i] + "' filled, ";

			if (!nodePoints.empty())
				str = str + "'-' with points lt rgb '#16a085' pointtype 7 pointsize 3.3 notitle \n";

			gp << str;

			for (int i = 0; i < arcs.size(); i++) {
				if (!arcs[i].empty())
					gp.send1d(arcs[i]);

			}
			if (!nodePoints.empty())
				gp.send1d(nodePoints);
		}
		else {
			string str = "plot";
			for (int i = 0; i < arcs.size(); i++)
				if (!arcs[i].empty())
					str = str + "'-' with vectors notitle lw 2 lt rgb '" + color[i] + "' filled, ";

			str = str + "'-' with points lt rgb '#e74c3c' pointtype 7 pointsize 2.5 notitle, \
			'-' with points lt rgb '#16a085' pointtype 7 pointsize 3.3 notitle \n";

			gp << str;

			for (int i = 0; i < arcs.size(); i++) {
				if (!arcs[i].empty())
					gp.send1d(arcs[i]);

			}

			gp.send1d(nodePoints);
			gp.send1d(Depot);
		}
	}
	
}*/

void graphAlgorithm::getRoutes(vector<Node*>& graph,  vector<vector<Node*>>& routes)
{
	// Do a depth-first-search to identify routes.
	IRP::NodeIRP *depot = IRP::NodeIRP::getNode(graph[0]);
	vector <IRP::NodeIRP::EdgeIRP*> edges = depot->getEdges();
	//edges->erase(edges->begin());

	vector<Node*> route;
	for (Node::Edge* edge : edges) {
		Node * start = new Node(0);
		Node * v = edge->getEndNode();
		start->addEdge(edge);

		route.push_back(start);
		while (v->getId() != 0) {
			Node * u = v;
			route.push_back(u);
			Node * temp = v->getEdge()->getEndNode();
			v = temp;
		}

		routes.push_back(route);
		route.clear();
	}
}

void graphAlgorithm::printToLatex(string * Columns, string * Rows, double ** data, string filename)
{
	
}

int graphAlgorithm::getColor(double value)
{

	int i = floor(value * 8.9);
	if (i < 0) i = 0;
	return i;
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
	
	for (NodeStrong::Edge *edge : (node.getEdges())) {
		endNode = NodeStrong::getStrongNode(edge->getEndNode());
		if (endNode->getIndex() == -1) {
			strongConnect(*endNode, index, S, result);
			node.setLowLink(min(node.getLowLink(), endNode->getLowLink()));

			if (!endNode->isOnStack()) { //End node is finished and part of strong component. No edge between strong components
				
				edge->setValue(-1);
			}
		}
		else if (endNode->isOnStack())
		
			node.setLowLink(min(node.getLowLink(), endNode->getIndex()));
		/*{int ind = endNode->getIndex();
			//Identified loop, potential subtour
			int low = node.getLowLink();
			int i;
			NodeStrong * ns;
			Node * u;
			Node * v = new Node(*endNode);
			for (i = S.size() - 1; i >= 0; i--) {
				if (node.getId() == S[i]->getId())
					break;
			}

			vector<Node*> Loop;
			do {
				ns = S[i];
				u = new Node(*ns);
				Node::Edge *edge = u->getEdge(*v);
				u->removeEdges();
				u->addEdge(edge);
				Loop.push_back(u);
				v = u;
				i--;
			} while (ns->getIndex() != ind);

			if (Loop.size() <= 5) {
				result.push_back(Loop);
			}
		}*/

		else {//Cross edge in tree. Set edge value to -1
			edge->setValue(-1);
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


