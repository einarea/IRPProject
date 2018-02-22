// IRPmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CustomerDB.h"
#include "CustomerVRPDB.h"
#include "IRP.h"
#include "VRPmodel.h"

#include <vector>
#include <cmath>
#include <boost/tuple/tuple.hpp>

#include "gnuplot-iostream.h"

using namespace std;

/*The construction heurestic solves an IRP with relaxed arc variables(x_ijt).
The solution provides information about which customers are visited in each period
and how much that are picked up/delivered at each customer.
For each time period a VRP is solved exactly for the visited nodes. */
double constructionHeurestic(CustomerIRPDB & db) {

	//Solve IRP with relaxed arc variables.
	bool relaxArcVariables = true;
	IRP model(db, relaxArcVariables);
	model.solveModel();

	//Vectors to hold visited customers and their demand
	vector<Customer *> customers;
	vector <vector<int>> demand;

	//Allocate new solution
	int id = model.allocateSolution();
	IRP::Solution *solIRP = model.getSolution(id);
	int idVRP = model.allocateSolution();
	IRP::Solution *solVRP = model.getSolution(idVRP);
	solVRP->printSolution(model);
	
	//Solve a VRP for each period
	for (int t = 1; t <= model.getNumOfPeriods(); t++) {
			model.getVisitedCustomers(t, customers);
			model.getDemand(t, demand, customers);
			if (customers.size() >= 1) {
				//Initialize a VRP database with the demand and customers
				CustomerVRPDB VRPdatabase(demand, customers);
				VRPmodel modelVRP(VRPdatabase, *model.getMap());
				modelVRP.solveModel();

				//Add partial solution to IRP
				modelVRP.addToIRPSolution(t, solVRP);
				//solVRP->printSolution(model);
				customers.clear();
			}
	}

	return solVRP->getObjective(&model);
	//solIRP->printSolution(model);
	
}//End construction heurestic




int main()
{
	//Graphic functionality requires gnuplot and boost libraries.
	//Boost 1.60 and gnuplot_iostream with VS 2015 is used in this project.
	FILE *gnuplotPipe = _popen("C:\\Octave\\3.2.4_gcc-4.4.0\\bin\\gnuplot", "w");
	Gnuplot gp(gnuplotPipe);
	// Create a script which can be manually fed into gnuplot later:
	//    Gnuplot gp(">script.gp");
	// Create script and also feed to gnuplot:
	//    Gnuplot gp("tee plot.gp | gnuplot -persist");
	// Or choose any of those options at runtime by setting the GNUPLOT_IOSTREAM_CMD
	// environment variable.

	// Gnuplot vectors (i.e. arrows) require four columns: (x,y,dx,dy)
	std::vector<boost::tuple<double, double, double, double> > pts_A;

	// You can also use a separate container for each column, like so:

	// You can also use a separate container for each column, like so:
	std::vector<double> pts_B_x;
	std::vector<double> pts_B_y;
	std::vector<double> pts_B_dx;
	std::vector<double> pts_B_dy;

	// You could also use:
	//   std::vector<std::vector<double> >
	//   boost::tuple of four std::vector's
	//   std::vector of std::tuple (if you have C++11)
	//   arma::mat (with the Armadillo library)
	//   blitz::Array<blitz::TinyVector<double, 4>, 1> (with the Blitz++ library)
	// ... or anything of that sort

	for (double alpha = 0; alpha<2; alpha ++) {
		pts_A.push_back(boost::make_tuple(
			0,
			0,
			1,
			1
			));

		pts_B_x.push_back(1);
		pts_B_y.push_back(1);
		pts_B_dx.push_back(-1);
		pts_B_dy.push_back(0);
	}

	// Don't forget to put "\n" at the end of each line!
	
	std::vector<std::pair<double, double> > xy_pts_A;
	for (double x = -2; x<2; x += 0.01) {
		double y = x*x*x;
		xy_pts_A.push_back(std::make_pair(x, y));
	}

	std::vector<std::pair<double, double> > xy_pts_B;
	for (double alpha = 0; alpha<1; alpha += 1.0 / 24.0) {
		double theta = alpha*2.0*3.14159;
		xy_pts_B.push_back(std::make_pair(cos(theta), sin(theta)));
	}

	gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
	gp << "plot '-' with lines title 'cubic', '-' with points lt rgb 'violet'   pointtype 7 pointsize 2 title 'circle'\n";
	gp.send1d(xy_pts_A);
	gp.send1d(xy_pts_B);

#ifdef _WIN32
	// For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
	// the gnuplot window doesn't get closed.
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
#endif
	/*Initialize database
	const int size = 10;
	double Solution[2][size];
	for (int i = 0; i < size; i++) {
		CustomerIRPDB *db = new CustomerIRPDB(6, 3);
		Solution[0][i] = constructionHeurestic(*db);

		IRP model(*db, true);
		IRP::Solution *sol = model.solveModel();
		Solution[1][i] = sol->getObjective(&model);
		delete db;
	}

	string s []= { "Constr", "Exact" };
	for (int i = 0; i < 2; i++) {
		printf("\n");
		cout<<s[i]<<": ";
		for (int j = 0; j < size; j++) {
			printf("%0.f\t", Solution[i][j]);
		}
	}*/
	//ofstream instanceFile;
	//string filename = "customerDataTest4.txt";
	//database.writeInstanceToFile(instanceFile, filename);
	//CustomerIRPDB database("CustomerData2.txt", true);
	
	//IRP model(database, false);
	//IRP::Solution *sol = model.solveModel();
	//sol->printSolution(model);
	
	return 0;
}


