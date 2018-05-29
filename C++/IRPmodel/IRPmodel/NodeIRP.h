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
		~EdgeIRP();
		NodeIRP * getEndNode();
		double LoadDel;
		double LoadPick;
	};

	//Copy constructor
	NodeIRP(const NodeIRP&);
	NodeIRP* getNextNode();
	NodeIRP& operator =(const NodeIRP& cpNode);

	bool inArcSet(const NodeIRP *) const;
	bool inArcSet(const NodeInstance *) const;
	double getTransCost(const NodeIRP * node) const;
	double getTravelTime(const NodeIRP * node) const;
	double getTransCost(const NodeInstance * node) const;
	double getTravelTime(const NodeInstance * node) const;
	bool isColocated(const NodeIRP * node) const;

	NodeIRP(const NodeInstance& data);
	~NodeIRP();
	bool isDelivery();
	//Override
	void addEdge(double loadDel, double loadPick, NodeIRP * child, double value);
	void copyEdge(NodeIRP::EdgeIRP * edge, NodeIRP *);
	EdgeIRP * getEdge();
	static NodeIRP * getNode(Node *);
	double Quantity;
	double Inventory;
	double TimeServed;
	int getPosX();
	int getPosY();
;
	vector <EdgeIRP*> getEdges();
	double getOutflow();
	double getPosX() const;
	double getPosY() const;
	double getHoldCost();
	const NodeInstance& getData() const;


private:
	const NodeInstance &NodeData;
	double getDistance(const NodeIRP * node) const;
	double getDistance(const NodeInstance * node) const;
};


inline double NodeIRP::getTransCost(const NodeIRP * node) const
{
	return floor(getDistance(node) * ModelParameters::TRANSCOST_MULTIPLIER) + ModelParameters::SERVICECOST_MULTIPLIER;
}

inline double NodeIRP::getTravelTime(const NodeIRP * node) const
{
	return floor(getDistance(node) * ModelParameters::TRAVELTIME_MULTIPLIER) + ModelParameters::SERVICETIME;
}

inline double NodeIRP::getDistance(const NodeIRP * node) const
{
	return (double) (sqrt(pow(NodeData.PosX - node->NodeData.PosX, 2) + pow(NodeData.PosY - node->NodeData.PosY, 2)));
}

inline double NodeIRP::getTransCost(const NodeInstance * node) const
{
	return floor(getDistance(node) * ModelParameters::TRANSCOST_MULTIPLIER) + ModelParameters::SERVICECOST_MULTIPLIER;
}

inline double NodeIRP::getTravelTime(const NodeInstance * node) const
{
	return floor(getDistance(node) * ModelParameters::TRAVELTIME_MULTIPLIER) + ModelParameters::SERVICETIME;
}

inline bool NodeIRP::isColocated(const NodeIRP * node) const
{
	return this->getData().isColocated(&node->getData());
}

inline double NodeIRP::getDistance(const NodeInstance * node) const
{
	return (double) (sqrt(pow(NodeData.PosX - node->PosX, 2) + pow(NodeData.PosY - node->PosY, 2)));
}


#endif
