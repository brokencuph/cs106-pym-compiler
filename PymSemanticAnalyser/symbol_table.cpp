#include <stdexcept>
#include "symbol_table.h"

static std::string exprTypeNames[] = {
	"TBD", "INT", "NUM", "STRING","ARRAY"
};

PymSymRef::PymSymRef(PymSymbol* p, TreeNode* rN)
	: parent(p), refNode(rN)
{
}

PymSymbol::PymSymbol(SymbolTable* p, TreeNode* dN)
	: parent(p), declNode(dN)
{
}

int SymbolTable::internal_id = 0;

SymbolTable::SymbolTable(TreeNode* t)
	: tableNode(t), id(internal_id++), lower(0), next(0),nextAttachPoint(&lower)
{

}

SymbolTable::SymbolTable(TreeNode* t, int id)
	: tableNode(t), id(id), lower(0), next(0),nextAttachPoint(&lower)
{
	SymbolTable::internal_id = id + 1;
}

PymSymbol& SymbolTable::lookup(const std::string& name)
{
	auto it = hashTable.find(name);
	if (it == hashTable.end())
	{
		if (upper.expired())
		{
			throw std::invalid_argument("Symbol " + name + " not found!");
		}
		else
		{
			return std::shared_ptr<SymbolTable>(upper)->lookup(name);
		}
	}
	return it->second;
}

bool SymbolTable::check_local(const std::string& name)
{
	return hashTable.find(name) != hashTable.end();
}

void SymbolTable::print_table()
{
	static int flag = 0;
	if (!flag)
	{
		printf("%-6s%-20s%-20s%-5s%-9s\n", "Table", "Name", "Kind", "Dcl", "Ref");
		printf("%-6s%-20s%-20s%-5s%-9s\n", "ID", "", "", "line", "lines");
		printf("%-6s%-20s%-20s%-5s%-9s\n", "----", "----", "----", "----", "----");
	}
	std::string idField = std::to_string(id);
	for (const auto& [name, sym] : hashTable)
	{
		printf("%-6s", idField.c_str());
		printf("%-20s", name.c_str());
		TreeNode* tr = sym.declNode;
		if (tr->nodeKind == NodeKind::STMT)
		{
			if (tr->kind.stmt == StmtKind::DEF)
			{
				printf("%-20s", "FUNC");
			}
			else if (tr->kind.stmt == StmtKind::DECL)
			{
				if (tr->attr.dclAttr.isAddr)
				{
					printf("%-20s", 
						("DCL_ARRAY <" + 
							exprTypeNames[(int)tr->attr.dclAttr.type] 
							+ ">").c_str());
				}
				else
				{
					printf("%-20s",
						("DCL_VAR <" +
							exprTypeNames[(int)tr->attr.dclAttr.type]
							+ ">").c_str());
				}
			}
		}
		else if (tr->nodeKind == NodeKind::EXPR)
		{
			printf("%-20s", "DYN_VAR");
		}
		else if (tr->nodeKind == NodeKind::PARAM)
		{
			if (tr->attr.dclAttr.isAddr)
			{
				printf("%-20s",
					("PARAM_ARRAY <" +
						exprTypeNames[(int)tr->attr.dclAttr.type]
						+ ">").c_str());
			}
			else
			{
				printf("%-20s",
					("PARAM_VAR <" +
						exprTypeNames[(int)tr->attr.dclAttr.type]
						+ ">").c_str());
			}
		}
		else
		{
			printf("Wrong table record.\n");
			continue;
		}
		printf("%-5d\n", tr->lineNo);
		for (const auto& ref : sym.refs)
		{
			printf("%-51s%d\n", "", ref.refNode->lineNo);
		}
	}
	flag++;
	if (lower)
	{
		lower->print_table();
	}
	if (next)
	{
		next->print_table();
	}
	flag--;
}
