#include <stdio.h>
#include "VRP.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "Map.h"
#include <math.h>
using namespace std;


int VRP::VRPMaster()
{
	int t = 3; //Sett manuelt
	int antallNoder = 20; //Sett manuelt. (Foreløpig er ikke depot en node)
	int noderBesoktArray[antallNoder][t];

	noderBesoktArrayFiller(&noderBesoktArray);

	for (int i = 1; i <= t; i++)
	{
		solution = solution + VRPSolver(int noderBesoktArray[][t], i, antallNoder);
	}

	return solution;
}


int VRP::VRPSolver(int noderBesoktArray[antallNoder][t], int t, int antallNoder)
{
	vector<int>v;
	vector<int>oversikt;
	vector<int>rekkefolge;
	vector<double>vinkel;

	int currentSolution = 0;
	int bestSolution = 1000000;		//Høyt tall som må tilpasses


	for (int j = 0; j <= antallNoder; j++)	// Fyller v-vektoren med nodene som skal med i VRPen
	{
		if (noderBesoktArray[j][t] != 0)
		{
			v.push_back(noderBesoktArray[j][t]);
			oversikt.push_back(j + 1);
		}

	}




	for (int k = 0; k < v.size(); k++)		// Løkke for å prøve alle nodene som startpunkt for sweep-heuristikken
	{
		rekkefolge.insert(0, k);
		vinkel.insert(0, 0);

		for (int l = 0; l < v.size(); l++)	// Løkke for å finne rekkefølgen
		{
			if (k != l)
			{

				double n1 = sqrt(getX(oversikt[k])*getX(oversikt[k]) + getY(oversikt[k])*getY(oversikt[k])), n2 = sqrt(getX(oversikt[k])*getX(oversikt[k]) + getY(oversikt[l])*getY(oversikt[l]));		//FIKS DETTE, håpr det funker
				double angle = acos((getX(oversikt[k])*getX(oversikt[l]) + getY(oversikt[k])*getY(oversikt[l])) / (n1*n2)) * 180 / M_PI;

				for (int m = 0; m < rekkefolge.size(); m++)
				{
					if (angle < vinkel[m])
					{
						rekkefolge.insert(m, l);
						vinkel.insert(m, angle);
					}

					if (angle == vinkel[m])
					{
						if (oversikt[k] < oversikt[rekkefolge[m]])
						{
							rekkefolge.insert(m, l);
							vinkel.insert(m, angle);
						}

						else
						{
							rekkefolge.insert(m+1, l);
							vinkel.insert(m+1, angle);
						}

					}

					if (m == (rekkefolge.size() - 1) && angle >= vinkel[m])
					{
						rekkefolge.push_back(l);
						vinkel.push_back(angle);
					}

				}

			}

		}

		currentSolution = solveVRPFunction(rekkefolge, v, oversikt);			//Finner objektivverdien for gjeldende startpunkt 

		if (currentSolution < bestSolution)
		{
			bestSolution = currentSolution;
		}

		rekkefolge.clear();
		vinkel.clear();

	}
}

int VRP::solveVRPFunction(vector<int>& rekkefolge, vector<int>& v, vector<int>& oversikt)
{

	customerDB database(filename);
	Map map(database);
	int capacityTracker = capacity;
	int travelTracker = maxTravel;
	bool while1 = false;
	bool while2 = false;
	int tracker = 0;
	int objektivVerdi = 0;
	int capacity = getcapacity();
	int MaxTravel = getMaxTravel();

	while (while1 == false)
	{
		objektivVerdi = objektivVerdi + getTransCost(0, oversikt[rekkefolge[tracker]]);
		capacityTracker = capacity - v[rekkefolge[tracker]];
		travelTracker = travelTracker - getDistance(0, oversikt[rekkefolge[tracker]])

			while (while2 == false)
			{
				if (tracker == rekkefolge.size())
				{
					objektivVerdi = objektivVerdi + getTransCost(oversikt[rekkefolge[tracker]], 0);
					return objektivVerdi;
				}

				if (capacityTracker - getTransCost(oversikt[rekkefolge[tracker]], oversikt[rekkefolge[tracker + 1]]) - getTransCost(oversikt[rekkefolge[tracker + 1]], 0) >= 0 && travelTracker - getDistance(oversikt[rekkefolge[tracker]], oversikt[rekkefolge[tracker + 1]]) - getDistance(oversikt[rekkefolge[tracker + 1]], 0) >= 0)
				{
					objektivVerdi = objektivVerdi + getTransCost(oversikt[rekkefolge[tracker]], oversikt[rekkefolge[tracker + 1]]);
					tracker = tracker + 1;
					capacityTracker = capacityTracker - v[rekkefolge[tracker + 1]];
					travelTracker = travelTracker - getDistance(oversikt[rekkefolge[tracker]], oversikt[rekkefolge[tracker + 1]]);
				}

				else
				{
					objektivVerdi = objektivVerdi + costs[oversikt[rekkefolge[tracker]]][0];
					tracker = tracker + 1;
					while2 = true;
					capacityTracker = capacity;
					travelTracker = maxTravel;
				}
			}

	}


	return objektivVerdi;

}

int VRP::getcapacity()					//Må lages
{
	return 0;
}

int VRP::getMaxTravel()					//Må lages
{
	return 0;
}

void VRP::noderBesoktArrayFiller(int* noderBesoktArray)			//Må lages
{
}