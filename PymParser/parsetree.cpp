#include <sstream>
#include <unordered_map>
#include "parsetree.h"
template<typename T>
using EToSMap = std::unordered_map<T, std::string>;

static std::string tokenNames[42] = { "ID","NUMBER","CHARS", // Literals
	"PLUS","MINUS","MUL","DIV","MOD", // math operators
	"LPR","RPR","LBR","RBR", // delimiters
	"ASSI", // assignment
	"GT","LT","GE","LE","EQ","NEQ", // relational operators
	"INDENT","DEDENT", // tokens relevant to indentation
	"NEWLINE","FEOF", // controling tokens
	"ERROR", // error token
	"COMMA","COLON",//"," ;
	"IF","ELSE","ELIF","WHILE","DEF","RETURN","AND","OR","NOT","INT","NUM","STR",//keywords
	"ARROW" };
static std::string nodeKindNames[] = {
	"STMT","EXPR","PARAM","LIST"
};
static std::string stmtKindNames[] = {
	"DEF", "DECL", "CMPD","IFC", "IF", "WHILE", "RETURN", "EXPR", "ELIF", "ELSE"
};
static std::string exprKindNames[] = {
	"OP", "NUM", "STR", "ID", "CALL", "ARRAY","ARRAYC"
};
static std::string paramKindNames[] = {
	"DYN", "STC", "STC_ARR"
};
static std::string listKindNames[] = {
	"STMT", "PARAM", "ARG", "ELIF"
};
static std::string exprTypeNames[] = {
	"TBD", "INT", "NUM", "STRING"
};
Rational Rational::parse(const std::string& str)
{
	Rational ans;
	std::stringstream ss(str);
	ss >> ans.num;
	if (ss.peek() == '\\')
	{
		ss.get();
		ss >> ans.den;
	}
	else
	{
		ans.den = 1;
	}
	return ans;
}

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

std::ostream& operator<<(std::ostream& os, const Rational& r)
{
	return os << r.num << "\\" << r.den;
}

std::ostream& operator<<(std::ostream& os, const TreeNode& t)
{
	static int spaceNum = 0;
	os << std::string(spaceNum, ' ');
	os << t.lineNo << ": "<<nodeKindNames[(int)t.nodeKind]<<" ";
	switch (t.nodeKind)
	{
	case NodeKind::EXPR:
		os << exprKindNames[(int)t.kind.expr]<<" ";
		switch (t.kind.expr) {
		case ExprKind::OP:
			os << tokenNames[(int)t.attr.exprAttr.op]<<" ";
			break;
		case ExprKind::NUM:
			os << t.attr.exprAttr.num << " ";
			break;
		case ExprKind::ARRAY:
			os << t.attr.exprAttr.id << "[exp] ";
			break;
		case ExprKind::ID:
			os << t.attr.exprAttr.id << " ";
			break;
		case ExprKind::CALL:
			os << t.attr.exprAttr.id << "(params) ";
			break;
		case ExprKind::STR:
			os << t.attr.exprAttr.str << " ";
			break;
		default:
			break;
		}
		break;
	case NodeKind::STMT	:
		os << stmtKindNames[(int)t.kind.stmt] << " ";
		switch (t.kind.stmt)
		{
		case StmtKind::DEF:
		case StmtKind::DECL:
			os << t.attr.dclAttr.name << " " << exprTypeNames[(int)t.attr.dclAttr.type] << " " 
				<< t.attr.dclAttr.isAddr << " " << t.attr.dclAttr.size << " ";
			break;
		default:
			break;
		}
		break;
	case NodeKind::LIST:
		os << listKindNames[(int)t.kind.list] << " ";
		break;
	case NodeKind::PARAM:
		os << paramKindNames[(int)t.kind.param] << " ";
		os << t.attr.dclAttr.name << " " << exprTypeNames[(int)t.attr.dclAttr.type] << " "
			<< t.attr.dclAttr.isAddr << " " << t.attr.dclAttr.size << " ";
		break;
	default:
		break;
	}
	spaceNum += 2;
	os << std::endl;
	for (auto& x : t.children) 
	{
		if (x) {
			os << *x;
		}
	}
	spaceNum -= 2;
	if (t.rSibling) {
		os << *t.rSibling;
	}
	return os;
}