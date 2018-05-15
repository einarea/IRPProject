#pragma once
#include <vector>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "ModelParameters.h"

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

		void fillInfo();
		InstanceInfo(string name);
		const Information * getInfo(int time) const;
		list<Information> infoHolder;
		string Name;
		void printInstanceToFile(double bestInfo = -1);
		int getMaxTime();
		void addSolutionPoint(int state, double objectiveVal, double time);
	};

	vector<InstanceInfo> Solutions;

	void addSolution(InstanceInfo*);

	void printAllInstancesToFile(string name);
	InstanceInfo * newInstance(string name);


	SolutionInfo();
	~SolutionInfo();

};

