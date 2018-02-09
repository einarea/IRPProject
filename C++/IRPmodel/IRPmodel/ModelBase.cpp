#include "stdafx.h"
#include "ModelBase.h"


ModelBase::ModelBase()
{
}

void ModelBase::createUnion(vector<int> &set1, vector<int> &set2, vector<int> &result)
{
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



ModelBase::~ModelBase()
{
}
