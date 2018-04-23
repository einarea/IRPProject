#pragma once

#include "stdafx.h"
#include "ModelBase.h"
#include <vector>
#include "NodeInstance.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace::std;

//Class that stores the instance and define arc set, used to initialize the instance from the file name
class NodeInstanceDB
{
	friend class Solution;
	friend class Route;
	friend class IRP;

private:
	int nPeriods;
	int Capacity;

	//utility functions
	string getNextToken(string & str, string & delimiter) const;
protected:
	vector <NodeInstance *> NodeData; //Vector of all nodes
	vector <NodeInstance *> DeliveryNodes;
	vector <NodeInstance *> PickupNodes;
	vector <NodeInstance *> Nodes;
public:

	NodeInstanceDB(string fileName);
	~NodeInstanceDB();

	bool inArcSet(NodeInstance * node1, NodeInstance * node2);
	bool inArcSet(int id1, int id2);
	bool inExtensiveArcSet(int, int);
	bool inSimultaneousArcSet(int, int);
	bool isColocated(int i, int j);
	int getTransCost(const NodeInstance &node1, const NodeInstance &node2);
	int getTravelTime(int i, int j);
	int getTransCost(int i, int j);
	int getDemand(const NodeInstance &node1, int period);
	int getDistance(const NodeInstance & node1, const NodeInstance &node2);
	int getDistance(int i, int j);
	int getX(const NodeInstance & node);
	int getY(const NodeInstance & node);
	int getNumNodes();
	bool isDelivery(int id);

	int getnPeriods() const;
	vector <NodeInstance *> * getNodes();
	NodeInstance * getNode(int id);
	int getnNodes();
	int getX(int);
	int getY(int);
	void writeInstanceToFile(ofstream &instanceFile, string Filename);
	void initializeSets();


	//sets

	vector  <int> Periods;
	vector  <int> AllPeriods;

	vector  <int> AllNodes;
	vector  <int> Depot;

};