
#include "stdafx.h"
#include "SolutionInfo.h"

SolutionInfo::InstanceInfo * SolutionInfo::newInstance(string name)
{
	Instances.push_back(*new SolutionInfo::InstanceInfo(name));
	return &Instances.back();
}

SolutionInfo::SolutionInfo()
{
}


double SolutionInfo::getMaxTime()
{
	double maxTime = 100000;
	for (auto i : Instances)
		if (i.infoHolder.back().Time < maxTime)
			maxTime = i.infoHolder.back().Time;

	return maxTime;
}

void SolutionInfo::printAllInstancesToFile(string name)
{
	//Print for every n'th second
	/*ofstream ins;
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
	ins.close();*/
}

void SolutionInfo::printAverageInstancesToFile(string name)
{
	//Print for every n'th second

	double maxTime = getMaxTime();

	ofstream ins;
	ins.open("Heurestic/" + name + ".txt");

	double average = 0;
	int n;
	double gap;
	for (int t = 1; t <= maxTime; t++){
		n = 0;
		average = 0;
		for (InstanceInfo i : Instances) {
			gap = i.getGap(t);
			if (gap != -1) {
				n++;
				average += gap;
			}
		}

		ins << "Time:\t" << t << "\tGap from lowerbound:\t" << to_string((double) average/n) << "\n";
	
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

	//insert maxtime
	MaxTime = infoHolder.back().Time;
}

double SolutionInfo::InstanceInfo::getGap(int t)
{
	double avg = getAverageObjective(t);
	if (avg == -1)
		return -1;
	else
		return (getAverageObjective(t) - bestBound) / bestBound;
}

//returns 0 if not defined for t
double SolutionInfo::InstanceInfo::getAverageObjective(int t)
{
	auto pos = infoHolder.begin();
	while (pos->Time < t && pos->State != "FINAL")
		pos++;

	double total = 0;
	int n = 0;
	while (pos->Time < t + 1 && pos->State != "FINAL") {
		total += pos->ObjectiveVal;
		pos++;
		n++;
	}

	if (n == 0)
		return -1;
	else
		return (double) total / n;
}

SolutionInfo::InstanceInfo::InstanceInfo(string name)
	:
	Name(name)
{
}

const SolutionInfo::Information * SolutionInfo::InstanceInfo::getInfo(int time) const
{
	auto pos = infoHolder.begin();
	while (pos->Time <= time)
		pos++;

	return &*pos;
}

double SolutionInfo::InstanceInfo::getBestSolutionTime()
{
	for (auto solutionPoint : infoHolder)
		if (solutionPoint.ObjectiveVal < (bestObjective+0.5))
			return solutionPoint.Time;

	exit(145);
}

double SolutionInfo::InstanceInfo::getPercentSolutionTime(double percent)
{
	for (auto solutionPoint : infoHolder)
		if (solutionPoint.ObjectiveVal < bestObjective* (1 + (double) percent/100))
			return solutionPoint.Time;

	exit(145);
}

void SolutionInfo::InstanceInfo::printInstanceToFile(double bBound, string * filename, bool heurestic)
{
	bestBound = bBound;
	ofstream ins;
	if (heurestic) {
		ins.open("Heurestic/" + Name + ".txt");
	}
	else
		ins.open("Exact/" + *filename + ".txt");

	ins << "Best objective:\t" << this->bestObjective << "\n";
	if (heurestic) {
		ins << "IRP relaxed sol:\t" << this->relaxedObj << "\n";
		ins << "IRP relaxed dual bound:\t" << this->irpRel << "\n";
	}
	ins << "Best bound:\t" << this->bestBound << "\n";
	ins << "Best sol found after:\t" << getBestSolutionTime() << "\n";
	ins << "0.5% from best sol after:\t" << getPercentSolutionTime(0.5) << "\n";
	ins << "1% from best sol after:\t" << getPercentSolutionTime(1) << "\n";
	ins << "2% from best sol after:\t" << getPercentSolutionTime(2) << "\n";
	ins << "5% from best sol after: \t" << getPercentSolutionTime(5) << "\n";
	ins << "10% from best sol after: \t" << getPercentSolutionTime(1) << "\n";
	ins << "nDivisible:\t" << this->nDivisible << "\n";
	ins << "nSimultaneous:\t" << this->nSimultanouesService << "\n";
	ins << "Total nodes served:\t" << this->nTotalNodesServed << "\n";
	ins << "Single visits:\t" << this->nSingleService << "\n";
	ins << "nRoutes:\t" << this->nRoutes << "\n";

	if (heurestic) {
		ins << "Diversification iterations:\t" << nIterations << "\n";
		ins << "Intensification iterations:\t" << nIntIterations << "\n";

		ins << "Route pool:\t" << nRoutePool << "\n";
	}

	ins << "Solution time:\t" << this->solTime << "\n";

	if (heurestic) {
		ins << "\nSolution method\n";
		if (ModelParameters::Simultanoues_RelaxedIRP)
			ins << "Simultanous IRP\n";
		else
			ins << "Divisible IRP\n";

	}

	ins << "\nSubtour elimination\n";
	if (ModelParameters::SUBTOUR_ELIMINATION) {
		ins << "Alpha:\t" << (double) ModelParameters::ALPHA/100 << "\n";
		ins << "Edge weight:\t" << (double) ModelParameters::EDGE_WEIGHT/100 << "\n";
	}
	else{
		ins << "Alpha:\t" << "-" << "\n";
		ins << "Edge weight:\t" << "-" << "\n";
		
	}

	if (heurestic) {
		ins << "\nHeurestic parameters\n";
		ins << "Heurestic time:\t" << ModelParameters::HEURESTIC_TIME << "\n";
		ins << "Intensification time:\t" << ModelParameters::INTENSIFICATION_TIME << "\n";
		ins << "IRP time\t:" << ModelParameters::MAX_RUNNING_TIME_IRP << "\n";
		ins << "VRP time\t:" << ModelParameters::MAX_RUNNING_TIME_VRP << "\n";
		if (ModelParameters::RouteChange)
			ins << "Min Route change:\t" << ModelParameters::ROUTE_LOCK << "%\n";

		if (ModelParameters::MinChanges)
			ins << "Min changes:\t" << ModelParameters::MIN_CHANGE << "%\n";
	}


	ins << "\nSolution process\n";
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

	case ModelParameters::BBNode: {
		stateStr = "BB";
		break;
	}
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
