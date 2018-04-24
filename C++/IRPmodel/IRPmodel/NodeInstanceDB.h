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


	//utility functions
	string getNextToken(string & str, string & delimiter) const;

	//Database vector
	vector <NodeInstance * > NodeData;

public:
	int Capacity;

	NodeInstanceDB(string fileName);
	NodeInstanceDB(int nCustomers, int nPeriods);
	~NodeInstanceDB();

	static NodeInstanceDB * createInstance(int nCustomers, int nPeriods, int version);
	static string getFilename(int nCustomers, int nPeriods, int version);

	bool inArcSet(NodeInstance * node1, NodeInstance * node2);
	NodeInstance * getDepot();
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

	vector  <NodeInstance *> AllNodes;
	vector <NodeInstance *> DeliveryNodes;
	vector <NodeInstance *> PickupNodes;
	vector <NodeInstance *> Nodes;
	vector  <NodeInstance *> Depot;

};