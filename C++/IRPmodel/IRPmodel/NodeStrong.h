#pragma once
#include "stdafx.h"
#include <vector>
#include "Node.h"

using namespace std;
class NodeStrong: public Node
{
public:

	NodeStrong(int id, vector <Edge> edges);
	NodeStrong(int id);
	NodeStrong(Node * node);

	class EdgeStrong : public Edge {
		public:
			EdgeStrong(Node & child, double value);
			double getValue();
			static EdgeStrong * getStrongEdge(Edge *);
			void setValue(double);
		private:
		double Value;

	};

	int getIndex();
	void setIndex(int);
	void setLowLink(int);
	void setOnStack(bool);
	int getLowLink();
	void addEdge(double value, Node & child);
	static NodeStrong * getStrongNode(Node *);
	bool isOnStack();
	~NodeStrong();


private:
	
	int Index = -1;
	int LowLink = -1;
	bool onStack;
	vector <Node> Nodes;
};

