#include <cstring>
#include <stdexcept>
#include "s_analyser.h"

using namespace std::string_literals;

using std::make_shared;

static inline void report_error(int lineNo, const char* msg)
{
	fprintf(stderr, "Line %d:\x001b[31mERROR\x001b[0m: %s", lineNo, msg);
}

static SharedTreeNode genParamListNode(const char* paramName, ExprType paramType)
{
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::LIST;
	root->kind.list = ListKind::PARAM;
	root->lineNo = 0;

	auto child = make_shared<TreeNode>();
	root->children[0] = child;
	child->nodeKind = NodeKind::PARAM;
	child->kind.param = ParamKind::STC;
	child->lineNo = 0;
	char* name = new char[strlen(paramName) + 1];
	strcpy(name, paramName);
	child->attr.dclAttr = { paramType, false, name, 0 };
	
	return root;
}

template <size_t pnum = 1u>
static SharedTreeNode genFuncNode(const char* funcName, const char* paramName, ExprType paramType, ExprType retType)
{
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::DEF;
	root->lineNo = 0;
	char* name = new char[strlen(funcName) + 1];
	strcpy(name, funcName);
	root->attr.dclAttr = { retType, false, name, 0 };
	if (pnum)
		root->children[0] = genParamListNode(paramName, paramType);

	return root;
}

SharedSymbolTable SemanticAnalyser::genSymbolTableTop()
{
	static const SharedTreeNode internalFuncs[] = {
		genFuncNode("printnum", "number", ExprType::NUM, ExprType::TBD),
		genFuncNode("printstr", "str", ExprType::STRING, ExprType::TBD),
		genFuncNode<0>("inputnum", "", ExprType::TBD, ExprType::TBD),
		genFuncNode<0>("inputstr", "", ExprType::TBD, ExprType::TBD),
		genFuncNode("str2num", "str", ExprType::STRING, ExprType::NUM),
		genFuncNode("num2str", "num", ExprType::NUM, ExprType::STRING),
		genFuncNode("sign", "num", ExprType::NUM, ExprType::STRING),
		genFuncNode("simple", "rational", ExprType::NUM, ExprType::NUM),
		genFuncNode("top", "rational", ExprType::NUM, ExprType::INT),
		genFuncNode("bot", "rational", ExprType::NUM, ExprType::INT),
		genFuncNode("int", "num", ExprType::NUM, ExprType::INT),
		genFuncNode("frac", "num", ExprType::NUM, ExprType::NUM)
	};
	auto topTable = make_shared<SymbolTable>(parseTree, 0);
	for (const auto& x : internalFuncs)
	{
		topTable->emplace_symbol(x->attr.dclAttr.name, x.get());
	}
	return topTable;
}

void SemanticAnalyser::genSymbolTable(TreeNode* tr, SharedSymbolTable st)
{
	auto nextAttachPoint = &st->lower;
	if (tr->nodeKind == NodeKind::STMT) // declarations
	{
		if (tr->kind.stmt == StmtKind::DECL)
		{
			if (st->check_local(tr->attr.dclAttr.name))
			{

			}
			st->emplace_symbol(std::string(tr->attr.dclAttr.name), tr);
		}
		else if (tr->kind.stmt == StmtKind::DEF)
		{
			auto lowerTable = make_shared<SymbolTable>(tr);
			genSymbolTable(tr->children[0].get(), lowerTable);
			genSymbolTable(tr->children[1].get(), lowerTable);
			lowerTable->upper = st;
			*nextAttachPoint = lowerTable;
			nextAttachPoint = &lowerTable->next;
			return;
		}
	}
	else if (tr->nodeKind == NodeKind::PARAM)
	{
		st->emplace_symbol(std::string(tr->attr.dclAttr.name), tr);
	}
	else if (tr->nodeKind == NodeKind::EXPR)
	{
		try
		{
			PymSymbol& sym = st->lookup(tr->attr.exprAttr.id);
			const auto isFunc = (sym.declNode->nodeKind == NodeKind::STMT 
				&& sym.declNode->kind.stmt == StmtKind::DEF);
			const auto isArray = (sym.declNode->attr.dclAttr.isAddr == true);
			if (tr->kind.expr == ExprKind::ID && isFunc)
			{
				throw std::invalid_argument("Function "s + tr->attr.exprAttr.id + "cannot be used directly."s);
			}
			else if (tr->kind.expr == ExprKind::ARRAY && !isArray)
			{
				throw std::invalid_argument(tr->attr.exprAttr.id + " is not an array."s);
			}
			else if (tr->kind.expr == ExprKind::CALL && !isFunc)
			{
				throw std::invalid_argument(tr->attr.exprAttr.id + " cannot be called."s);
			}
			tr->type = sym.declNode->attr.dclAttr.type;
			sym.emplace_ref(tr);
		}
		catch (const std::invalid_argument& e)
		{
			fprintf(stderr, "Line %d:\x001b[31mERROR\x001b[0m: %s", tr->lineNo, e.what());
		}

	}
}

SharedSymbolTable SemanticAnalyser::genSymbolTable()
{
	auto top = genSymbolTableTop();
	genSymbolTable(parseTree, top);
	return top;
}

SemanticAnalyser::SemanticAnalyser(TreeNode* t)
	: parseTree(t), symTable(nullptr)
{

}

SharedSymbolTable SemanticAnalyser::getSymbolTable()
{
	if (symTable == nullptr)
	{
		symTable = genSymbolTable();
	}
	return symTable;
}
