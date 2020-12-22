#ifndef PARSE_TREE_
#define PARSE_TREE_


#include <string>
#include "token.h"

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

struct Rational
{
	int num, den; // numerator and denominator
};

struct TreeNode
{
	TreeNode* children[MAX_CHILDREN];
	TreeNode* lSibling;
	TreeNode* rSibling;

	int lineNo;

	NodeKind nodeKind;
	union Kind { StmtKind stmt; ExprKind expr; ParamKind param; } kind;
	union Attr{
		union ExprAttr{
			TokenType op;
			Rational num;
			char* str;
			char* id;
		} exprAttr;
		struct {
			ExprType type;
			char* name;
			size_t size;
		} dclAttr;
	} attr;
	ExprType type;

	void* something;

	TreeNode();

	~TreeNode();

	friend std::ostream& operator<<(std::ostream& os, const TreeNode& t);
};



#endif