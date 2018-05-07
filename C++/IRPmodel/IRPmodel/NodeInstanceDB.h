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
	void initializeArcSet();

public:
	double Capacity;

	NodeInstanceDB(string fileName);
	NodeInstanceDB(int nCustomers, int nPeriods);
	~NodeInstanceDB();

	vector<int> getDifference2(vector<int> set1, vector<NodeInstance*> set2) const;
	vector<NodeInstance*> getDifference(vector<NodeInstance*> set1, vector<NodeInstance*> set2) const;
	static NodeInstanceDB * createInstance(int nCustomers, int nPeriods, int version);
	static NodeInstanceDB * openInstance(int nCustomers, int nPeriods, int version);
	static string getFilename(int nCustomers, int nPeriods, int version);
	NodeInstance * getColocatedNode(const NodeInstance*);

	bool inArcSet(NodeInstance * node1, NodeInstance * node2) const;
	const NodeInstance * getDepot() const;
	bool inArcSet(int id1, int id2) const;
	bool inExtensiveArcSet(int, int) const;
	bool inSimultaneousArcSet(int, int) const;
	bool isColocated(int i, int j) const;
	int getTransCost(const NodeInstance &node1, const NodeInstance &node2) const;
	int getTravelTime(int i, int j) const;
	int getTransCost(int i, int j) const;
	int getDemand(const NodeInstance &node1, int period) const;
	int getDistance(const NodeInstance & node1, const NodeInstance &node2) const;
	int getDistance(int i, int j) const; 
	int getX(const NodeInstance & node) const;
	int getY(const NodeInstance & node) const;
	int getNumNodes() const;
	bool isDelivery(int id) const;

	int getnPeriods() const;
	vector <NodeInstance *> * getNodes();
	NodeInstance const * getNode(int id) const;
	int getX(int) const;
	int getY(int) const;
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