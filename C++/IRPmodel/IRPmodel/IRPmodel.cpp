// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CustomerDB.h"
#include "IRP.h"


int main()
{
	CustomerDB database("CustomerData.txt");
	IRP model(database);
	model.solveLP();
	model.sepStrongComponents();
    return 0;
}

