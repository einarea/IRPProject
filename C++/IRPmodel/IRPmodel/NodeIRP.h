#pragma once
#include "IRP.h"
#include "Node.h"

class NodeIRP :
	public Node
{
public:

	class EdgeIRP : public Edge {
	public:
		EdgeIRP(Node *child, double load, int t, double value);
		NodeIRP * getEndNode();
		double Load;
		int Period;
	};
	static IRP& Instance;
	NodeIRP(int id);
	~NodeIRP();
	//Override
	void addEdge(double load, NodeIRP * child, int period, double value);
	EdgeIRP * getEdge(int period);
	vector<double> Quantity;
	vector<double> Inventory;
	vector<double> TimeServed;
	vector <EdgeIRP*> getEdges();

	bool isDelivery();

};

