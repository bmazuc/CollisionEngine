#ifndef _AABB_TREE_H_
#define _AABB_TREE_H_

#include <vector>
#include <map>
#include "BroadPhase.h"

class	IAABB;
struct	STreeNode;
struct	SPolygonPair;

class CAABBTree : public IBroadPhase
{
public:
	CAABBTree(float margin = 0.2f);
	CAABBTree(CAABBTree const& tree) = delete;
	CAABBTree(CAABBTree&& tree) = delete;
	~CAABBTree();

	CAABBTree& operator=(CAABBTree const& tree) = delete;
	CAABBTree& operator=(CAABBTree&& tree) = delete;

	void AddPolygon(CPolygonPtr polygon);

	void Update() override;

	virtual void GetCollidingPairsToCheck(std::vector<SPolygonPair>& pairsToCheck) override;

	void PrintTree();

private:
	void DrawNodeAABB(STreeNode* node);

	void Insert(STreeNode* node, STreeNode* parent);
	void Extract(STreeNode* node);

	void CheckNodeValidity(STreeNode* node);

	void ComputeCollidingPairsToCheck(STreeNode* leftNode, STreeNode* rightNode);
	
	void CheckNodeChildren(STreeNode* node);
	void ClearNodeChildrenCheckedFlag(STreeNode* node);


	void PrintNode(STreeNode* node, std::string decal = "");
	
	STreeNode* m_root;

	float m_margin;

	std::vector<STreeNode*> m_invalidNodes;
	std::vector<SPolygonPair> m_pairsList;
};

#endif
