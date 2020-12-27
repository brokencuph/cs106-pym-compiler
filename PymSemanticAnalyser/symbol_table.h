#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <list>
#include <unordered_map>
#include <memory>

#include "parsetree.h"

struct PymSymbol;

struct PymSymRef
{
	PymSymbol* parent;
	TreeNode* refNode;

	PymSymRef(PymSymbol* p, TreeNode* rN);
};

using PymSymRefList = std::list<PymSymRef>;

struct SymbolTable;

struct PymSymbol
{
	SymbolTable* parent;
	PymSymRefList refs;
	TreeNode* declNode;

	PymSymbol(SymbolTable* p, TreeNode* dN);

	template <class... Types>
	void emplace_ref(Types... args)
	{
		refs.emplace_back(this, std::forward(args)...);
	}
};

struct SymbolTable
{
private:
	static int internal_id;
public:
	const int id; // cannot be modified after constructed
	TreeNode* tableNode;

	// a tree-like structure
	std::shared_ptr<SymbolTable> lower, next;
	std::weak_ptr<SymbolTable> upper, prev;

	std::unordered_map<std::string, PymSymbol> hashTable;

	SymbolTable(TreeNode* t);

	SymbolTable(TreeNode* t, int id);

	template <class... Types>
	void emplace_symbol(const std::string& name, Types... args)
	{
		hashTable.emplace(name, PymSymbol(this, std::forward(args)...));
	}

	PymSymbol& lookup(const std::string& name);

	bool check_local(const std::string& name);
};

using SharedSymbolTable = std::shared_ptr<SymbolTable>;

#endif