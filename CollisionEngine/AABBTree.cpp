#include "AABBTree.h"
#include "TreeNode.h"
#include "AABB.h"

#include <iostream>

CAABBTree::CAABBTree(float margin)
	: m_root(nullptr),	m_margin(margin)
{
}

CAABBTree::~CAABBTree()
{
	if (m_root)
	{
		delete m_root;
		m_root = nullptr;
	}
}

void CAABBTree::Update()
{
	if (!m_root)
		return;
	PrintTree();
	if (m_root->IsLeaf())
		m_root->UpdateAABB(m_margin);
	else
	{
		CheckNodeValidity(m_root);

		if (!m_invalidNodes.empty())
		{
			for (int idx = 0; idx < m_invalidNodes.size(); ++idx)
			{
				STreeNode* node = m_invalidNodes[idx];
				Extract(node);

				//reinsert node
				node->UpdateAABB(m_margin);
				Insert(node, m_root);
			}
			m_invalidNodes.clear();
		}
	}
}

void CAABBTree::Extract(STreeNode* node)
{
	STreeNode* parent = node->parent;
	STreeNode* grandParent = parent->parent;
	STreeNode* sibling = node->GetSibling();

	sibling->parent = grandParent;

	if (grandParent)
	{
		sibling->parent = grandParent;
		grandParent->ChangeChild(parent, sibling);
	}
	else
	{
		m_root = sibling;
		sibling->parent = nullptr;
	}

	parent->ClearChildren();
	delete parent;
}

void CAABBTree::CheckNodeValidity(STreeNode* node)
{
	if (node->IsLeaf())
	{
		if (!node->aabb.Contains(*node->data->GetAABB()))
			m_invalidNodes.push_back(node);
	}
	else
	{
		CheckNodeValidity(node->children[0]);
		CheckNodeValidity(node->children[1]);
	}

}

void CAABBTree::DrawNodeAABB(STreeNode* node)
{
	if (!node->IsLeaf())
	{
		if (node == m_root)
			node->aabb.Draw(1.f, 0.f, 0.f);
		else
			node->aabb.Draw(0.f, 0.f, 1.f);

		DrawNodeAABB(node->children[0]);
		DrawNodeAABB(node->children[1]);

		return;
	}

	node->data->GetAABB()->Draw(1.f, 0.f, 1.f);
	node->aabb.Draw();
}

void CAABBTree::AddPolygon(CPolygonPtr polygon)
{
	if (m_root)
	{
		STreeNode* node = new STreeNode();
		node->SetLeaf(polygon);
		node->UpdateAABB(m_margin);
		Insert(node, m_root);
	}
	else
	{
		m_root = new STreeNode();
		m_root->SetLeaf(polygon);
		m_root->UpdateAABB(m_margin);
	}
}

void CAABBTree::Insert(STreeNode* node, STreeNode* parent)
{
	if (parent->IsLeaf())
	{
		STreeNode* newParent = new STreeNode();

		newParent->parent = parent->parent;

		newParent->SetBranch(node, parent);

		if (newParent->parent)
			newParent->parent->ChangeChild(parent, newParent);

		if (parent == m_root)
			m_root = newParent;

		newParent->UpdateAABB(m_margin);
	}
	else
	{
		STreeNode* leftChild = parent->children[0];
		STreeNode* rightChild = parent->children[1];

		//Looking for area increase
		SAABB leftChildAABB = leftChild->aabb;
		SAABB rightChildAABB = rightChild->aabb;

		float leftCost = leftChildAABB.Merge(node->aabb).Area() - leftChildAABB.Area();
		float rightCost = rightChildAABB.Merge(node->aabb).Area() - rightChildAABB.Area();

		if (leftCost < rightCost)
			Insert(node, leftChild);
		else
			Insert(node, rightChild);
	}

	parent->UpdateAABB(m_margin);
}

void CAABBTree::GetCollidingPairsToCheck(std::vector<SPolygonPair>& pairsToCheck)
{
	if (!m_root || m_root->IsLeaf())
		return;

	ComputeCollidingPairsToCheck(m_root->children[0], m_root->children[1]);
	pairsToCheck = m_pairsList;
	m_pairsList.clear();

	ClearNodeChildrenCheckedFlag(m_root);
}

// 3 cases : 
//		- 2 leaves
//		- 1 leaf and 1 branch
//		- 2 branches
void CAABBTree::ComputeCollidingPairsToCheck(STreeNode* leftNode, STreeNode* rightNode)
{
	if (leftNode->IsLeaf())
	{
		if (rightNode->IsLeaf())	// 2 leaves case
		{
			if (leftNode->data->GetAABB()->Overlaps(*rightNode->data->GetAABB()))
				m_pairsList.push_back(SPolygonPair(leftNode->data, rightNode->data));
		}
		else                        // 1 leaf and 1 branch case
		{
			CheckNodeChildren(rightNode);
			ComputeCollidingPairsToCheck(leftNode, rightNode->children[0]);
			ComputeCollidingPairsToCheck(leftNode, rightNode->children[1]);
		}
	}
	else
	{
		if (rightNode->IsLeaf())	// 1 leaf and 1 branch case
		{
			CheckNodeChildren(leftNode);
			ComputeCollidingPairsToCheck(leftNode->children[0], rightNode);
			ComputeCollidingPairsToCheck(leftNode->children[1], rightNode);
		}
		else                        // 2 branches case
		{
			CheckNodeChildren(leftNode);
			CheckNodeChildren(rightNode);
			ComputeCollidingPairsToCheck(leftNode->children[0], rightNode->children[0]);
			ComputeCollidingPairsToCheck(leftNode->children[1], rightNode->children[0]);
			ComputeCollidingPairsToCheck(leftNode->children[0], rightNode->children[1]);
			ComputeCollidingPairsToCheck(leftNode->children[1], rightNode->children[1]);
		}
	}
}

void CAABBTree::CheckNodeChildren(STreeNode* node)
{
	if (!node->childrenChecked)
	{
		ComputeCollidingPairsToCheck(node->children[0], node->children[1]);
		node->childrenChecked = true;
	}
}

void CAABBTree::ClearNodeChildrenCheckedFlag(STreeNode* node)
{
	node->childrenChecked = false;
	if (!node->IsLeaf())
	{
		ClearNodeChildrenCheckedFlag(node->children[0]);
		ClearNodeChildrenCheckedFlag(node->children[1]);
	}
}

void CAABBTree::PrintTree()
{
	std::cout << "------------------" << std::endl;
	std::cout << "AABB TREE : " << std::endl;
	if (m_root)
		PrintNode(m_root);
	else
		std::cout << "No root" << std::endl;

	std::cout << "------------------" << std::endl;
}

void CAABBTree::PrintNode(STreeNode* node, std::string decal)
{
	if (!node->IsLeaf())
	{
		std::cout << decal.c_str() << "- Branch" << std::endl;
		decal += "   ";
		if (node->children[0])
			PrintNode(node->children[0], decal);
		if (node->children[1])
			PrintNode(node->children[1], decal);

		return;
	}

	std::cout << decal.c_str() << "- Leaf" << std::endl;
}