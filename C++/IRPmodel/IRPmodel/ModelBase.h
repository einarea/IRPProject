#include "stdafx.h"
#include "xprb_cpp.h"
#include <vector>
#include "Node.h"
#include "xprs.h"

using namespace ::dashoptimization;
using namespace std;

class ModelBase
{
public:
	ModelBase();
	static void createUnion(vector<int> & set1, vector<int> & set2, vector<int> & result);
	static void createRangeSet(int start, int end, vector <int> & set);
	static vector<int> createDifferenceSet(vector <int> & set1, vector <int> & set2);
	static int getMax(vector <int>);
	~ModelBase();
};

