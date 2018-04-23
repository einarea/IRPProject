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
private:
	int nPeriods;

	//utility functions
	string getNextToken(string & str, string & delimiter) const;
protected:
	vector <NodeInstance *> Nodes; //Vector of all customers 

public:

	NodeInstanceDB(string fileName);
	~NodeInstanceDB();

	bool inArcSet(int, int);
	bool inExtensiveArcSet(int, int);
	bool inSimultaneousArcSet(int, int);
	bool isColocated(int i, int j);
	int getTransCost(const NodeInstance &node1, const NodeInstance &node2);
	int getDistance(const NodeInstance & node1, const NodeInstance &node2);
	int getX(const NodeInstance & node);
	int getY(const NodeInstance & node);
	int getNumNodes();

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
	vector  <int> DeliveryNodes;
	vector <int> PickupNodes;
	vector  <int> Nodes;
	vector  <int> AllNodes;
	vector  <int> Depot;

};