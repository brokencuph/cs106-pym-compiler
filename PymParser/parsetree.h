#ifndef PARSE_TREE_
#define PARSE_TREE_

constexpr auto MAX_CHILDREN = 4;

// these are scoped enums, so no suffix needed
enum class NodeKind
{
	 STMT, EXPR, PARAM
};

enum class StmtKind
{
	DEF, DECL, CMPD, IF, WHILE, RETURN, EXPR
};

enum class ExprKind
{
	OP, NUM, STR, ID, CALL, ARRAY
};

enum class ParamKind
{
	DYN, STC, STC_ARR
};

enum class ExprType
{
	INT, NUM, STRING, ADDR
};

struct TreeNode
{
	TreeNode* children[MAX_CHILDREN];
	TreeNode* lSibling;
	TreeNode* rSibling;

	int lineNo;

	NodeKind nodeKind;
	union { StmtKind stmt; ExprKind expr; ParamKind param; } kind;


	void* something;
};

#endif