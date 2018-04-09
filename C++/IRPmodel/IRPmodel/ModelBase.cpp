#include "stdafx.h"
#include "ModelBase.h"


ModelBase::ModelBase()
{
}

void ModelBase::createUnion(vector<int> &set1, vector<int> &set2, vector<int> &result)
{
	//result.resize(set1.size() + set2.size());
	for (int i = 0; i<set1.size(); i++)
	{
		result.push_back(set1[i]);
	}

	for (int i = 0; i<set2.size(); i++)
	{
		result.push_back(set2[i]);
	}
}



void ModelBase::createRangeSet(int start, int end, vector<int> &set)
{
	for (int i = start; i <= end; i++) {
		set.push_back(i);
	}
}

//Removes set2 from set1
vector<int> ModelBase::createDifferenceSet(vector<int>& set1, vector<int>& set2)
{
	vector<int> difference;
	bool include;
	for (auto i : set1) {
		include = true;
		for (auto j : set2) {
			if (i == j)
				include = false;
		}

		if (include)
			difference.push_back(i);
	}

	return difference;
}

int ModelBase::getMax(vector<int> values)
{
	int max = -10000;
	for (auto i : values)
		if (i > max)
			max = i;
	return max;
}



ModelBase::~ModelBase()
{
}
