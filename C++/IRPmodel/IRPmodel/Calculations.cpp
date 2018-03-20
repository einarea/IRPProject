#include "stdafx.h"
#include "Calculations.h"

using namespace DataSets;


Calculations::Calculations(vector<double> *** modelData, int instances, int periods)
	:
	nInstances(instances),
	nPeriods(periods),
	nModels(ModelList.size()),
	nFields(ContentList.size())
{

	//Initialize dataholders
	total = new double**[nModels];
	totalAvg = new vector<double> *[nModels];
	totCostAvg = new double*[nModels];
	varTot = new double **[nModels];
	var = new double **[nModels];

	for (auto i : ModelList) {
		total[i] = new double *[nFields];
		totalAvg[i] = new vector<double> [nFields];
		totCostAvg[i] = new double[nFields];
		varTot[i] = new double *[nModels];
		var[i] = new double *[nModels];

		for (auto j : ContentList) {
			totCostAvg[i][j] = 0;
			total[i][j] = new double[nPeriods + 1];
			totalAvg[i][j].resize(nPeriods + 1);
			varTot[i][j] = new double[nPeriods + 1];
			var[i][j] = new double[nPeriods + 1];


			for (int t = 1; t <= nPeriods; t++) {
				total[i][j][t] = 0;
				totalAvg[i][j][t] = 0;
				varTot[i][j][t] = 0;
				var[i][j][t] = 0;
			}
		}
	}

	//Initialize cost data
	CostData = new double **[CostDataList.size()];
	for (auto field : CostDataList) {
		CostData[field] = new double *[nPeriods + 2];
		for (int t = 1; t <= nPeriods + 1; t++) {
			CostData[field][t] = new double[2];
			for (auto i : DataTypeList)
				CostData[field][t][i] = 0;
		}
	}

	//Initialize routedata
	RouteData = new double **[RouteDataList.size()];

	for (auto field : RouteDataList) {
		RouteData[field] = new double *[nPeriods + 2];
		for (int t = 1; t <= nPeriods + 1; t++) {
			RouteData[field][t] = new double[2];
			for (auto i : DataTypeList)
				RouteData[field][t][i] = 0;
		}
	}

	AnalyzeModel(modelData);
}

void Calculations::AnalyzeModel(vector<double> *** & modelData)
{

	//Calculate the average over all instances
	for (auto model : ModelList)
		for (auto field : ContentList) {
			double A = 0;
			for (int period = 1; period <= nPeriods; period++) {
				totalAvg[model][field][period] += getAverage(modelData[model][field][period]);
				double a = getAverage(modelData[model][field][period]);
				totCostAvg[model][field] += totalAvg[model][field][period];
			}
			A = totCostAvg[model][field];
		
			int c = 1;
		}


	//Calculate the sample standard deviation over all instances
	/*for (auto model : ModelList)
		for (auto field : ContentList)
			for (int period = 1; period <= nPeriods; period++)
				var[model][field][period] += getStdDev(modelData[model][field][period], totalAvg[model][field][period]);*/
}

double Calculations::getAverage(vector<double> &values)
{
	double avg = 0;
	for (auto v : values)
		avg += v * 1/size(values);

	return avg;
}
double Calculations::getStdDev(vector<double> &v, double average = -1)
{
	double stdDev = 0;
	if (average == -1)
		average = getAverage(v);

	for (auto val : v)
		stdDev = sqrt(pow(val - average, 2) / (v.size() - 1));

	return stdDev;
}


double *** Calculations::getRouteInformation()
{
	vector<int> RouteSet = { Routes, nNodeVisits, Delivery, Pickup};

	for (int t = 1; t <= nPeriods; t++) {
		RouteData[nVehicles][t][Average] = totalAvg[Construction][Routes][t] - totalAvg[Exact][Routes][t];
		RouteData[nNodes][t][Average] = (totalAvg[Construction][nNodeVisits][t] - totalAvg[Exact][nNodeVisits][t]) / totCostAvg[Exact][nNodeVisits] * 100;
		RouteData[dDelivery][t][Average] = (totalAvg[Construction][Delivery][t] - totalAvg[Exact][Delivery][t]) / totCostAvg[Exact][Delivery] * 100;
		RouteData[dPickup][t][Average] = (totalAvg[Construction][Pickup][t] - totalAvg[Exact][Pickup][t]) / totCostAvg[Exact][Pickup] * 100;

	}
	//Calculate the cumulative vehicles
	RouteData[nVehicles][nPeriods + 1][Average] = totCostAvg[Construction][Routes] - totCostAvg[Exact][Routes];
	RouteData[nNodes][nPeriods + 1][Average] = (totCostAvg[Construction][nNodeVisits]- totCostAvg[Exact][nNodeVisits]) / totCostAvg[Exact][nNodeVisits]*100;
	RouteData[dDelivery][nPeriods + 1][Average] = (totCostAvg[Construction][Delivery] - totCostAvg[Exact][Delivery]) / totCostAvg[Exact][Delivery] * 100;
	RouteData[dPickup][nPeriods + 1][Average] = (totCostAvg[Construction][Pickup] - totCostAvg[Exact][Pickup]) / totCostAvg[Exact][Pickup] * 100;
	return RouteData;
}

double  *** Calculations::getCostDistribution()
{
	vector <int> CostSet = { HoldingCost, TransCost, Objective };

	
	vector<double> * hold;
	vector <double> * trans;
	vector <double> * deltaH;
	vector<double> * deltaT;
	vector<double> * deltaObj;

	hold = new vector<double>[nPeriods+1];
	trans = new vector<double>[nPeriods+1];
	deltaH = new vector<double>[nPeriods+1];
	deltaT = new vector<double>[nPeriods+1];
	deltaObj = new vector<double>[nPeriods+1];

	for (int t = 1; t <= nPeriods; t++) {
		hold[t].resize(nInstances);
		trans[t].resize(nInstances);
		deltaH[t].resize(nInstances);
		deltaT[t].resize(nInstances);
		deltaObj[t].resize(nInstances);
	}

	//Calculate cost distribution for each period
	for (int t = 1; t <= nPeriods; t++) {
		double a = totCostAvg[Exact][Objective];
		double b = totalAvg[Exact][HoldingCost][t];
		hold[t][Average] = totalAvg[Exact][HoldingCost][t] / totCostAvg[Exact][Objective] * 100;
		double c = b / a * 100;
		trans[t][Average] = totalAvg[Exact][TransCost][t] / totCostAvg[Exact][Objective] * 100;
	
		deltaH[t][Average] = (totalAvg[Construction][HoldingCost][t] - totalAvg[Exact][HoldingCost][t]) / totCostAvg[Exact][HoldingCost] * 100;
		deltaT[t][Average] = (totalAvg[Construction][TransCost][t] - totalAvg[Exact][TransCost][t]) / totCostAvg[Exact][TransCost] * 100;
	
		deltaObj[t][Average] = (totalAvg[Construction][Objective][t] - totalAvg[Exact][Objective][t]) / totCostAvg[Exact][Objective] * 100;

		CostData[HoldExact][t][Average] = hold[t][Average];
		double d = CostData[HoldExact][t][Average];
		CostData[TransExact][t][Average] = trans[t][Average];
		CostData[deltaHold][t][Average] = deltaH[t][Average];
		
		CostData[deltaTrans][t][Average] = deltaT[t][Average];
		CostData[deltaObjective][t][Average] = deltaObj[t][Average];

	}

	//Calculate the cumulative cost distribution
	double holdTot = totCostAvg[Exact][HoldingCost]/ totCostAvg[Exact][Objective] * 100;
	double transTot = totCostAvg[Exact][TransCost] / totCostAvg[Exact][Objective] * 100;

	double deltaHTot = (totCostAvg[Construction][HoldingCost] - totCostAvg[Exact][HoldingCost]) / totCostAvg[Exact][HoldingCost] * 100;
	double deltaTTot = (totCostAvg[Construction][TransCost] - totCostAvg[Exact][TransCost]) / totCostAvg[Exact][TransCost] * 100;
	double deltaObjTot = (totCostAvg[Construction][Objective] - totCostAvg[Exact][Objective]) / totCostAvg[Exact][Objective] * 100;

	CostData[HoldExact][nPeriods + 1][Average] = holdTot;
	CostData[TransExact][nPeriods + 1][Average] = transTot;
	CostData[deltaHold][nPeriods + 1][Average] = deltaHTot;
	CostData[deltaTrans][nPeriods + 1][Average] = deltaTTot;
	CostData[deltaObjective][nPeriods + 1][Average] = deltaObjTot;

	return CostData;
}
				




Calculations::~Calculations()
{
	int a = 1;
}
