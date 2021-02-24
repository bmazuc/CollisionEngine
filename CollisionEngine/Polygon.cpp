#include "Polygon.h"
#include <GL/glu.h>
#include <iostream>
#include <vector>

#include "GlobalVariables.h"
#include "PhysicEngine.h"
#include "AABBTree.h"

CPolygon::CPolygon(size_t index)
	: m_vertexBufferId(0), m_index(index), density(0.1f)
{
}

CPolygon::~CPolygon()
{
	DestroyBuffers();
}

void CPolygon::Build()
{
	m_lines.clear();

	CreateBuffers();
	BuildLines();
	BuildAABB();
}

void CPolygon::Draw()
{
	// Set transforms (qssuming model view mode is set)
	float transfMat[16] = {	rotation.X.x, rotation.X.y, 0.0f, 0.0f,
							rotation.Y.x, rotation.Y.y, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f,
							position.x, position.y, -1.0f, 1.0f };
	glPushMatrix();
	glMultMatrixf(transfMat);

	// Draw vertices
	BindBuffers();
	glDrawArrays(GL_LINE_LOOP, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	BuildAABB();
}

size_t	CPolygon::GetIndex() const
{
	return m_index;
}

Vec2	CPolygon::TransformPoint(const Vec2& point) const
{
	return position + rotation * point;
}

Vec2	CPolygon::InverseTransformPoint(const Vec2& point) const
{
	return rotation.GetInverse() * (point - position);
}

bool	CPolygon::IsPointInside(const Vec2& point) const
{
	float maxDist = -FLT_MAX;

	for (const Line& line : m_lines)
	{
		Line globalLine = line.Transform(rotation, position);
		float pointDist = globalLine.GetPointDist(point);
		maxDist = Max(maxDist, pointDist);
	}

	return maxDist <= 0.0f;
}

bool	CPolygon::CheckCollision(const CPolygon& poly, Vec2& colPoint, Vec2& colNormal, float& colDist) const
{
	//Implements Separating Axis Theorem
	Vec2 penetrationVector = Vec2(FLT_MAX, FLT_MAX);

	Vec2 pointA, pointB, lineDir;
	for (int idx = 0; idx < m_lines.size(); ++idx)
	{
		pointA = points[idx];
		pointB = points[(idx + 1) % points.size()];

		lineDir = (pointB - pointA).Normalized();

		Vec2 normal = lineDir.GetNormal();

		if (IsSeparatingAxis(poly, normal, colPoint, penetrationVector))
			return false;
	}

	for (int idx = 0; idx < points.size(); ++idx)
	{
		pointA = points[idx];
		pointB = points[(idx + 1) % points.size()];

		lineDir = (pointB - pointA).Normalized();

		Vec2 normal = lineDir.GetNormal();

		if (IsSeparatingAxis(poly, normal, colPoint, penetrationVector))
			return false;
	}

	colDist = std::abs(penetrationVector.GetLength());
	colNormal = penetrationVector.Normalized();

	return true;
}

bool CPolygon::IsSeparatingAxis(const CPolygon& poly, const Vec2 normal, Vec2& colPoint, Vec2& penetrationVector) const
{
	float minA = FLT_MAX;
	float minB = FLT_MAX;
	float maxA = -FLT_MAX;
	float maxB = -FLT_MAX;

	Vec2 colPointA, colPointB;

	ProjectPointsOnNormal(normal, minA, maxA, colPointA);
	poly.ProjectPointsOnNormal(normal, minB, maxB, colPointB);

	if (maxA >= minB && maxB >= minA)
	{
		float dist = Min(maxB - minA, maxA - minB);

		if (dist < penetrationVector.GetLength())
		{
			penetrationVector = normal * dist;

			if (maxB > maxA)
			{
				colPoint = colPointA - penetrationVector;
			}
			else
			{
				penetrationVector *= -1.f;
				colPoint = colPointB;
			}
		}

		return false;
	}

	return true;
}

void CPolygon::ProjectPointsOnNormal(const Vec2 normal, float& min, float& max, Vec2& colPoint) const
{
	for (int idx = 0; idx < points.size(); ++idx)
	{
		Vec2 transformedPoint = TransformPoint(points[idx]);
		float projection = transformedPoint.DotProduct(normal);
		min = Min(min, projection);

		if (projection > max)
		{
			max = projection;
			colPoint = transformedPoint;
		}
	}
}

void CPolygon::CreateBuffers()
{
	DestroyBuffers();

	float* vertices = new float[3 * points.size()];
	for (size_t i = 0; i < points.size(); ++i)
	{
		vertices[3 * i] = points[i].x;
		vertices[3 * i + 1] = points[i].y;
		vertices[3 * i + 2] = 0.0f;
	}

	glGenBuffers(1, &m_vertexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * points.size(), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
}

void CPolygon::BindBuffers()
{
	if (m_vertexBufferId != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (void*)0);
	}
}

void CPolygon::DestroyBuffers()
{
	if (m_vertexBufferId != 0)
	{
		glDeleteBuffers(1, &m_vertexBufferId);
		m_vertexBufferId = 0;
	}
}

void CPolygon::BuildLines()
{
	for (size_t index = 0; index < points.size(); ++index)
	{
		const Vec2& pointA = points[index];
		const Vec2& pointB = points[(index + 1) % points.size()];

		Vec2 lineDir = (pointA - pointB).Normalized();

		m_lines.push_back(Line(pointB, lineDir));
	}
}

void CPolygon::BuildAABB()
{
	m_aabb.Build(position, points);
}

SAABB* const CPolygon::GetAABB()
{
	return &m_aabb;
}