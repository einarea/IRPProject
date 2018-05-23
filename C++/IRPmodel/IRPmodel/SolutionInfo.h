#pragma once
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

		double bestExactSolution = -1;
		void fillInfo();
		double getGap(int t);
		double getAverageObjective(int t);
		int MaxTime = -1;
		InstanceInfo(string name);
		const Information * getInfo(int time) const;
		list<Information> infoHolder;
		string Name;
		void printInstanceToFile(double bestInfo = -1);
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

