#include "stdafx.h"
#include "IRP.h"
#include "ModelBase.h"
using namespace ::dashoptimization;

IRP::IRP(FILE *InstanceFile)
{
	//Initialize sets, parameters to model
	//(*map) = Map(InstanceFile);									//map of customers
	if(!initializeSets()) {
		cout<<"Data Error: Could not initialize sets.";
		return;
	}
	
	//Initialize problem in BCL
	prob = XPRBprob("IRP");
	
	if (!initializeVariables())
		return;
	


}

XPRBprob & IRP::getProblem() {
	return prob;
}


bool IRP::initializeSets()
{
	NumOfCustomers = 3;//(*map).getNumCustomers();					//Number of customers
	NumOfPeriods = 2;

	ModelBase::createRangeSet(1, NumOfPeriods, &Periods);
	ModelBase::createRangeSet(1, NumOfCustomers, &DeliveryNodes);
	ModelBase::createRangeSet(1, NumOfCustomers, &DeliveryNodes);
	ModelBase::createRangeSet(1, NumOfCustomers, &PickupNodes);
	ModelBase::createUnion(DeliveryNodes, PickupNodes, Nodes);
	ModelBase::createRangeSet(1, 1, &Depot);
	ModelBase::createUnion(Depot, Nodes, AllNodes);
	return true;
}

bool IRP::initializeVariables()
{

	for (int i : AllNodes) {
		for (int j : AllNodes) {
			for (int t : Periods) {
				if (i != j) {
					x[i][j][t] = prob.newVar("x", XPRB_PL, 0, 200);
					loadDelivery[i][j][t] = prob.newVar(XPRBnewname("loadDelivery_%d%d%d", i, j, t), XPRB_PL, 0, 100);
					loadPickup[i][j][t] = getProblem().newVar(XPRBnewname("loadPickup_%d%d%d", i, j, t), XPRB_PL, 0, 100);
				}

			}
		}

		for (int i : AllNodes) {
			for (int t : Periods)
				y[i][t] = getProblem().newVar(XPRBnewname("y_%d", i), XPRB_PL, 0, 1);


			for (int i : DeliveryNodes) {
				for (int t : Periods) {
					delivery[i][t] = getProblem().newVar(XPRBnewname("y_%d%d", i, t), XPRB_PL, 0, 100);
					inventory[i][t] = getProblem().newVar(XPRBnewname("inventory_%d%d", i, t), XPRB_PL, 0, 100);
				}
			}

			for (int i : PickupNodes)
				for (int t : Periods) {
					pickup[i][t] = getProblem().newVar(XPRBnewname("y_%d%d", i, t), XPRB_PL, 0, 1);
					inventory[i][t] = getProblem().newVar(XPRBnewname("inventory_%d%d", i, t), XPRB_PL, 0, 100);
				}

			return true;
		}
	}
}
// end initialize variables



int IRP::getNumOfCustomers()
{
	return NumOfCustomers;
}

int IRP::getNumOfPeriods(IRP * model)
{
	return (*model).NumOfPeriods;
}


IRP::~IRP()
{
}
