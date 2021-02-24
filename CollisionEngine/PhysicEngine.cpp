#include "PhysicEngine.h"

#include <iostream>
#include <string>
#include "GlobalVariables.h"
#include "World.h"
#include "Renderer.h" // for debugging only
#include "Timer.h"

#include "BroadPhase.h"
#include "BroadPhaseBrut.h"

#include "AABBTree.h"

void	CPhysicEngine::Reset()
{
	m_pairsToCheck.clear();
	m_collidingPairs.clear();

	m_active = true;

	//m_broadPhase = new CBroadPhaseBrut();
	m_broadPhase = new CAABBTree();
}

void	CPhysicEngine::Activate(bool active)
{
	m_active = active;
}

void	CPhysicEngine::DetectCollisions()
{
	CTimer timer;
	timer.Start();
	CollisionBroadPhase();
	timer.Stop();
	if (gVars->bDebug)
	{
		gVars->pRenderer->DisplayText("Collision broadphase duration " + std::to_string(timer.GetDuration() * 1000.0f) + " ms");
	}

	timer.Start();
	CollisionNarrowPhase();
	timer.Stop();
	if (gVars->bDebug)
	{
		gVars->pRenderer->DisplayText("Collision narrowphase duration " + std::to_string(timer.GetDuration() * 1000.0f) + " ms, collisions : " + std::to_string(m_collidingPairs.size()));
	}
}

void	CPhysicEngine::Step(float deltaTime)
{
	if (!m_active)
	{
		return;
	}

	DetectCollisions();
	m_broadPhase->Update();
}

void	CPhysicEngine::CollisionBroadPhase()
{
	m_pairsToCheck.clear();
	m_broadPhase->GetCollidingPairsToCheck(m_pairsToCheck);

	/*if (m_pairsToCheck.empty())
		return;*/

	/*std::cout << "-------------------" << std::endl;
	for (int i = 0; i < m_pairsToCheck.size(); ++i)
	{
		std::cout << "Poly A : " << m_pairsToCheck[i].polyA->position.x << ", " << m_pairsToCheck[i].polyA->position.y << " | Poly B : " << m_pairsToCheck[i].polyB->position.x << ", " << m_pairsToCheck[i].polyB->position.y << std::endl;
	}
	std::cout << "-------------------" << std::endl;*/
}

void	CPhysicEngine::CollisionNarrowPhase()
{
	m_collidingPairs.clear();
	for (const SPolygonPair& pair : m_pairsToCheck)
	{
		SCollision collision;
		collision.polyA = pair.polyA;
		collision.polyB = pair.polyB;
		if (pair.polyA->CheckCollision(*(pair.polyB), collision.point, collision.normal, collision.distance)) 
		{
			m_collidingPairs.push_back(collision);
		}
	}
}

void CPhysicEngine::AddPolygon(CPolygonPtr polygon)
{
	m_broadPhase->AddPolygon(polygon);
}

/*void CPhysicEngine::RemovePolygon(CPolygonPtr polygon)
{
	m_broadPhase->RemovePolygon(polygon);
}*/