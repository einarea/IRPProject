#pragma once
#include <vector>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "Route.h"
#include "ModelParameters.h"

using namespace std;

class RouteAnalyzer
{
public:
	RouteAnalyzer();

	class RouteGeneration {
	public:
		RouteGeneration();
		int Orig;
		int SI;
		int Insrem;
		int DoubleIns;
		int LeastServedRem;
		int ResLSRem;
		int LeastServedIns;
		int Merge;
		int SR;

		int getTotalRoutes();
		int getRouteTypeSelected(int Selection);
		int getTotalRouteTypeSelected();
		int getRoutes(int Selection);
		void addSelectedGeneration(const vector<Route*>& routes);

		//Selection
		int OrigSel;
		int SISel;
		int SRSel;
		int InsremSel;
		int DoubleInsSel;
		int LeastServedRemSel;
		int ResLSRemSel;
		int LeastServedInsSel;
		int MergeSel;


		double avgRouteLength;
		double ImprovementSel;
	};

	class RoutePopulation {
	public:

		RoutePopulation();
		vector<RouteGeneration> Generations;
		double Improvement;
		//Generation
		//Returns average of all intensification iterations
		int getTotalRouteType(int Selection);
		double getAverageTotalRouteType(int selection);
		int getTotalRoutes();
		double getAverageTotalRoutes();

		int getRouteTypeSelected(int Selection);
		double getAverageRouteTypeSelected(int sel);

		int getTotalSelectedRoutes();
		double getAverageTotalSelectedRoutes();

		double getAvgRouteLength();

		RouteGeneration * addGeneration(const vector <Route*>& routes);
		RouteGeneration * getLatestGeneration();
	
	
	};

	class Instance {
	public:
		Instance(int id);
		vector<RoutePopulation> Iterations;
		RoutePopulation* addIteration();
		int getTotalRouteType(int Selection);
		double getAverageRouteType(int Sel);

		int getTotalRoutes();
		double getAverageTotalRoutes();

		double getAverageImprovement();
		int getRouteTypeSelected(int Selection);
		double getAverageRouteTypeSelected(int sel);

		double getAvgRouteLength();

		int getTotalSelectedRoutes();
		double getAverageTotalSelectedRoutes();
		RoutePopulation * getLatestPopulation();
		int Id;

	};

	vector<Instance> Instances;
	
	Instance * getInstance(int id);
	Instance * getLatestInstance();

	Instance * addInstance(int id);
	double getAverageSelectedRoutes(int Selection);
	double getAverageRouteType(int Selection);
	double getAverageTotalSelectedRoutes();
	double getAverageTotalRoute();
	
	double getAverageRouteProp(int Selection);
	double getAverageSelectedRoutesProp(int Selection);
	double getAverageImprovement();

	double getAverageRouteLength();

	void printAverageToFile(string filename);

	~RouteAnalyzer();
};

