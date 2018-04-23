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

	NodeIRP(int id, NodeInstance& data);
	~NodeIRP();
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
private:
	NodeInstance &nodeData;
};