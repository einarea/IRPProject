#pragma once
class ModelParameters
{
public:

	static const int MAX_RUNNING_TIME = 7200; //seconds

	ModelParameters();
	static const int TRANSCOST_MULTIPLIER = 13;
	static const int SERVICECOST_MULTIPLIER =100;
	static const int TRAVELTIME_MULTIPLIER = 1;
	static const int SERVICETIME = 20;
	static const int maxTime = 480;
	static const int Capacity;
	//static const int CapacityPenalty = 10000;
	static const int VehiclePenalty = 100000;
	static const int overTime = 3;
	static const int nVehicles = 3;

	//Heurestic
	//static const double VisitRatio = 0.5;

	//************For generating customers********************//
	//Ranges where values are randomly drawn from.

	//Holding cost
	static const int HoldingCostLow = 1;
	static const int HoldingCostHigh = 5;

	//Demand at delivery nodes and pickup nodes
	static const int DemandDelLow = 5;
	static const int DemandDelHigh = 50;
	static const int DemandPickLow = 5;
	static const int DemandPickHigh = 50;

	//Lower inventory bounds
	static const int LBDel = 5;				//Delivery
	static const int LBPic = 0;				//Pickup

	//Upper inventory limits
	static const int UBDelLow = 75;
	static const int UBDelHigh = 100;
	static const int UBPickLow = 75;
	static const int UBPickHigh = 100;

	static const int InitInventoryDel = 15;	//Delivery nodes
	static const int InitInventoryPick = 15; //Pickup nodes
	
	
	static const int LBPick = 0;		

	//Tabu list size
	static const int TabuLength = 2;

	//Valid inequalitites
	static const int MinimumNodeVisit = 1;


	//Operators
	static const int REMOVE_SERVICE = 0;
	static const int HIGHEST_HOLDINGCOST = 1;
	static const int HIGHEST_TOTALCOST = 2;

	//weights to color graphs
	static const int LOAD = 1;
	static const int X = 2;
	~ModelParameters();
};

