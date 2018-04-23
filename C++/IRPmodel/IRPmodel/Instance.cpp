#include "stdafx.h"
#include "Instance.h"
#include "Instance.h"


Instance::Instance()
{
	//Initialize sets
	if (!initializeSets()) {
		cout << "Data Error: Could not initialize sets.";
		return;
	}
}


Instance::Instance(CustomerIRPDB &)
{
}

Instance::~Instance()
{
}



