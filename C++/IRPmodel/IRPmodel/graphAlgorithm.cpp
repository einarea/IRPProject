#include "stdafx.h"
#include "graphAlgorithm.h"
#include <algorithm>



graphAlgorithm::graphAlgorithm()
{
}

//Tarjans strongly connected components algorithm
void graphAlgorithm::sepByStrongComp(vector<Node>& graph, vector<vector<Node>> & result)
{
	
	int index = 0;
	stack <Node> S;
	for (Node &node : graph) {
		if (node.getIndex() == -1) {
			strongConnect(node, index, S, result);
		}
	}
}

void graphAlgorithm::strongConnect(Node & node, int &index, stack <Node> &S, vector<vector<Node>> & result)
{
	Node *endNode;
	node.setIndex(index);
	node.setLowLink(index);
	index += 1;
	S.push(node);
	node.setOnStack(true);

	for (Node::Edge &edge : (*node.getEdges())) {
		endNode = edge.getEndNode();
		if (endNode->getIndex() == -1)
			strongConnect(*endNode, index, S, result);

		else if (endNode->isOnStack())
		{
			node.setLowLink(min(node.getLowLink(), endNode->getIndex()));
		}
	}

	if (node.getLowLink() == node.getIndex()) {
		do{
			//Add strong component to result
			(*endNode) = S.top();
			result[node.getIndex()].push_back(*endNode);
			S.pop();
			printf("%d	", (*endNode).getId());
		} while ((*endNode) != node);
		printf("\n");
		
	}

}



graphAlgorithm::~graphAlgorithm()
{
}


