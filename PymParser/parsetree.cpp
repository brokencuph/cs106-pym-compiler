#include "parsetree.h"

TreeNode::TreeNode() : lSibling(0), rSibling(0), children{ 0 }, something(0),
	lineNo(0), nodeKind(NodeKind::STMT), type(ExprType::TBD)
{
	kind.stmt = StmtKind::DEF;
	attr.dclAttr = { ExprType::TBD, false, nullptr, 0 };
	//attr.dclAttr.type = ExprType::TBD;
	//attr.dclAttr.name = nullptr;
	//attr.dclAttr.isAddr = false;
	//attr.dclAttr.size = 0u;
}

// clear string in heap
TreeNode::~TreeNode()
{
	if (nodeKind == NodeKind::STMT)
	{
		if (kind.stmt == StmtKind::DECL || kind.stmt == StmtKind::DEF)
		{
			if (attr.dclAttr.name)
			{
				delete attr.dclAttr.name;
				attr.dclAttr.name = nullptr;
			}
		}
	}
	else if (nodeKind == NodeKind::PARAM)
	{
		if (attr.dclAttr.name)
		{
			delete attr.dclAttr.name;
			attr.dclAttr.name = nullptr;
		}
	}
	else if (nodeKind == NodeKind::EXPR)
	{
		switch (kind.expr)
		{
		case ExprKind::STR:
			if (attr.exprAttr.str)
			{
				delete attr.exprAttr.str;
				attr.exprAttr.str = nullptr;
			}
			break;
		case ExprKind::ID:
		case ExprKind::ARRAY:
		case ExprKind::CALL:
			if (attr.exprAttr.id)
			{
				delete attr.exprAttr.id;
				attr.exprAttr.id = nullptr;
			}
			break;
		}
	}
	if (something)
	{
		delete something;
		something = nullptr;
	}
}

std::ostream& operator<<(std::ostream& os, const TreeNode& t)
{
	return os;
}