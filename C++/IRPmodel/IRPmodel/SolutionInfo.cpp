
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

SolutionInfo::InstanceInfo::InstanceInfo(string name)
	:
	Name(name)
{
}

const SolutionInfo::Information * SolutionInfo::InstanceInfo::getInfo(int time) const
{
	int i = 0;
	for (int i = 0; i < infoHolder.size(); i++)
		if (infoHolder[i].Time == time)
			return &infoHolder[i];

	return nullptr;
}

void SolutionInfo::InstanceInfo::printInstanceToFile()
{
	ofstream ins;
	ins.open("Heurestic/" + Name);

	for (Information v : infoHolder) {
		ins << v.State << "\t";
		ins << v.Time << "\t";
		ins << v.ObjectiveVal << "\n";
	}

	ins.close();
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
