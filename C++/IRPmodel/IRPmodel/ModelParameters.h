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
	static const int Capacity = 150;
	static const int CapacityPenalty = 10;
	static const int nVehicles = 4;

	//For generating customers
	//multiplied with number between 1 and 10
	static const int HoldingCost = 1; 
	static const int InitInventoryDel = 8;	//Delivery nodes
	static const int InitInventoryPick = 8; //Pickup nodes
	static const int DemandDel = 12;			//Delivery nodes
	static const int DemandPick = 13;		//Pickup nodes
	static const int LBDel= 6;				//Lower bound delivery nodes
	static const int LBPick = 2;			//Lower bound pickup
	static const int UBDel = 18;				//Lower bound pickup
	static const int UBPick = 18;			//Lower bound pickup

	~ModelParameters();
};

