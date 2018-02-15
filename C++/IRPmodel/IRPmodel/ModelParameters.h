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
	static const int nVehicles = 3;
	~ModelParameters();
};

