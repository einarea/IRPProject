// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CustomerDB.h"
#include "IRP.h"


int main()
{
	IRP model("CustomerData.txt");
	model.solveLP();
    return 0;
}

