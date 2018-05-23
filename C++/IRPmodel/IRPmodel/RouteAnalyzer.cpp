#include "stdafx.h"
#include "RouteAnalyzer.h"


RouteAnalyzer::RouteAnalyzer()
{
}



RouteAnalyzer::Instance * RouteAnalyzer::getInstance(int id)
{
	for (Instance i : Instances) {
		if (i.Id == id)
			return &i;
	}

	exit(145);
}

RouteAnalyzer::Instance * RouteAnalyzer::getLatestInstance()
{
	return &Instances.back();
}

RouteAnalyzer::Instance * RouteAnalyzer::addInstance(int id)
{
	Instances.push_back(*new RouteAnalyzer::Instance(id));
	return &Instances.back();
}

double RouteAnalyzer::getAverageSelectedRoutes(int Selection)
{
	double average = 0;
	for (Instance i : Instances)
		average += (double)i.getAverageRouteTypeSelected(Selection) * 1 / Instances.size();

	return average;
}

double RouteAnalyzer::getAverageRouteType(int Selection)
{
	double average = 0;
	for (Instance i : Instances)
		average += (double) i.getAverageRouteType(Selection) * 1 / Instances.size();

	return average;
}

double RouteAnalyzer::getAverageTotalSelectedRoutes()
{
	double average = 0;
	for (Instance i : Instances)
		average += (double) i.getAverageTotalSelectedRoutes() * 1 / Instances.size();

	return average;
}

double RouteAnalyzer::getAverageTotalRoute()
{
	double average = 0;
	for (Instance i : Instances)
		average += (double) i.getAverageTotalRoutes() * 1/Instances.size();

	return average;
}


double RouteAnalyzer::getAverageRouteProp(int Selection)
{
	return (double) getAverageRouteType(Selection) / getAverageTotalRoute();
}

double RouteAnalyzer::getAverageSelectedRoutesProp(int Selection)
{
	return (double) getAverageSelectedRoutes(Selection) / getAverageTotalSelectedRoutes();
}

double RouteAnalyzer::getAverageImprovement()
{

	double average = 0;
	for (Instance i : Instances) {
		average += (double) i.getAverageImprovement() * 1 / Instances.size();
	}

	return average;
}

double RouteAnalyzer::getAverageRouteLength()
{
	double avg = 0;
	for (auto i : Instances)
		avg += i.getAvgRouteLength() * 1/Instances.size();

	return avg;
}

void RouteAnalyzer::printAverageToFile(string filename)
{
	//Print for every n'th second
	ofstream ins;
	ins.open("Heurestic/" + filename);

	ins << "Average route length:\t" << getAverageRouteLength() << "\t";

	ins << "Generated routes average\n";
	ins << "Total routes: " << getAverageTotalRoute()<<"\n";
	ins << "OrigRoute\t"<<getAverageRouteType(Route::ORIG) << "\n";
	ins << "SI\t" << getAverageRouteType(Route::SIMPLE_INSERTION) <<"\n";
	ins << "SR\t" << getAverageRouteType(Route::SIMPLE_REMOVAL) << "\n";
	ins << "InsRem\t" << getAverageRouteType(Route::INSERTION_REMOVAL) << "\n";
	ins << "Comb\t" << getAverageRouteType(Route::MERGE) << "\n";
	ins << "DoubleInsRem\t" << getAverageRouteType(Route::DOUBLE_INSERTION_REMOVAL) << "\n";
	ins << "LSIns\t" << getAverageRouteType(Route::LEAST_SERVED_INSERTION) << "\n";
	ins << "LSRem\t" << getAverageRouteType(Route::LEAST_SERVED_REMOVAL) << "\n";
	ins << "ResLSRem\t" << getAverageRouteType(Route::RESTRICTED_LEAST_SERVED_REMOVAL) << "\n";

	ins << "\n\nProportions";
	ins << "OrigRoute\t" << getAverageRouteProp(Route::ORIG) << "\n";
	ins << "SI\t" << getAverageRouteProp(Route::SIMPLE_INSERTION) << "\n";
	ins << "SR\t" << getAverageRouteProp(Route::SIMPLE_REMOVAL) << "\n";
	ins << "InsRem\t" << getAverageRouteProp(Route::INSERTION_REMOVAL) << "\n";
	ins << "Comb\t" << getAverageRouteProp(Route::MERGE) << "\n";
	ins << "DbINSRem\t" << getAverageRouteProp(Route::DOUBLE_INSERTION_REMOVAL) << "\n";
	ins << "LSIns\t" << getAverageRouteProp(Route::LEAST_SERVED_INSERTION) << "\n";
	ins << "LSrem\t" << getAverageRouteProp(Route::LEAST_SERVED_REMOVAL) << "\n";
	ins << "ResLSRem\t" << getAverageRouteProp(Route::RESTRICTED_LEAST_SERVED_REMOVAL) << "\n";

	ins << "\n\nSelected Routes average\n";
	ins << "Total routes: " << getAverageTotalSelectedRoutes()<<"\n";
	ins << "OrigRoute\t" << getAverageSelectedRoutes(Route::ORIG) << "\n";
	ins << "SI\t" << getAverageSelectedRoutes(Route::SIMPLE_INSERTION) << "\n";
	ins << "SR\t" << getAverageSelectedRoutes(Route::SIMPLE_REMOVAL) << "\n";
	ins << "InsRem\t" << getAverageSelectedRoutes(Route::INSERTION_REMOVAL) << "\n";
	ins << "Comb\t" << getAverageSelectedRoutes(Route::MERGE) << "\n";
	ins << "DbInsRem\t" << getAverageSelectedRoutes(Route::DOUBLE_INSERTION_REMOVAL) << "\n";
	ins << "LSins\t" << getAverageSelectedRoutes(Route::LEAST_SERVED_INSERTION) << "\n";
	ins << "LSrem\t" << getAverageSelectedRoutes(Route::LEAST_SERVED_REMOVAL) << "\n";
	ins << "ResLSRem\t" << getAverageSelectedRoutes(Route::RESTRICTED_LEAST_SERVED_REMOVAL) << "\n";

	ins << "\n\nProportions";
	ins << "OrigRoute\t" << getAverageSelectedRoutesProp(Route::ORIG) << "\n";
	ins << "SI\t" << getAverageSelectedRoutesProp(Route::SIMPLE_INSERTION) << "\n";
	ins << "SR\t" << getAverageSelectedRoutesProp(Route::SIMPLE_REMOVAL) << "\n";
	ins << "InsRem\t" << getAverageSelectedRoutesProp(Route::INSERTION_REMOVAL) << "\n";
	ins << "Comb\t" << getAverageSelectedRoutesProp(Route::MERGE) << "\n";
	ins << "DbInsrem\t" << getAverageSelectedRoutesProp(Route::DOUBLE_INSERTION_REMOVAL) << "\n";
	ins << "Lsins\t" << getAverageSelectedRoutesProp(Route::LEAST_SERVED_INSERTION) << "\n";
	ins << "LSrem\t" << getAverageSelectedRoutesProp(Route::LEAST_SERVED_REMOVAL) << "\n";
	ins << "ResLSRem\t" << getAverageSelectedRoutesProp(Route::RESTRICTED_LEAST_SERVED_REMOVAL) << "\n";

	ins << "\nImprovement: ";
	ins << getAverageImprovement() <<"\n";

	
	ins.close();
}

RouteAnalyzer::~RouteAnalyzer()
{
}

RouteAnalyzer::Instance::Instance(int id)
	:
	Id(id)
{
}

RouteAnalyzer::RoutePopulation* RouteAnalyzer::Instance::addIteration()
{
	Iterations.push_back(*new RoutePopulation());
	return &Iterations.back();
}

int RouteAnalyzer::Instance::getTotalRouteType(int Selection)
{
	int total = 0;
	for (auto it : Iterations)
		total += it.getTotalRouteType(Selection);

	return total;
}

double RouteAnalyzer::Instance::getAverageRouteType(int Sel)
{
	double total = 0;
	for (RoutePopulation it : Iterations)
		total += (double)it.getAverageTotalRouteType(Sel) * 1 / Iterations.size();

	return total;
}

int RouteAnalyzer::Instance::getTotalRoutes()
{
	int total = 0;
	for (auto it : Iterations)
		total += it.getTotalRoutes();

	return total;
}

double RouteAnalyzer::Instance::getAverageTotalRoutes()
{
	double total = 0;
	for (RoutePopulation it : Iterations)
		total += (double)it.getAverageTotalRoutes()* 1 / Iterations.size();

	return total;
}

double RouteAnalyzer::Instance::getAverageImprovement()
{
	double average = 0;
	for (auto pop : Iterations)
		average += (double) pop.Improvement * 1/Iterations.size();

	return average;
}

int RouteAnalyzer::Instance::getRouteTypeSelected(int Selection)
{
	int total = 0;
	for (auto it : Iterations)
		total += it.getRouteTypeSelected(Selection);

	return total;
}

double RouteAnalyzer::Instance::getAverageRouteTypeSelected(int sel)
{
	double total = 0;
	for (RoutePopulation it : Iterations)
		total += (double)it.getAverageRouteTypeSelected(sel) * 1 / Iterations.size();

	return total;
}

double RouteAnalyzer::Instance::getAvgRouteLength()
{
	double avg = 0;
	for (auto it : Iterations)
		avg += it.getAvgRouteLength() * 1/Iterations.size();

	return avg;
}

int RouteAnalyzer::Instance::getTotalSelectedRoutes()
{
	int total = 0;
	for (auto it : Iterations)
		total += it.getTotalSelectedRoutes();

	return total;
}

double RouteAnalyzer::Instance::getAverageTotalSelectedRoutes()
{
	double total = 0;
	for (RoutePopulation it : Iterations)
		total += (double)it.getAverageTotalSelectedRoutes() * 1 / Iterations.size();

	return total;
}

RouteAnalyzer::RoutePopulation * RouteAnalyzer::Instance::getLatestPopulation()
{
	return &Iterations.back();
}



RouteAnalyzer::RoutePopulation::RoutePopulation()
	:
	Improvement(0)
{
}

int RouteAnalyzer::RoutePopulation::getTotalRouteType(int Selection)
{
	int total = 0;
	for (auto gen : Generations)
		total += gen.getRouteTypeSelected(Selection);

	return total;
}

double RouteAnalyzer::RoutePopulation::getAverageTotalRouteType(int Selection)
{
	double total = 0;
	for (RouteGeneration gen : Generations)
		total += (double) gen.getRoutes(Selection) * 1 / Generations.size();

	return total;
}



int RouteAnalyzer::RoutePopulation::getTotalRoutes()
{
	int total = 0;
	for (RouteGeneration gen : Generations)
		total += gen.getTotalRoutes();

	return total;
}

double RouteAnalyzer::RoutePopulation::getAverageTotalRoutes()
{
	double total = 0;
	for (RouteGeneration gen : Generations)
		total += (double)gen.getTotalRoutes() * 1 / Generations.size();

	return total;
}



int RouteAnalyzer::RoutePopulation::getRouteTypeSelected(int Selection)
{
	int total = 0;
	for (auto gen : Generations)
		total += gen.getRouteTypeSelected(Selection);

	return total;
}

double RouteAnalyzer::RoutePopulation::getAverageRouteTypeSelected(int sel)
{
	double total = 0;
	for (RouteGeneration gen : Generations)
		total += (double)gen.getRouteTypeSelected(sel) * 1 / Generations.size();

	return total;
}

int RouteAnalyzer::RoutePopulation::getTotalSelectedRoutes()
{
	int total = 0;
	for (auto result : Generations) {
		total += result.getTotalRouteTypeSelected();
	}

	return total;
}

double RouteAnalyzer::RoutePopulation::getAverageTotalSelectedRoutes()
{
	double total = 0;
	for (RouteGeneration gen : Generations)
		total += (double)gen.getTotalRouteTypeSelected() * 1 / Generations.size();

	return total;
}

double RouteAnalyzer::RoutePopulation::getAvgRouteLength()
{
	double avg = 0;
	for (auto gen : Generations)
		avg += gen.avgRouteLength * 1/Generations.size();

	return avg;
}

RouteAnalyzer::RouteGeneration * RouteAnalyzer::RoutePopulation::addGeneration(const vector<Route*>& Routes)
{
	Generations.push_back(*new RouteGeneration());
	RouteAnalyzer::RouteGeneration * routeGeneration = &Generations.back();
	for (auto r : Routes)
		switch (r->State) {
		case Route::ORIG: {
			routeGeneration->Orig++;
			break;
		}
		case Route::SIMPLE_INSERTION: {
			routeGeneration->SI++;
			break;
		}

		case Route::SIMPLE_REMOVAL: {
			routeGeneration->SR++;
			break;
		}

		case Route::INSERTION_REMOVAL: {
			routeGeneration->Insrem++;
			break;
		}
		case Route::MERGE: {
			routeGeneration->Merge++;
			break;
		}
		case Route::LEAST_SERVED_INSERTION: {
			routeGeneration->LeastServedIns++;
			break;
		}
		case Route::LEAST_SERVED_REMOVAL: {
			routeGeneration->LeastServedRem++;
			break;
		}

		case Route::RESTRICTED_LEAST_SERVED_REMOVAL: {
			routeGeneration->ResLSRem++;
			break;
		}

		case Route::DOUBLE_INSERTION_REMOVAL: {
			routeGeneration->DoubleIns++;
			break;
		}

		default: {
			cout << "Error\n";
			break;
		}
		}

	return routeGeneration;

}

RouteAnalyzer::RouteGeneration * RouteAnalyzer::RoutePopulation::getLatestGeneration()
{
	return &Generations.back();
}

void RouteAnalyzer::RouteGeneration::addSelectedGeneration(const vector<Route*>& Routes)
{
	for (auto r : Routes)
		switch (r->State) {
		case Route::ORIG: {
			OrigSel++;
			break;
		}
		case Route::SIMPLE_INSERTION: {
			SISel++;
			break;
		}

		case Route::SIMPLE_REMOVAL: {
			SRSel++;
			break;
		}
		case Route::INSERTION_REMOVAL: {
			InsremSel++;
			break;
		}
		case Route::MERGE: {
			MergeSel++;
			break;
		}
		case Route::LEAST_SERVED_INSERTION: {
			LeastServedInsSel++;
			break;
		}
		case Route::LEAST_SERVED_REMOVAL: {
			LeastServedRemSel++;
			break;
		}
		case Route::RESTRICTED_LEAST_SERVED_REMOVAL: {
			ResLSRemSel++;
			break;
		}

		case Route::DOUBLE_INSERTION_REMOVAL: {
			DoubleInsSel++;
			break;
		}

		default: {
			cout << "Error\n";
			break;
		}
		}

	avgRouteLength = 0;
	for (auto r : Routes)
		avgRouteLength += (double) r->getSize() * 1 / Routes.size();
}




RouteAnalyzer::RouteGeneration::RouteGeneration()
	:
	Orig(0),
	SI(0),
	SR(0),
	Insrem(0),
	DoubleIns(0),
	LeastServedRem(0),
	ResLSRem(0),
	LeastServedIns(0),
	Merge(0),
	OrigSel(0),
	SISel(0),
	SRSel(0),
	InsremSel(0),
	DoubleInsSel(0),
	LeastServedRemSel(0),
	ResLSRemSel(0),
	LeastServedInsSel(0),
	MergeSel(0)

{
}

int RouteAnalyzer::RouteGeneration::getTotalRoutes()
{
	return Orig + SI + SR + Insrem + DoubleIns + LeastServedRem + ResLSRem + LeastServedIns + Merge;
}

int RouteAnalyzer::RouteGeneration::getRouteTypeSelected(int Selection)
{
	int average = 0;
	switch (Selection) {
	case Route::ORIG: {
		average += OrigSel;
		break;
	}
	case Route::SIMPLE_INSERTION: {
		average += SISel;
		break;
	}

	case Route::RESTRICTED_LEAST_SERVED_REMOVAL: {
		average += ResLSRemSel;
		break;
	}

	case Route::SIMPLE_REMOVAL: {
		average += SRSel;
		break;
	}

	case Route::INSERTION_REMOVAL: {
		average += InsremSel;
		break;
	}
	case Route::MERGE: {
		average += MergeSel;
		break;
	}
	case Route::LEAST_SERVED_INSERTION: {
		average += LeastServedRemSel;
		break;
	}
	case Route::LEAST_SERVED_REMOVAL: {
		average += LeastServedInsSel;
		break;
	}
	case Route::DOUBLE_INSERTION_REMOVAL: {
		average += DoubleInsSel;
		break;
	}
	}
	return average;
}

int RouteAnalyzer::RouteGeneration::getTotalRouteTypeSelected()
{
	return  OrigSel + SISel + SRSel + InsremSel + DoubleInsSel + LeastServedRemSel + ResLSRemSel + LeastServedInsSel + MergeSel;
}

int RouteAnalyzer::RouteGeneration::getRoutes(int Selection)
{
	int average = 0;

		switch (Selection) {
		case Route::ORIG: {
			average += Orig;
			break;
		}
		case Route::SIMPLE_INSERTION: {
			average += SI;
			break;
		}

		case Route::SIMPLE_REMOVAL: {
			average += SR;
			break;
		}

		case Route::INSERTION_REMOVAL: {
			average += Insrem;
			break;
		}
		case Route::MERGE: {
			average += Merge;
			break;
		}
		case Route::LEAST_SERVED_INSERTION: {
			average += LeastServedRem;
			break;
		}
		case Route::LEAST_SERVED_REMOVAL: {
			average += LeastServedIns;
			break;
		}

		case Route::RESTRICTED_LEAST_SERVED_REMOVAL: {
			average += ResLSRem;
			break;
		}

		case Route::DOUBLE_INSERTION_REMOVAL: {
			average += DoubleIns;
			break;
		}
		}
	
		return average;
}

