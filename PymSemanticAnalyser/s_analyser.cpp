#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "s_analyser.h"

using namespace std::string_literals;

using std::make_shared;

void SemanticAnalyser::reportError(int lineNo, const char* msg)
{
	fprintf(stderr, "Line %d:\x001b[31mERROR\x001b[0m: %s\n", lineNo, msg);
}

void SemanticAnalyser::reportError(int lineNo, const std::string &msg)
{
	fprintf(stderr, "Line %d:\x001b[31mERROR\x001b[0m: %s\n", lineNo, msg.c_str());
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
	if (tr->nodeKind == NodeKind::STMT) // declarations
	{
		if (tr->kind.stmt == StmtKind::DECL)
		{
			if (st->check_local(tr->attr.dclAttr.name))
			{
				reportError(tr->lineNo, ("Redeclaration of symbol "s + tr->attr.dclAttr.name));
			}
			else 
			{
				st->emplace_symbol(std::string(tr->attr.dclAttr.name), tr);
			}
		}
		else if (tr->kind.stmt == StmtKind::DEF)
		{
			if (st->check_local(tr->attr.dclAttr.name))
			{
				reportError(tr->lineNo, "Redeclaration of function "s + tr->attr.dclAttr.name);
				return;
			}
			st->emplace_symbol(std::string(tr->attr.dclAttr.name), tr);
			auto lowerTable = make_shared<SymbolTable>(tr);
			genSymbolTable(tr->children[0].get(), lowerTable);
			genSymbolTable(tr->children[1].get(), lowerTable);
			lowerTable->upper = st;
			*st->nextAttachPoint = lowerTable;
			st->nextAttachPoint = &lowerTable->next;
			if (tr->rSibling) 
			{
				genSymbolTable(tr->rSibling.get(),st);
			}
			return;
		}
	}
	else if (tr->nodeKind == NodeKind::PARAM)
	{
		if (st->check_local(tr->attr.dclAttr.name))
		{
			reportError(tr->lineNo, ("Redeclaration of symbol "s + tr->attr.dclAttr.name));
		}
		else
		{
			st->emplace_symbol(std::string(tr->attr.dclAttr.name), tr);
		}
	}
	else if (tr->nodeKind == NodeKind::EXPR)
	{
		if (tr->kind.expr == ExprKind::OP)
		{
			if (tr->attr.exprAttr.op == TokenType::ASSI)
			{
				if (tr->children[0]->kind.expr == ExprKind::ID )
				{
					if (!st->check_local(tr->children[0]->attr.exprAttr.id))
					{
						st->emplace_symbol(std::string(tr->children[0]->attr.exprAttr.id),tr);
					}
				}
				else if(tr->children[0]->kind.expr == ExprKind::ARRAY)
				{
					try
					{
						auto sym = st->lookup(tr->children[0]->attr.exprAttr.id);
						sym.emplace_ref(tr->children[0].get());

					}
					catch (const std::invalid_argument& e) {
						reportError(tr->lineNo, e.what());
					}
					
				}
			}
		}
		else if (tr->kind.expr == ExprKind::ID || tr->kind.expr == ExprKind::ARRAY || tr->kind.expr == ExprKind::CALL)
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
				if (tr->kind.expr == ExprKind::ID && isArray)
				{
					tr->type = ExprType::ARRAY;
				}
				else
				{
					tr->type = sym.declNode->attr.dclAttr.type;
				}
				tr->something = sym.declNode; // record the declaration node of this access, for post-order use
				sym.emplace_ref(tr);
			}
			catch (const std::invalid_argument& e)
			{
				reportError(tr->lineNo, e.what());
			}
		}

	}
	for (const auto& c : tr->children)
	{
		if (c)
		{
			genSymbolTable(c.get(), st);
		}
	}
	if (tr->rSibling)
	{
		genSymbolTable(tr->rSibling.get(), st);
	}
}

SharedSymbolTable SemanticAnalyser::genSymbolTable()
{
	auto top = genSymbolTableTop();
	genSymbolTable(parseTree, top);
	return top;
}

int SemanticAnalyser::assignTypes(TreeNode* tr)
{
	int res[MAX_CHILDREN];
	std::transform(tr->children, tr->children + MAX_CHILDREN, res, [this](const SharedTreeNode& x) {
		return x ? assignTypes(x.get()) : -2;
		});
	assignTypes(tr->rSibling.get());
	if (tr->nodeKind != NodeKind::EXPR)
	{
		return -1;
	}
	switch (tr->kind.expr)	
	{
	case ExprKind::NUM:
		if (tr->attr.exprAttr.num.den == 1)
		{
			return int(tr->type = ExprType::INT);
		}
		else
			return int(tr->type = ExprType::NUM);
	case ExprKind::STR:
		return int(tr->type = ExprType::STRING);
	case ExprKind::ARRAYC:
		return int(tr->type = ExprType::ARRAY);	
	case ExprKind::ARRAY:
		if (res[0] != int(ExprType::INT) && res[0] != int(ExprType::TBD)) 
		{
			reportError(tr->lineNo, "Array subscript should be integer.");	
		}
		return int(tr->type);
	case ExprKind::ID:
		return int(tr->type);
	case ExprKind::CALL:
		if (tr->something)
		{
			try
			{
				TreeNode* defTr = reinterpret_cast<TreeNode*>(tr->something);
				TreeNode* p_param = defTr->children[0]->children[0].get();
				TreeNode* p_arg = tr->children[0]->children[0].get();
				int cnt = 0;
				while (p_param && p_arg)
				{
					if (!(p_param->attr.dclAttr.isAddr && p_arg->type == ExprType::ARRAY
						|| !p_param->attr.dclAttr.isAddr && p_arg->type == p_param->attr.dclAttr.type))
					{
						reportError(tr->lineNo, "Argument type mismatch on position " + std::to_string(cnt) + ".");
					}
					cnt++;
					p_param++;
					p_arg++;
				}
				if (p_param && !p_arg || !p_param && p_arg)
				{
					reportError(tr->lineNo, "Wrong argument number.");
				}
			}
			catch (const std::exception& e)
			{
				reportError(tr->lineNo, "Internal parse tree error.");
			}
		}
		return int(tr->type);
	case ExprKind::OP:
		if (res[0] == int(ExprType::TBD) || res[1] == int(ExprType::TBD)) {
			return int(tr->type = ExprType::TBD);
		}
		switch (tr->attr.exprAttr.op)
		{
		case TokenType::MUL:
			if (res[0] == int(ExprType::INT) && res[1] == int(ExprType::INT))
			{
				return int(tr->type = ExprType::INT);
			}
			else if (res[0] == int(ExprType::INT) && res[1] == int(ExprType::NUM))
			{
				return int(tr->type = ExprType::NUM);
			}
			else if (res[0] == int(ExprType::NUM) && res[1] == int(ExprType::INT))
			{
				return int(tr->type = ExprType::NUM);
			}
			else if (res[0] == int(ExprType::NUM) && res[1] == int(ExprType::NUM))
			{
				return int(tr->type = ExprType::NUM);
			}
			else
			{
				reportError(tr->lineNo, "Operand type mismatch.");
				return -1;
			}
		case TokenType::DIV:
			if (res[0] != int(ExprType::NUM) && res[0] != int(ExprType::INT) ||
				res[1] != int(ExprType::NUM) && res[1] != int(ExprType::INT))
			{
				reportError(tr->lineNo, "Operand type mismatch.");
				return -1;
			}
			else
			{
				return int(tr->type = ExprType::NUM);
			}
		case TokenType::MOD:
		case TokenType::AND:
		case TokenType::OR:
			if (res[0] != int(ExprType::INT) || res[1] != int(ExprType::INT))
			{
				reportError(tr->lineNo, "Operand type mismatch.");
				return -1;
			}
			else
			{
				return int(tr->type = ExprType::INT);
			}
		case TokenType::NOT:
			if (res[0] != int(ExprType::INT))
			{
				reportError(tr->lineNo, "Operand type mismatch.");
				return -1;
			}
			else
			{
				return int(tr->type = ExprType::INT);
			}
		case TokenType::PLUS:
			if (res[1] == -2)
			{
				if (res[0] == int(ExprType::INT) || res[0] == int(ExprType::NUM))
				{
					return int(tr->type = ExprType(res[0]));
				}
				else
				{
					reportError(tr->lineNo, "Operand type mismatch.");
					return -1;
				}
			}
			else
			{
				if (res[0] == int(ExprType::INT) && res[1] == int(ExprType::INT))
				{
					return int(tr->type = ExprType::INT);
				}
				else if (res[0] == int(ExprType::INT) && res[1] == int(ExprType::NUM))
				{
					return int(tr->type = ExprType::NUM);
				}
				else if (res[0] == int(ExprType::NUM) && res[1] == int(ExprType::INT))
				{
					return int(tr->type = ExprType::NUM);
				}
				else if (res[0] == int(ExprType::NUM) && res[1] == int(ExprType::NUM))
				{
					return int(tr->type = ExprType::NUM);
				}
				else if (res[0] == int(ExprType::STRING) && res[1] == int(ExprType::STRING))
				{
					return int(tr->type = ExprType::STRING);
				}
				else
				{
					reportError(tr->lineNo, "Operand type mismatch.");
					return -1;
				}
				
			}
		case TokenType::MINUS:
			if (res[1] == -2)
			{
				if (res[0] == int(ExprType::INT) || res[0] == int(ExprType::NUM))
				{
					return int(tr->type = ExprType(res[0]));
				}
				else
				{
					reportError(tr->lineNo, "Operand type mismatch.");
					return -1;
				}
			}
			else
			{
				if (res[0] == int(ExprType::INT) && res[1] == int(ExprType::INT))
				{
					return int(tr->type = ExprType::INT);
				}
				else if (res[0] == int(ExprType::INT) && res[1] == int(ExprType::NUM))
				{
					return int(tr->type = ExprType::NUM);
				}
				else if (res[0] == int(ExprType::NUM) && res[1] == int(ExprType::INT))
				{
					return int(tr->type = ExprType::NUM);
				}
				else if (res[0] == int(ExprType::NUM) && res[1] == int(ExprType::NUM))
				{
					return int(tr->type = ExprType::NUM);
				}
				else
				{
					reportError(tr->lineNo, "Operand type mismatch.");
					return -1;
				}

			}
		case TokenType::ASSI:
			if (res[0] == int(ExprType::INT) && res[1] == int(ExprType::INT)) {
				return int(tr->type = ExprType::INT);
			}
			else if (res[0] == int(ExprType::NUM) && res[1] == int(ExprType::NUM)) {
				return int(tr->type = ExprType::NUM);
			}
			else if (res[0] == int(ExprType::STRING) && res[1] == int(ExprType::STRING)) {
				return int(tr->type = ExprType::STRING);
			}
			else if (res[0] == int(ExprType::ARRAY) && res[1] == int(ExprType::ARRAY)) {
				return int(tr->type = ExprType::ARRAY);
			}
			else if (res[0] == int(ExprType::NUM) && res[1] == int(ExprType::INT)) {
				return int(tr->type = ExprType::NUM);
			}
			else if (res[0] == int(ExprType::ARRAY) && res[1] == int(ExprType::ARRAY))
			{
				return int(tr->type = ExprType::ARRAY);
			}
			else 
			{
				reportError(tr->lineNo, "Assignment type mismatch.");
				return -1;
			}
		default:
			break;
		}
	default:
		break;
	}
	return -1;
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

void SemanticAnalyser::assignTypes()
{
	assignTypes(parseTree);
}
