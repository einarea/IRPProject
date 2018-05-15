
#include "stdafx.h"
#include "SolutionInfo.h"

SolutionInfo::InstanceInfo * SolutionInfo::newInstance(string name)
{
	return new InstanceInfo(name);
}

SolutionInfo::SolutionInfo()
{
}


void SolutionInfo::addSolution(InstanceInfo * instance)
{
	Solutions.push_back(*instance);
}

void SolutionInfo::printAllInstancesToFile(string name)
{
	//Print for every n'th second
	ofstream ins;
	ins.open("Heurestic/" +name);

	int n = 0;
	int increment = 1;

	while (n <= 60) {
		ins << n << "\t";
		for (InstanceInfo i : Solutions) {
			const Information * info = i.getInfo(n);
			if (info != nullptr) {
				ins << info->ObjectiveVal << "\t";
			}
		}
		ins << "\n";
		n += increment;
	}
	ins.close();
}


SolutionInfo::~SolutionInfo()
{
}


void SolutionInfo::InstanceInfo::fillInfo()
{
	double n;
	double obj;
	auto pos = infoHolder.begin();
	while (pos->State != "FINAL") {
		n = pos->Time + 1;
		obj = pos->ObjectiveVal;
		pos++;
		while (n != pos->Time + 1){
			if(pos->State != "CON" || pos->Time == infoHolder.begin()->Time)
				infoHolder.insert(pos, *new Information("N/A", obj, n));
			else
				infoHolder.insert(pos, *new Information("N/A", -1, n));
			n++;
		}
	}
}

SolutionInfo::InstanceInfo::InstanceInfo(string name)
	:
	Name(name)
{
}

const SolutionInfo::Information * SolutionInfo::InstanceInfo::getInfo(int time) const
{
	return 0;
}

void SolutionInfo::InstanceInfo::printInstanceToFile(double bestBound)
{
	ofstream ins;
	ins.open("Heurestic/" + Name);

	if (bestBound == -1) {

		for (Information &info : infoHolder) {
			if (info.State != "FINAL") {
				ins << info.State << "\t";
				ins << info.Time << "\t";
				if (info.ObjectiveVal == -1) {
					ins << "\t";
					ins << infoHolder.back().ObjectiveVal << "\n";
				}
				else {
					ins << info.ObjectiveVal << "\t";
					ins << infoHolder.back().ObjectiveVal << "\n";
				}



			}
		}
	}
	else {
		for (Information &info : infoHolder) {
			if (info.State != "FINAL") {
				ins << info.State << "\t";
				ins << info.Time << "\t";
				if (info.ObjectiveVal == -1) {
					ins << "\t";
					ins << (infoHolder.back().ObjectiveVal - bestBound) / bestBound << "\n";
				}
				else {
					ins << (info.ObjectiveVal-bestBound)/bestBound << "\t";
					ins << (infoHolder.back().ObjectiveVal-bestBound)/bestBound << "\n";
				}

			}
		}
	}


	ins.close();
}

int SolutionInfo::InstanceInfo::getMaxTime()
{
	return infoHolder.back().Time;
}

void SolutionInfo::InstanceInfo::addSolutionPoint(int state, double objectiveVal, double time)
{
	string stateStr;
	switch (state) {

	case ModelParameters::ROUTE_SEARCH: {
		stateStr = "RS";
		break;
	}

	case ModelParameters::VRP:{
		stateStr = "CON";
		break;
	}

	case ModelParameters::SHIFT_QUANTITY: {
		stateStr = "SQ";
		break;
	}
	case ModelParameters::INTENSIFICATAION_END: {
		stateStr = "INT";
		break;
	}

	case ModelParameters::FINAL_SOL: {
		stateStr = "FINAL";
		break;
	}

	default: {
		stateStr = "Err";
		break;
	}
	}

	infoHolder.push_back(*new Information(stateStr, objectiveVal, time));
}

SolutionInfo::Information::Information(string state, double objectiveVal, double time)
	:
	State(state),
	ObjectiveVal(objectiveVal),
	Time(time)
{
}
