#ifndef PARSE_TREE_
#define PARSE_TREE_


#include <string>
#include <memory>
#include "token.h"

constexpr auto MAX_CHILDREN = 4;

// these are scoped enums, so no suffix needed
enum class NodeKind
{
	 STMT, EXPR, PARAM
};

enum class StmtKind
{
	DEF, DECL, CMPD, IF, WHILE, RETURN, EXPR, ELIF, ELSE
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
	TBD, INT, NUM, STRING,
	
};

struct Rational
{
	int num, den; // numerator and denominator
};

struct TreeNode
{
	std::shared_ptr<TreeNode> children[MAX_CHILDREN];
	std::shared_ptr<TreeNode> lSibling;
	std::shared_ptr<TreeNode> rSibling;

	int lineNo;

	NodeKind nodeKind;
	union Kind { StmtKind stmt; ExprKind expr; ParamKind param; } kind;
	union Attr{
		union ExprAttr{
			TokenType op;
			Rational num;
			char* str; // these pointers are owned by this pointer, and their space will be deallocated.
			char* id;
		} exprAttr;
		struct {
			ExprType type;
			bool isAddr;
			char* name;
			size_t size;
		} dclAttr;
	} attr;
	ExprType type;
	bool isAddr;

	void* something;

	TreeNode();

	~TreeNode();

	friend std::ostream& operator<<(std::ostream& os, const TreeNode& t);
};



#endif