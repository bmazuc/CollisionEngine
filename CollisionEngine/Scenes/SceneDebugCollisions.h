#ifndef _SCENE_DEBUG_COLLISIONS_H_
#define _SCENE_DEBUG_COLLISIONS_H_

#include "BaseScene.h"

#include "Behaviors/DisplayCollision.h"
#include "AABBTree.h"

class CSceneDebugCollisions : public CBaseScene
{
private:
	virtual void Create() override
	{
		CBaseScene::Create();

		CPolygonPtr firstPoly = gVars->pWorld->AddTriangle(30.0f, 20.0f); 
		//CPolygonPtr firstPoly = gVars->pWorld->AddRectangle(30.0f, 20.0f);
		firstPoly->density = 0.0f;
		firstPoly->position = Vec2(-5.0f, -5.0f);
		firstPoly->Build();
		gVars->pPhysicEngine->AddPolygon(firstPoly);

		CPolygonPtr secondPoly = gVars->pWorld->AddTriangle(25.0f, 20.0f);
		//CPolygonPtr secondPoly = gVars->pWorld->AddRectangle(25.0f, 20.0f);

		/*float width = gVars->pRenderer->GetWorldWidth();
		float height = gVars->pRenderer->GetWorldHeight();
		SRandomPolyParams params;
		params.minRadius = params.maxRadius = 10.0f;
		params.minBounds = params.maxBounds = Vec2(1.f, 1.f);
		params.minPoints = params.maxPoints = 8;
		CPolygonPtr secondPoly = gVars->pWorld->AddRandomPoly(params);*/

		secondPoly->position = Vec2(20.0f, 5.0f);
		secondPoly->density = 0.0f;
		secondPoly->Build();
		gVars->pPhysicEngine->AddPolygon(secondPoly);

		/*CPolygonPtr thirdPoly = gVars->pWorld->AddTriangle(20.0f, 20.0f);
		thirdPoly->position = Vec2(10.0f, 5.0f);
		thirdPoly->density = 0.0f;
		thirdPoly->Build();
		thirdPoly->BuildAABB();
		gVars->pPhysicEngine->AddPolygon(thirdPoly);*/

		CDisplayCollision* displayCollision = static_cast<CDisplayCollision*>(gVars->pWorld->AddBehavior<CDisplayCollision>(nullptr).get());
		displayCollision->polyA = firstPoly;
		displayCollision->polyB = secondPoly;
		displayCollision->Start();

/*		CDisplayCollision* displayCollisiong = static_cast<CDisplayCollision*>(gVars->pWorld->AddBehavior<CDisplayCollision>(nullptr).get());
		displayCollisiong->polyA = firstPoly;
		displayCollisiong->polyB = thirdPoly;
		displayCollisiong->Start();

		CDisplayCollision* displayCollisiond = static_cast<CDisplayCollision*>(gVars->pWorld->AddBehavior<CDisplayCollision>(nullptr).get());
		displayCollisiond->polyA = thirdPoly;
		displayCollisiond->polyB = secondPoly;
		displayCollisiond->Start();*/

	}
};

#endif