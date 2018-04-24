#include "stdafx.h"
#include "CustomerVRPDB.h"


/*CustomerVRP * CustomerVRPDB::getCustomer(int id)
{
	for (Customer *c : Customers) {
		if (c->getId() == id) {
			CustomerVRP * derivedPtr = static_cast <CustomerVRP *> (CustomerDB::getCustomer(id));
			if (derivedPtr != 0)
				return derivedPtr;
		}
	}
}

//Recieves demand and position for the customers
CustomerVRPDB::CustomerVRPDB(vector<vector<double>> & Demand,  vector<Customer *> &customers)
{
	for (Customer * c : customers) {
		CustomerVRP * cust = new CustomerVRP(c->getId(), c->getXpos(), c->getYpos(), Demand[Customer::DELIVERY][c->getId()], Demand[Customer::PICKUP][c->getId()]);
		Customers.push_back(cust);
	}
}

CustomerVRPDB::CustomerVRPDB(vector<IRP::NodeIRP *> nodes, Map& map)
{
	for (IRP::NodeIRP * node : nodes) {
		//Only add if not exisiting
		bool exists = false;

		for (Customer * c1 : Customers) {
			CustomerVRP * c = static_cast <CustomerVRP*> (c1);
			//if customer exisits
			if (map.getCustomer(node->getId())->getId() == c->getId()) {
				exists = true;
				if (map.isDelivery(node->getId()))
					c->setDelivery(node->Quantity);
				else
					c->setPickup(node->Quantity);
			}
		}

		//if customer is not added
		if(! exists){
				Customer * cust = map.getCustomer(node->getId());

				//Push back customer with delivery or pickup
				if (map.isDelivery(node->getId()))
					Customers.push_back(new CustomerVRP(cust->getId(), cust->getXpos(), cust->getYpos(), node->Quantity, 0));
				else
					Customers.push_back(new CustomerVRP(cust->getId(), cust->getXpos(), cust->getYpos(), 0, node->Quantity));
		} //end not exists
		//todo, sort(Customers);
	} //end nodes
	
		
}

int CustomerVRPDB::getnCustomers()
{
	return Customers.size();
}



int CustomerVRPDB::getDemand(int id, int indicator)
{
	return getCustomer(id)->getDemand(indicator);
}


CustomerVRPDB::~CustomerVRPDB()
{
}*/
