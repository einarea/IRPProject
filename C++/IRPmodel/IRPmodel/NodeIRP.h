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

	//Copy constructor
	NodeIRP(NodeIRP&);
	bool inArcSet(NodeIRP *);
	bool inArcSet(NodeInstance *);
	template<typename T> double getTransportationCost(T * node);
	template<typename T> double getTravelTime(T* node);

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
	NodeInstance &NodeData;
	template<typename T> getDistance(T * node);
};

#endif

template<typename T>
inline double NodeIRP::getTransportationCost(T * node)
{
	return getDistance(node) * ModelParameters::TRANSCOST_MULTIPLIER + ModelParameters::SERVICECOST_MULTIPLIER;
}

template<typename T>
inline double NodeIRP::getTravelTime(T * node)
{
	return getDistance(node) * ModelParameters::TRAVELTIME_MULTIPLIER + ModelParameters::SERVICETIME;
}

template<typename T>
inline NodeIRP::getDistance(T * node)
{
	return (int)floor(sqrt(pow(PosX - node->PosX, 2) + pow(PosY - node->PosY, 2)));
}
