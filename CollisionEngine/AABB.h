#ifndef _AABB_H_
#define _AABB_H_

#include <vector>

#include "Maths.h"

struct SAABB
{
	SAABB();
	SAABB(Vec2 const& _center, Vec2 const& _halfwidth);
	void Build(Vec2 const& _center, std::vector<Vec2> const& points);
	void Draw(float r = 0.f, float g = 1.f, float b = 0.f);

	float	Area();
	bool	Contains(SAABB other);
	SAABB	Merge(SAABB other);
	bool	Overlaps(SAABB other);

	Vec2 center;
	Vec2 halfwidth;
};
#endif
