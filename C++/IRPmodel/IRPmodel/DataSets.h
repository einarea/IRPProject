#pragma once
#include <stdlib.h>
#include <initializer_list>

using namespace std;
#define SequentialEnum(Name,...) \
enum Name { __VA_ARGS__ };  \
namespace DataSets\
{ \
	const initializer_list<Name> Name##List{ __VA_ARGS__ }; \
}

SequentialEnum(Model,
	Exact,
	Construction,
);

SequentialEnum(Content,
	HoldingCost,
	TransCost,
	Routes,
	Objective,
	nNodeVisits,
	Service,
);



SequentialEnum(CostData,
	HoldExact,
	TransExact,
	deltaHold,
	deltaTrans,
	deltaObjective,
);

SequentialEnum(RouteData,
	nVehicles,
	nNodes,
	dService
);

SequentialEnum(DataType,
	Average,
	StdDev,
);






