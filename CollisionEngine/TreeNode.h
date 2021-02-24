#ifndef _TREE_NODE_H_
#define _TREE_NODE_H_

#include "AABB.h"
#include "Polygon.h"

struct STreeNode
{
public:
	STreeNode();
	STreeNode(STreeNode const& node)			= delete;
	STreeNode(STreeNode&& node)					= delete;
	~STreeNode();

	STreeNode& operator=(STreeNode const& node)	= delete;
	STreeNode& operator=(STreeNode&& node)		= delete;

	bool IsLeaf();

	void SetBranch(STreeNode* node0, STreeNode* node1);
	void SetLeaf(CPolygonPtr _data);

	void ChangeChild(STreeNode* original, STreeNode* node);
	void UpdateAABB(float margin);

	STreeNode* GetSibling();

	bool		childrenChecked;

	STreeNode*	parent;
	STreeNode*	children[2];

	SAABB		aabb;
	CPolygonPtr	data;

	void ClearChildren();
};

#endif
