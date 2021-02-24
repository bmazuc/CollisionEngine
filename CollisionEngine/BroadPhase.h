#ifndef _BROAD_PHASE_H_
#define _BROAD_PHASE_H_

#include "PhysicEngine.h"

class IBroadPhase
{
public:
	virtual void AddPolygon(CPolygonPtr polygon)
	{

	}

	virtual void RemovePolygon(CPolygonPtr polygon)
	{

	}

	virtual void Update()
	{

	}

	virtual void GetCollidingPairsToCheck(std::vector<SPolygonPair>& pairsToCheck) = 0;
};

#endif