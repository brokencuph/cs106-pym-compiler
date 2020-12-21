#ifndef PARSE_TREE_
#define PARSE_TREE_

constexpr auto MAX_CHILDREN = 4;

enum class NodeKind
{
	 STMT_ND, EXPR_ND, PARAM_ND
};

struct TreeNode
{
	TreeNode* children[MAX_CHILDREN];
	TreeNode* lSibling;
	TreeNode* rSibling;

	int lineNo;

	NodeKind nodeKind;



	void* something;
};

#endif