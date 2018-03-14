#include "stdafx.h"
#include "CustomerIRPDB.h"


CustomerIRPDB::CustomerIRPDB(string FileName, bool COORDINATE_INPUT)
{
	ifstream CustomerRecords(FileName, ios::in);

	if (!CustomerRecords)
	{
		cerr << "Customer records file could not be opened" << endl;
		exit(1);
	}


	string line;

	//Get number of periods
	getline(CustomerRecords, line);
	string delimiter = "=";
	getNextToken(line, delimiter);
	nPeriods = stoi(getNextToken(line, delimiter));


	//Push back depot
	Customers.push_back(new Customer(0, 0, 0));
	//check file for errors
	getline(CustomerRecords, line);

	int CustomerID = 1;
	int * HoldingCost;
	int ** Demand;
	int * InitInventory;
	int * LowerLimit;
	int * UpperLimit;


	while (!getline(CustomerRecords, line).eof()) {

		size_t pos = 0;
		string token;
		delimiter = "\t";

		//Inventory parameters
		HoldingCost = new int[2];
		LowerLimit = new int[2];
		UpperLimit = new int[2];

		for (int i = 0; i < 2; i++) {
			HoldingCost[i] = stoi(getNextToken(line, delimiter));
		}

		for (int i = 0; i < 2; i++) {
			LowerLimit[i] = stoi(getNextToken(line, delimiter));
		}
		for (int i = 0; i < 2; i++) {
			UpperLimit[i] = stoi(getNextToken(line, delimiter));
		}

		//Dynamically allocated array
		Demand = new int *[2]; //pickup and delivery demand

		for (int i = 0; i < 2; i++) {
			Demand[i] = new int[nPeriods];
			for (int t = 0; t < nPeriods; t++) {
				Demand[i][t] = stoi(getNextToken(line, delimiter));
			}
		}

		//Inital inventory
		InitInventory = new int[2]; //pickup and delivery demand
		for (int i = 0; i < 2; i++) {
			InitInventory[i] = stoi(getNextToken(line, delimiter));
		}

		//If datafile provide coordinates
		if (COORDINATE_INPUT) {
			int x = stoi(getNextToken(line, delimiter));
			int y = stoi(getNextToken(line, delimiter));
			CustomerIRP * cust = new CustomerIRP(CustomerID, HoldingCost, LowerLimit, UpperLimit, Demand, InitInventory, x, y);
			Customers.push_back(cust);
			CustomerID++;
		}

		else {
			CustomerIRP * cust = new CustomerIRP(CustomerID, HoldingCost, LowerLimit, UpperLimit, Demand, InitInventory);
			Customers.push_back(cust);
			CustomerID++;
		}
	}
}

CustomerIRPDB::CustomerIRPDB(int numberOfCustomers, int nPer)
{
	//Push back depot
	Customers.push_back(new Customer(0, 0, 0));

	nPeriods = nPer;
	for (int i = 1; i <= numberOfCustomers; i++){
		Customers.push_back(generateCustomer(i, nPeriods, i*10));
	}
	
}

void CustomerIRPDB::writeInstanceToFile(ofstream &instanceFile, string Filename)
{
	instanceFile.open(Filename);
	instanceFile << "nPeriods = " << getnPeriods() << "\n";
	instanceFile << "HD\tHP\tLLD\tLLP\tULD\tULP\t";	

	for (int t = 1 ; t <= getnPeriods(); t++) {
		instanceFile << "D" << t << "\t";
	}

	for (int t = 1; t <= getnPeriods(); t++) {
		instanceFile << "P" << t << "\t";
	}

	instanceFile << "I01\tI02\tX\tY\n";

	for (Customer *c : Customers) {
		if (c->getId() != 0) {
			CustomerIRP * cust = getCustomer(c);

			instanceFile << cust->getHoldCost(Customer::DELIVERY) << "\t" << cust->getHoldCost(Customer::PICKUP)
				<< "\t" << cust->getLowerLimit(Customer::DELIVERY) << "\t" << cust->getLowerLimit(Customer::PICKUP)
				<< "\t" << cust->getUpperLimit(Customer::DELIVERY) << "\t" << cust->getUpperLimit(Customer::PICKUP) << "\t";

			for (int t = 1; t <= getnPeriods(); t++) {
				instanceFile << cust->getDemand(t, Customer::DELIVERY) << "\t";
			}
			for (int t = 1; t <= getnPeriods(); t++) {
				instanceFile << cust->getDemand(t, Customer::PICKUP) << "\t";
			}

			instanceFile << cust->getInitInventory(Customer::DELIVERY) << "\t";
			instanceFile << cust->getInitInventory(Customer::PICKUP) << "\t";
			instanceFile << cust->getXpos() << "\t" << cust->getYpos() << "\n";
		}
	}
	
	instanceFile << "end-of-file";
	instanceFile.close();
}



int CustomerIRPDB::getnPeriods()
{
	return nPeriods;
}

int CustomerIRPDB::getHoldCost(int id, int indicator)
{
	return getCustomer(id)->getHoldCost(indicator);
}

int CustomerIRPDB::getUpperLimit(int id, int indicator)
{
	return getCustomer(id)->getUpperLimit(indicator);
}


int CustomerIRPDB::getInitInventory(int id, int indicator)
{
	return getCustomer(id)->getInitInventory(indicator);
}

int CustomerIRPDB::getLowerLimit(int id, int indicator)
{
	
	return getCustomer(id)->getLowerLimit(indicator);
}

CustomerIRP * CustomerIRPDB::getCustomer(int id) {
	CustomerIRP * derivedPtr = static_cast <CustomerIRP *> (CustomerDB::getCustomer(id));
	if (derivedPtr != 0)
		return derivedPtr;
}

int CustomerIRPDB::getDemand(int id, int period, int indicator)
{
	return getCustomer(id)->getDemand(period, indicator);
}

CustomerIRP * CustomerIRPDB::getCustomer(Customer * c)
{
	CustomerIRP * derivedPtr = static_cast <CustomerIRP *> (c);
	if (derivedPtr != 0)
		return derivedPtr;
}

string CustomerIRPDB::getNextToken(string &str, string& delimiter)
{
	size_t pos = 0;
	pos = str.find(delimiter);
	string token = str.substr(0, pos);
	str.erase(0, pos + delimiter.length());
	return token;
}



CustomerIRPDB::~CustomerIRPDB()
{
}

CustomerIRP * CustomerIRPDB::generateCustomer(int id, int periods, int randSeed)
{
	return new CustomerIRP(id, periods, randSeed);
}

