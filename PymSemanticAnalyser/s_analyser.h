#ifndef S_ANALYSER_H
#define S_ANALYSER_H

#include "parsetree.h"
#include "symbol_table.h"

class SemanticAnalyser
{
private:
	TreeNode* parseTree; // this will be modified by Analyser
	SharedSymbolTable symTable;

	bool _error = false;
	
	SharedSymbolTable genSymbolTableTop();

	void genSymbolTable(TreeNode* tr, SharedSymbolTable st);

	SharedSymbolTable genSymbolTable();
	int assignTypes(TreeNode *tr);

	void reportError(int lineNo, const char* msg);
	void reportError(int lineNo, const std::string& msg);
public:
	SemanticAnalyser(TreeNode*);

	SharedSymbolTable getSymbolTable();
	void assignTypes();

	bool error();

};

#endif