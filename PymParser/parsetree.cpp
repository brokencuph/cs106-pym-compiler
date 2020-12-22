#include "parsetree.h"

TreeNode::TreeNode() : lSibling(0), rSibling(0), children{ 0 }, something(0),
	lineNo(0), nodeKind(NodeKind::STMT)
{
	kind.stmt = StmtKind::DEF;
}

TreeNode::~TreeNode()
{
	for (int i = 0; i < MAX_CHILDREN; i++)
	{
		if (children[i])
			children[i]->~TreeNode();
	}
	rSibling->~TreeNode();
}

std::ostream& operator<<(std::ostream& os, const TreeNode& t)
{
	return os;
}