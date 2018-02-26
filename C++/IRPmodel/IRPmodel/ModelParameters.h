#pragma once
class ModelParameters
{
public:
	ModelParameters();
	static const int TRANSCOST_MULTIPLIER = 15;
	static const int SERVICECOST_MULTIPLIER =50;
	static const int TRAVELTIME_MULTIPLIER = 1;
	static const int SERVICETIME = 10;
	static const int maxTime = 2000;
	static const int Capacity = 300;
	static const int CapacityPenalty = 10;
	static const int nVehicles = 3;

	//Heurestic
	//static const double VisitRatio = 0.5;

	//For generating customers
	//multiplied with number between 1 and 10
	static const int HoldingCost = 2; 
	static const int InitInventoryDel = 15;	//Delivery nodes
	static const int InitInventoryPick = 15; //Pickup nodes
	static const int DemandDel = 6;			//Delivery nodes
	static const int DemandPick = 6;		//Pickup nodes
	static const int LBDel = 2;				//Lower bound delivery nodes
	static const int LBPick = 0;			//Lower bound pickup
	static const int UBDel = 30;				//Lower bound pickup
	static const int UBPick = 38;			//Lower bound pickup


	//Valid inequalitites
	static const int MinimumNodeVisit = 1;

	~ModelParameters();
};

