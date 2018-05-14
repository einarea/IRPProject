#pragma once
class ModelParameters
{
public:

	//Running time
	static const int MAX_RUNNING_TIME_IRP = 60;//seconds
	static const int MAX_RUNNING_TIME_VRP = 60; //seconds
	static const int MAX_TIME_ROUTE_PROBLEM = 90; //seconds
	static const int INTENSIFICATION_MAX_TIME = 600; //seconds
	static const int TERMINATE_IF_NO_NEW_SOLUTION = 120; //seconds
	static const int HEURESTIC_TIME = 3600; // seconds
	static const int ROUTE_LOCK = 70;

	//Valid inequalitues
	static const int ExcessParameter = 30; //Pecentage excess above vehicle capacity. Only those with more are added

	//Selection for shift quantity
	static const int MAX_SHIFT = 3;
	static const int RESTRICTED_SHIFT = 4;
	static const int MINIMIZE_VISITS = 5;
	static const int SLACK = 10; //Slack in restricted shift constraint, % of capacity

	//Diversication holding cost
	static const int HOLDING_COST_INCREMENT = 5; //Used by addHoldingCostCtr, Holding cost allowed to increase in percentage, removed from objective function.

	//Diversication tabuList 
	static const int TabuLength = 2;  //Number of diversication iterations locked
	static const int TABU_LOCK = 20; //Percentage of changes to lock randomly.

	//Selection for route search
	static const int REQUIRE_CHANGE = 31;
	static const int NO_CHANGE = 46;

	////Model parameters;
	static const int nVehicles = 3;
	static const int TRANSCOST_MULTIPLIER = 13;
	static const int SERVICECOST_MULTIPLIER =100;
	static const int TRAVELTIME_MULTIPLIER = 1;
	static const int SERVICETIME = 20;
	static const int maxTime = 480;
	static const int VehiclePenalty = 100000;

	//Simultanous model
	static const bool Simultaneous = false;
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

	//**************Constant identifiers******************
	//Tabu list size
	static const int ForceVisits = 33;
	static const int ForceChanges = 34;

	//Valid inequalitites
	static const int MinimumNodeVisit = 22;
	static const int MinimumFlow = 23;
	static const int MinimumInventory = 24;
	static const int SubsetSizeMinFlow = 3;


	//Operators
	static const int REMOVE_SERVICE = 0;
	static const int HIGHEST_HOLDINGCOST = 1;
	static const int HIGHEST_TRANSPORTATIONCOST = 22;
	static const int HIGHEST_RESIDUAL_CAPACITY = 23;
	static const int HIGHEST_TOTALCOST = 2;

	//weights to color graphs
	static const int LOAD = 1;
	static const int X = 2;

	//Constants
	//Infeasibility identifiers
	static const int LOWER_LIMIT_BREAK = 51;
	static const int UPPER_LIMIT_BREAK = 52;
	static const int WITHIN_LIMITS = 53;

	//Route selection
	static const int MIN_SERVICE = 33;
	static const int CLOCKWISE = 54;
	static const int COUNTER_CLOCKWISE = 53;

	//STATE OF SOLUTION
	static const int ROUTE_SEARCH = 76;
	static const int SHIFT_QUANTITY = 77;
	static const int IRP_REL = 78;
	static const int VRP = 79;
	static const int INTENSIFICATAION_END = 80;
	static const int FINAL_SOL = 81;


	ModelParameters();
	~ModelParameters();
};

