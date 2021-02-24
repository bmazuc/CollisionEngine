#include "AABB.h"
#include <iostream>
#include "GlobalVariables.h"
#include "Renderer.h"

SAABB::SAABB()
	: center(), halfwidth()
{
}

SAABB::SAABB(Vec2 const& _center, Vec2 const& _halfwidth)
	: center(_center), halfwidth(_halfwidth)
{
}

void SAABB::Build(Vec2 const& _center, std::vector<Vec2> const& points)
{
	Vec2 point, min, max;
	for (int idx = 0; idx < points.size(); ++idx)
	{
		point = points[idx];
		
		if (point.x > max.x)
		{
			max.x = point.x;
		}
		else if (point.x < min.x)
		{
			min.x = point.x;
		}

		if (point.y > max.y)
		{
			max.y = point.y;
		}
		else if (point.y < min.y)
		{
			min.y = point.y;
		}
	}
	
	halfwidth	= (max - min) / 2.f;
	center		= _center  + ((max + min) / 2.f);
}

void SAABB::Draw(float r, float g, float b)
{
	Vec2 min = center - halfwidth;
	Vec2 max = center + halfwidth;

	gVars->pRenderer->DrawLine(Vec2(min.x, min.y), Vec2(min.x, max.y), r, g, b);
	gVars->pRenderer->DrawLine(Vec2(min.x, max.y), Vec2(max.x, max.y), r, g, b);
	gVars->pRenderer->DrawLine(Vec2(max.x, max.y), Vec2(max.x, min.y), r, g, b);
	gVars->pRenderer->DrawLine(Vec2(max.x, min.y), Vec2(min.x, min.y), r, g, b);
}

float SAABB::Area()
{
	return 4 * halfwidth.x * halfwidth.y;
}

bool SAABB::Contains(SAABB other)
{
	Vec2 min = center - halfwidth;
	Vec2 max = center + halfwidth;

	Vec2 otherMin = other.center - other.halfwidth;
	Vec2 otherMax = other.center + other.halfwidth;

	return min.x <= otherMin.x && max.x >= otherMax.x
		&& min.y <= otherMin.y && max.y >= otherMax.y;
}

SAABB SAABB::Merge(SAABB other)
{
	Vec2 min = center - halfwidth;
	Vec2 max = center + halfwidth;

	Vec2 otherMin = other.center - other.halfwidth;
	Vec2 otherMax = other.center + other.halfwidth;

	if (min.x > otherMin.x)
		min.x = otherMin.x;
	if (max.x < otherMax.x)
		max.x = otherMax.x;

	if (min.y > otherMin.y)
		min.y = otherMin.y;
	if (max.y < otherMax.y)
		max.y = otherMax.y;

	SAABB result;
	result.halfwidth = (max - min) / 2.f;
	result.center = ((max + min) / 2.f);

	return result;
}

bool SAABB::Overlaps(SAABB other)
{
	Vec2 min = center - halfwidth;
	Vec2 max = center + halfwidth;

	Vec2 otherMin = other.center - other.halfwidth;
	Vec2 otherMax = other.center + other.halfwidth;

	return max.x > otherMin.x && min.x < otherMax.x
		&& max.y > otherMin.y && min.y < otherMax.y;
}