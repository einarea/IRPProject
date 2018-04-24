#ifndef NODEIRP_H
#define NODEIRP_H

#include "Node.h"
#include "NodeInstance.h"


//Class that represents a node in a time period
class NodeIRP:
	public Node
{

	friend class NodeIRPHolder;

public:
	class EdgeIRP : public Edge {

	//Node IRP Holder has access to private members of class NodeIRP


	public:
		EdgeIRP(Node *child, double loadDel, double loadPick, double value);
		NodeIRP * getEndNode();
		double LoadDel;
		double LoadPick;
	};

	NodeIRP();
	bool inArcSet(NodeIRP *);
	bool inArcSet(NodeInstance *);
	NodeIRP(NodeInstance& data);
	~NodeIRP();
	bool isDelivery();
	//Override
	void addEdge(double loadDel, double loadPick, NodeIRP * child, double value);
	EdgeIRP * getEdge();
	static NodeIRP * getNode(Node *);
	double Quantity;
	double Inventory;
	double TimeServed;
	vector <EdgeIRP*> getEdges();
	double getOutflow();
	double getPosX();
	double getPosY();
	double getHoldCost();
	NodeInstance& getData() const;
private:
	bool DELIVERY;
	NodeInstance &nodeData;
};

#endif