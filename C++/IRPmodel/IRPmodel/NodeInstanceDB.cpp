#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include "NodeInstanceDB.h"
using namespace std;





int NodeInstanceDB::getDistance(const NodeInstance& node1, const NodeInstance& node2) const
{
	int distance;

	int x1 = node1.getXpos();
	int y1 = node1.getYpos();
	int x2 = node2.getXpos();
	int y2 = node2.getYpos();

	distance = (int)floor(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
	
	return distance;
}

int NodeInstanceDB::getDistance(int i, int j) const
{
	auto node1 = getNode(i);
	auto node2 = getNode(j);
	int x1 = node1->PosX;
	int y1 = node1->PosY;
	int x2 = node2->PosX;
	int y2 = node2->PosY;

	int distance = (int)floor(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));

	return distance;
}

bool NodeInstanceDB::isColocated(int i, int j) const
{
	return false;
}

int NodeInstanceDB::getTransCost(const NodeInstance &node1, const NodeInstance &node2) const
{
	int distance = getDistance(node1, node2);
	return distance * ModelParameters::TRANSCOST_MULTIPLIER + ModelParameters::SERVICECOST_MULTIPLIER;
}

int NodeInstanceDB::getTravelTime(int i, int j) const
{
	return getDistance(i, j) * ModelParameters::TRAVELTIME_MULTIPLIER + ModelParameters::SERVICETIME;
}

int NodeInstanceDB::getTransCost(int i, int j) const
{
	int distance = getDistance(i, j);
	return distance * ModelParameters::TRANSCOST_MULTIPLIER + ModelParameters::SERVICECOST_MULTIPLIER;
}

int NodeInstanceDB::getDemand(const NodeInstance & node1, int period) const
{
	return node1.Demand[period];
}


int NodeInstanceDB::getX(const NodeInstance& node) const
{
	return node.getXpos();
}

int NodeInstanceDB::getY(const NodeInstance& node) const
{
	return node.getYpos();
}

//returns the number of pickup and delivery nodes
int NodeInstanceDB::getNumNodes() const
{
	//Do not count depot
	return Nodes.size() - 1;
}

bool NodeInstanceDB::isDelivery(int id) const
{
	const NodeInstance * node = getNode(id);
	if (node->isDelivery())
		return true;
	else
		return false;
}

int NodeInstanceDB::getnPeriods() const
{
	return nPeriods;
}

void NodeInstanceDB::writeInstanceToFile(ofstream &instanceFile, string Filename)
{
	instanceFile.open(Filename);
	instanceFile << "nPeriods = " << getnPeriods() << "\n";
	instanceFile << "DEL\tHC\tLL\tUL\t";

	for (int t = 1; t <= nPeriods; t++) {
		instanceFile << "Q" << t << "\t";
	}

	

	instanceFile << "I0\tX\tY\n";

	int i; 
	for (auto node : Nodes) {
		if (node->getId() != 0) {
	
			node->Delivery ? i = 1 : i = 0;
			instanceFile << i <<  "\t"
				<< node->HoldingCost << "\t"
				<< node->LowerLimit << "\t"
				<< node->UpperLimit << "\t";

			for (int t = 1; t <= nPeriods; t++) {
				instanceFile << node->Demand[t] <<"\t";
			}
			
			instanceFile << node->InitInventory << "\t";
			instanceFile << node->PosX << "\t" << node->PosY << "\n";
		}
	}

	instanceFile << "end-of-file";
	instanceFile.close();
}

void NodeInstanceDB::initializeSets()
{
	for (auto node : NodeData) {
		AllNodes.push_back(node);
		if (node->getId() != 0) {
			Nodes.push_back(node);
			if (node->isDelivery())
				DeliveryNodes.push_back(node);
			else
				PickupNodes.push_back(node);
		}
	}

	for (int t = 0; t <= getnPeriods(); t++) {
		AllPeriods.push_back(t);
		if (t != 0)
			Periods.push_back(t);
	}

	initializeArcSet();

}


string NodeInstanceDB::getNextToken(string &str, string& delimiter) const
{
	size_t pos = 0;
	pos = str.find(delimiter);
	string token = str.substr(0, pos);
	str.erase(0, pos + delimiter.length());
	return token;
}

void NodeInstanceDB::initializeArcSet()
{
	for (auto nodePick : PickupNodes) {
		for (auto nodeDel : DeliveryNodes)
			if (nodePick->isColocated(nodeDel)) {
				nodePick->ForbiddenNodes.push_back(nodeDel);
				break;
			}
	}

}


//Constructor
NodeInstanceDB::NodeInstanceDB(string fileName)
{
	
	ifstream nodeRecords(fileName, ios::in);

	if (!nodeRecords)
	{
		cerr << "Customer records file could not be opened" << endl;
		exit(1);
	}

	string line;

	//Get number of periods
	getline(nodeRecords, line);
	string delimiter = "=";
	getNextToken(line, delimiter);
	nPeriods = stoi(getNextToken(line, delimiter));


	//Push back depot
	NodeData.push_back(new NodeInstance(0, true, 0, 0, nPeriods, 1));
	//check file for errors
	getline(nodeRecords, line);

	int nodeID = 1;
	int HoldingCost;
	vector<int> Demand;
	int InitInventory;
	int LowerLimit;
	int UpperLimit;
	bool Del;

	while (!getline(nodeRecords, line).eof()) {

		size_t pos = 0;
		string token;
		delimiter = "\t";

		//Inventory parameters
		Del  = stoi(getNextToken(line, delimiter)) == 1 ? true : false;
		HoldingCost = stoi(getNextToken(line, delimiter));
		LowerLimit = stoi(getNextToken(line, delimiter));
		UpperLimit = stoi(getNextToken(line, delimiter));

		Demand.resize(nPeriods + 1);
		//Not defined for period 0
		Demand[0] = -1;
		for (int t = 1; t <= nPeriods; t++) {
			Demand[t] = stoi(getNextToken(line, delimiter));
		}

		//Inital inventory
		InitInventory = stoi(getNextToken(line, delimiter));

		//If datafile provide coordinates
		int x = stoi(getNextToken(line, delimiter));
		int y = stoi(getNextToken(line, delimiter));
		NodeInstance * node = new NodeInstance(nodeID, Del, x, y, nPeriods, InitInventory, HoldingCost, UpperLimit, LowerLimit, Demand);
		NodeData.push_back(node);
		nodeID++;

	
	}


	initializeSets();

	//Initialize capacity
	int TotalDemand = 0;
	for (int t : Periods) {
		for (auto node : NodeData) {
			TotalDemand += node->Demand[t];
		}
	}

	Capacity = 2 * floor(TotalDemand / (ModelParameters::nVehicles*getnPeriods()));

}

//Generate two colocated nodes for nCustomers
NodeInstanceDB::NodeInstanceDB(int nCustomers, int nPer)
	:
	nPeriods(nPer)
{
	NodeData.push_back(new NodeInstance(0, true, 0, 0, nPer, 1));

	for (int i = 1; i <= 2*nCustomers; i = i+2) {
		auto delNode = new NodeInstance(i, true, nPer, i);
		NodeData.push_back(delNode);
		NodeData.push_back(new NodeInstance(i+1, false, delNode->PosX, delNode->PosY, nPer, i+1));
	}

	initializeSets();

	//Initialize capacity
	int TotalDemand = 0;
	for (int t : Periods) {
		for (auto node : NodeData) {
			TotalDemand += node->Demand[t];
		}
	}

	Capacity = 2 * floor(TotalDemand / (ModelParameters::nVehicles*getnPeriods()));
}


NodeInstanceDB::~NodeInstanceDB()
{
}

NodeInstanceDB* NodeInstanceDB::createInstance(int nCustomers, int nPeriods, int version)
{
	//Sleep(1000);
	NodeInstanceDB *db = new NodeInstanceDB(nCustomers, nPeriods);

	ofstream instanceFile;

	string filename = getFilename(nCustomers, nPeriods, version);
	db->writeInstanceToFile(instanceFile, filename);
	return db;

}

NodeInstanceDB * NodeInstanceDB::openInstance(int nCustomers, int nPeriods, int version)
{
	return new NodeInstanceDB(getFilename(nCustomers, nPeriods, version));
}

string NodeInstanceDB::getFilename(int nCustomers, int nPeriods, int version)
{
	string extension(".txt");
	return  "Instances/C" + to_string(nCustomers) + "T" + to_string(nPeriods) + "_v" + to_string(version) + extension;
}

bool NodeInstanceDB::inArcSet(int i, int j) const
{
	if (ModelParameters::Simultaneous)
		return inSimultaneousArcSet(i, j);
	else
		return inExtensiveArcSet(i, j);
}

bool NodeInstanceDB::inArcSet(NodeInstance * node1, NodeInstance * node2) const
{
	if (node1->getId() == node2->getId())
		return false;

	if (node1->hasArc(node2));
		return true;
	
	return false;
}

const NodeInstance * NodeInstanceDB::getDepot() const
{
	for (auto node : AllNodes)
		if (node->getId() == 0)
			return node;

	exit(111);
}


bool NodeInstanceDB::inExtensiveArcSet(int i, int j) const
{
	bool a = (i == j || (i == getNumNodes() + j && j != 0));
	return !a;
}

bool NodeInstanceDB::inSimultaneousArcSet(int i, int j) const
{
	//Check if incident from delivery node and not to co-located pickup node
	if (i == 0)
		if (j <= getNumNodes())
			return true;
		else
			return false;

	if (isDelivery(i) && !isColocated(i, j))
		return false;

	//Check if incident to pickup node and not from co-located delivery node
	if (!isDelivery(j) && !isColocated(i, j) && j != 0)
		return false;

	return inExtensiveArcSet(i, j);
}

vector<NodeInstance*>* NodeInstanceDB::getNodes()
{
	return &NodeData;
}

NodeInstance const * NodeInstanceDB::getNode(int id) const
{
	for (NodeInstance *n: NodeData) {
		if (n->getId()==id)
			return n;
	}
}


int NodeInstanceDB::getX(int id) const
{
	return getNode(id)->PosX;
}

int NodeInstanceDB::getY(int id) const
{
	return getNode(id)->PosY;
}
