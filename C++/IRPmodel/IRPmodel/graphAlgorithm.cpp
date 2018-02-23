#include "stdafx.h"
#include "graphAlgorithm.h"
#include <algorithm>



graphAlgorithm::graphAlgorithm()
{
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


