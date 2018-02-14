#pragma once
class ModelParameters
{
public:
	ModelParameters();
	static const int TRANSCOST_MULTIPLIER = 10;
	static const int SERVICECOST_MULTIPLIER = 2;
	static const int TRAVELTIME_MULTIPLIER = 1;
	static const int SERVICETIME = 10;
	static const int maxTime = 2000;
	static const int Capacity = 1000;
	static const int nVehicles = 4;

	~ModelParameters();
};

