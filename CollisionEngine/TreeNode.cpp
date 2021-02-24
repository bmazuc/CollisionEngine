#include "TreeNode.h"
#include "AABB.h"

STreeNode::STreeNode()
{
	parent = nullptr;
	data = nullptr;
	childrenChecked = false;
	ClearChildren();
}

STreeNode::~STreeNode()
{
	if (children[1])
		delete children[1];

	if (children[0])
		delete children[0];

	ClearChildren();
}

bool STreeNode::IsLeaf()
{
	return children[0] == nullptr;
}

void STreeNode::SetBranch(STreeNode* node0, STreeNode* node1)
{
	node0->parent = this;
	node1->parent = this;

	children[0] = node0;
	children[1] = node1;
}

void STreeNode::SetLeaf(CPolygonPtr _data)
{
	data = _data;
	aabb = *data->GetAABB();
	ClearChildren();
}

void STreeNode::ClearChildren()
{
	children[0] = nullptr;
	children[1] = nullptr;
}

void STreeNode::ChangeChild(STreeNode* original, STreeNode* node)
{
	if (children[0] == original)
		children[0] = node;
	else if (children[1] == original)
		children[1] = node;
}

void STreeNode::UpdateAABB(float margin)
{
	if (IsLeaf())
	{
		aabb.center = data->GetAABB()->center;
		aabb.halfwidth = data->GetAABB()->halfwidth + Vec2(margin, margin);
	}
	else
		aabb = children[0]->aabb.Merge(children[1]->aabb);
}

STreeNode* STreeNode::GetSibling()
{
	return (parent->children[0] == this) ? parent->children[1] : parent->children[0];
}