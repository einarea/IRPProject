#include "stdafx.h"
#include "ModelBase.h"


ModelBase::ModelBase()
{
}

void ModelBase::createUnion(vector<int> &set1, vector<int> &set2, vector<int> &result)
{

	result = vector<int>(set1.size() + set2.size());

	for (int i = 0; i<set1.size(); i++)
	{
		result[i] = set1[i];
	}

	for (int i = set2.size(); i<set2.size(); i++)
	{
		result[i] = set2[i - set1.size()];
	}
}

void ModelBase::createRangeSet(int start, int end, vector<int> * set)
{
	(*set) = vector<int>(end - start + 1);

	for (int i = 0; i < end; i++) {
		(*set)[i] = start + i;
	}
}


ModelBase::~ModelBase()
{
}
