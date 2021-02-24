#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <GL/glew.h>
#include <vector>
#include <memory>


#include "Maths.h"
#include "AABB.h"


class CPolygon
{
private:
	friend class CWorld;

	CPolygon(size_t index);
public:
	~CPolygon();

	Vec2				position;
	Mat2				rotation;
	std::vector<Vec2>	points;

	void				Build();

	SAABB* const		GetAABB();
	void				Draw();
	size_t				GetIndex() const;

	float				GetArea() const;

	Vec2				TransformPoint(const Vec2& point) const;
	Vec2				InverseTransformPoint(const Vec2& point) const;

	// if point is outside then returned distance is negative (and doesn't make sense)
	bool				IsPointInside(const Vec2& point) const;

	bool				CheckCollision(const CPolygon& poly, Vec2& colPoint, Vec2& colNormal, float& colDist) const;

	// Physics
	float				density;
	Vec2				speed;

private:

	void				BuildAABB();
	bool				IsSeparatingAxis(const CPolygon& poly, const Vec2 normal, Vec2& colPoint, Vec2& penetrationVector) const;
	void				ProjectPointsOnNormal(const Vec2 normal, float& min, float& max, Vec2& colPoint) const;

	void				CreateBuffers();
	void				BindBuffers();
	void				DestroyBuffers();

	void				BuildLines();

	GLuint				m_vertexBufferId;
	size_t				m_index;

	std::vector<Line>	m_lines;

	SAABB				m_aabb;
};

typedef std::shared_ptr<CPolygon>	CPolygonPtr;

#endif