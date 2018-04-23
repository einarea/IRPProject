#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include "NodeInstanceDB.h"
using namespace std;





int NodeInstanceDB::getDistance(const NodeInstance& node1, const NodeInstance& node2)
{
	int distance;

	int x1 = node1.getXpos();
	int y1 = node1.getYpos();
	int x2 = node2.getXpos();
	int y2 = node2.getYpos();

	distance = (int)floor(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
	
	return distance;
}

int NodeInstanceDB::getTransCost(const NodeInstance &node1, const NodeInstance &node2)
{
	int distance = getDistance(node1, node2);
	return distance * ModelParameters::TRANSCOST_MULTIPLIER + ModelParameters::SERVICECOST_MULTIPLIER;
}


int NodeInstanceDB::getX(const NodeInstance& node)
{
	return node.getXpos();
}

int NodeInstanceDB::getY(const NodeInstance& node)
{
	return node.getYpos();
}

//returns the number of pickup and delivery nodes
int NodeInstanceDB::getNumNodes()
{
	//Do not count depot
	return Nodes.size() - 1;
}

int NodeInstanceDB::getnPeriods() const
{
	return nPeriods;
}

void NodeInstanceDB::writeInstanceToFile(ofstream &instanceFile, string Filename)
{
	instanceFile.open(Filename);
	instanceFile << "nPeriods = " << getnPeriods() << "\n";
	instanceFile << "HD\tLL\tUL\t";

	for (int t = 1; t <= getnPeriods(); t++) {
		instanceFile << "Q" << t << "\t";
	}

	

	instanceFile << "I01\tX\tY\n";

	for (auto n : Nodes) {
		if (n->getId() != 0) {
			NodeInstance * node = getNode(n->getId());

			instanceFile << node->HoldingCost << "\t"
				<< "\t" << node->LowerLimit << "\t"
				<< "\t" << node->UpperLimit << "\t";

			for (int t = 1; t <= getnPeriods(); t++) {
				instanceFile << node->Demand[t];
			}
			
			instanceFile << node->InitInventory << "\t";
			instanceFile << node->PosX << "\t" << node->PosY << "\n";
		}
	}

	instanceFile << "end-of-file";
	instanceFile.close();
}


string NodeInstanceDB::getNextToken(string &str, string& delimiter) const
{
	size_t pos = 0;
	pos = str.find(delimiter);
	string token = str.substr(0, pos);
	str.erase(0, pos + delimiter.length());
	return token;
}

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
	Nodes.push_back(new NodeInstance(0, 0, 0));
	//check file for errors
	getline(nodeRecords, line);

	int nodeID = 1;
	int HoldingCost;
	vector<int> Demand;
	int InitInventory;
	int LowerLimit;
	int UpperLimit;


	while (!getline(nodeRecords, line).eof()) {

		size_t pos = 0;
		string token;
		delimiter = "\t";

		//Inventory parameters
		HoldingCost = stoi(getNextToken(line, delimiter));
		LowerLimit = stoi(getNextToken(line, delimiter));
		UpperLimit = stoi(getNextToken(line, delimiter));

		Demand.resize(nPeriods);

		for (int t = 0; t < nPeriods; t++) {
			Demand[t] = stoi(getNextToken(line, delimiter));
		}

		//Inital inventory
		InitInventory = stoi(getNextToken(line, delimiter));

		//If datafile provide coordinates
		int x = stoi(getNextToken(line, delimiter));
		int y = stoi(getNextToken(line, delimiter));
		NodeInstance * node = new NodeInstance(nodeID, x, y, nPeriods, InitInventory, HoldingCost, Demand);
		Nodes.push_back(node);
		nodeID++;
	
	}
}

NodeInstanceDB::~NodeInstanceDB()
{
}

vector<NodeInstance*>* NodeInstanceDB::getNodes()
{
	return &Nodes;
}

NodeInstance * NodeInstanceDB::getNode(int id)
{
	for (NodeInstance *n: Nodes) {
		if (n->getId()==id)
			return n;
	}
}
