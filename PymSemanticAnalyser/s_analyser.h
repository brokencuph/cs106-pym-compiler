#ifndef S_ANALYSER_H
#define S_ANALYSER_H

#include "parsetree.h"
#include "symbol_table.h"

class SemanticAnalyser
{
private:
	TreeNode* parseTree; // this will be modified by Analyser
	SharedSymbolTable symTable;

	SharedSymbolTable genSymbolTableTop();

	void genSymbolTable(TreeNode* tr, SharedSymbolTable st);

	SharedSymbolTable genSymbolTable();

	void reportError(int lineNo, const char* msg);
public:
	SemanticAnalyser(TreeNode*);

	SharedSymbolTable getSymbolTable();
};

#endif