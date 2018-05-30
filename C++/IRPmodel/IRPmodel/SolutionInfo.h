#pragma once

#ifndef SOLUTIONINFO_H
#define SOLUTIONINF_H

#include <vector>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "ModelParameters.h"
#include "RouteAnalyzer.h"

using namespace std;
class SolutionInfo
{

public:
	struct Information {
		Information(string state, double objectiveVal, double time);
		string State;
		double ObjectiveVal;
		double Time;
	};

	class InstanceInfo {
	public:

		double bestObjective = -1;
		int nRoutePool = -1;
		int nIterations = -1; 
		int nIntIterations = -1;
		double relaxedObj = -1;
		int nDivisible = -1;
		int nSingleService = -1;
		int nSimultanouesService = -1;
		int nTotalNodesServed = -1;
		int nRoutes = -1;
		double solTime = -1;
		double bestBound = -1;
		double irpRel = -1;
		void fillInfo();
		double getGap(int t);
		double getAverageObjective(int t);
		int MaxTime = -1;
		InstanceInfo(string name);
		const Information * getInfo(int time) const;
		list<Information> infoHolder;
		string Name;
		double getBestSolutionTime();
		double getPercentSolutionTime(double percent);
		void printInstanceToFile(double bestInfo = -1, string * filename = nullptr, bool heurestic = true);
		int getMaxTime();
		void addSolutionPoint(int state, double objectiveVal, double time);
	};

	vector<InstanceInfo> Instances;
	int MaxTime = -1;
	double getMaxTime();
	void printAllInstancesToFile(string name);
	void printAverageInstancesToFile(string name);
	InstanceInfo * newInstance(string name);


	SolutionInfo();
	~SolutionInfo();

};

#endif