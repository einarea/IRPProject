#include <vector>

using namespace std;

class ModelBase
{
public:
	ModelBase();
	static void createUnion(vector<int> & set1, vector<int> & set2, vector<int> & result);
	static void createRangeSet(int start, int end, vector <int> * set);
	~ModelBase();
};

