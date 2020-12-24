#include <stdexcept>
#include <memory>
#include <cstring>
#include <algorithm>
#include <type_traits>

#include "parser.h"

using std::make_shared;

static constexpr TokenType rela_op[] = { TokenType::LT, TokenType::LE, TokenType::GT, TokenType::GE, TokenType::NEQ, TokenType::EQ };

static constexpr TokenType add_min_op[] = { TokenType::PLUS, TokenType::MINUS };

static constexpr TokenType mul_div_op[] = { TokenType::MUL, TokenType::DIV, TokenType::MOD };

static constexpr TokenType or_op[] = { TokenType::OR };

static constexpr TokenType and_not_op[] = { TokenType::AND, TokenType::NOT };

template <typename ARR>
static inline bool inOpList(TokenType t, ARR&& l)
{
	constexpr auto s = std::extent_v<ARR>;
	return std::find(std::begin(l), std::end(l), t) != std::end(l);
}

template <typename ARR, typename func>
SharedTreeNode Parser::expr_op_lassoc(ARR&& tk, func next_stage)
{
	try
	{
		return next_stage();
	}
	catch (const std::invalid_argument&)
	{
		auto root = make_shared<TreeNode>();
		root->nodeKind = NodeKind::EXPR;
		root->kind.expr = ExprKind::OP;
		root->lineNo = pos->line;
		root->children[0] = expr_op_lassoc(tk, next_stage);
		if (!inOpList(pos->type, tk))
		{
			throw std::invalid_argument("Wrong operand");
		}
		root->attr.exprAttr.op = pos->type;
		pos++;
		root->children[1] = next_stage();
		return root;
	}
}


static constexpr ExprType toExprType(TokenType t) {
	if (t == TokenType::INT)
	{
		return ExprType::INT;
	}
	else if (t == TokenType::STR)
	{
		return ExprType::STRING;
	}
	else if (t == TokenType::NUM)
	{
		return ExprType::NUM;
	}
}
std::pair<TokenType, bool> Parser::interpret_type()
{
	if (pos->type != TokenType::INT && pos->type != TokenType::STR && pos->type != TokenType::NUM) {
		throw std::invalid_argument("Type Name expected");
	}
	std::pair<TokenType, bool> p;
	p.first = pos->type;
	p.second = false;
	pos++;
	if (pos->type == TokenType::LBR) {
		pos++;
		if (pos->type != TokenType::RBR) {
			throw std::invalid_argument("] expected");
		}
		pos++;
		p.second = true;
	}
	
	return p;
}

Parser::Parser(std::list<Token> tokenList) : content(std::move(tokenList)), pos(content.cbegin())
{
	
}

SharedTreeNode Parser::parse()
{
	pos = content.cbegin();
	return program();
}

SharedTreeNode Parser::program()
{
	auto root = statement_list();
	if (pos->type != TokenType::FEOF)
	{
		throw std::invalid_argument("Internal error: no EOF token found.");
	}
	return root;
}

SharedTreeNode Parser::statement_list()
{
	auto root = make_shared<TreeNode>();
	auto curStmtPos = &root->children[0]; // address of write position
	while (pos != content.cend() && pos->type != TokenType::FEOF)
	{
		*curStmtPos = statement();
		curStmtPos = &((*curStmtPos)->rSibling);
		pos++;
	}
	return root;
}

SharedTreeNode Parser::statement()
{
	auto nextPos = ++std::list<Token>::const_iterator(pos);
	switch (pos->type)
	{
	case TokenType::DEF:
		return def_stmt();
	case TokenType::IF:
		return if_stmt();
	case TokenType::WHILE:
		return while_stmt();
	case TokenType::RETURN:
		return return_stmt();
	case TokenType::INDENT:
		return compound_stmt();
	case TokenType::ID:
		if (nextPos != content.cend() && nextPos->type == TokenType::COLON)
		{
			return decl_stmt();
		}
		else
		{
			return expr_stmt();
		}
	default:
		return expr_stmt();
	}
}

SharedTreeNode Parser::def_stmt()
{
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::DEF;
	root->lineNo = pos->line;
	if (pos->type != TokenType::DEF)
	{
		throw std::invalid_argument("DEF expected");
	}
	pos++;
	if (pos->type != TokenType::ID)
	{
		throw std::invalid_argument("identifier expected");
	}
	char* id = new char[pos->str.size() + 1];
	strcpy(id, pos->str.c_str());
	root->attr.dclAttr.name = id;
	pos++;
	if (pos->type != TokenType::LPR)
	{
		throw std::invalid_argument("( expected");
	}
	pos++; 
	root->children[0] = param_list();
	if (pos->type != TokenType::RPR)
	{
		throw std::invalid_argument(") expected");
	}
	pos++;
	if (pos->type == TokenType::ARROW)
	{
		pos++;
		auto [type, isarray] = interpret_type();
		root->attr.dclAttr.type = toExprType(type);
		root->attr.dclAttr.isAddr = isarray;
	}
	else {
		root->attr.dclAttr.type = ExprType::TBD;
	}
	if (pos->type != TokenType::COLON)
	{
		throw std::invalid_argument("COLON expected");
	}
	pos++;
	if (pos->type != TokenType::NEWLINE)
	{
		throw std::invalid_argument("NEWLINE expected");
	}
	pos++;
	root->children[1] = compound_stmt();
	return root;
}

SharedTreeNode Parser::compound_stmt()
{
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::CMPD;
	root->lineNo = pos->line;
	if (pos->type != TokenType::INDENT) 
	{
		throw std::invalid_argument("INDENT expected");
	}
	pos++;
	root->children[0] = statement_list();
	if (pos->type != TokenType::DEDENT)
	{
		throw std::invalid_argument("DEDENT expected");
	}
	pos++;
	return root;
}

SharedTreeNode Parser::while_stmt() {
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::WHILE;
	root->lineNo = pos->line;
	if (pos->type != TokenType::WHILE)
	{
		throw std::invalid_argument("while expected");
	}
	pos++;
	root->children[0] = expression();
	if (pos->type != TokenType::COLON)
	{
		throw std::invalid_argument("COLON expected");
	}
	pos++;
	if (pos->type != TokenType::NEWLINE)
	{
		throw std::invalid_argument("NEWLINE expected");
	}
	pos++;
	root->children[1] = compound_stmt();
	return root;
}
SharedTreeNode Parser::expr_stmt() {
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::EXPR;
	root->lineNo = pos->line;
	root->children[0] = expression();
	if (pos->type != TokenType::NEWLINE)
	{
		throw std::invalid_argument("NEWLINE expected");
	}
	pos++;
	return root;
}

SharedTreeNode Parser::if_stmt() {
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::IF;
	root->lineNo = pos->line;
	
	root->children[0] = if_clause();
	root->children[1] = elif_clause_list();
	root->children[2] = else_clause();

	return root;
}

SharedTreeNode Parser::if_clause() {
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::IF;
	root->lineNo = pos->line;

	if (pos->type != TokenType::IF) {
		throw std::invalid_argument("IF expected");
	}
	pos++;
	root->children[0] = expression();
	if (pos->type != TokenType::COLON) {
		throw std::invalid_argument("COLON expected");
	}
	pos++;
	if (pos->type != TokenType::NEWLINE) {
		throw std::invalid_argument("NEWLINE expected");
	}
	pos++;
	root->children[1] = compound_stmt();
	return root;
}

SharedTreeNode Parser::elif_clause_list() {
	if (pos->type != TokenType::ELIF) {
		return nullptr;
	}
		
	auto root = make_shared<TreeNode>();
	auto curStmtPos = &root->children[0];
	while (pos != content.cend() && pos->type != TokenType::FEOF) {
		*curStmtPos = elif_clause();
		curStmtPos = &((*curStmtPos)->lSibling);
		pos++;
	}

	
}
SharedTreeNode Parser::elif_clause()
{
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::ELIF;
	root->lineNo = pos->line;
	if (pos->type != TokenType::ELIF) {
		throw std::invalid_argument("ELIF expected");
	}
	pos++;
	root->children[0] = expression();
	if (pos->type != TokenType::COLON) {
		throw std::invalid_argument("COLON expected");
	}
	pos++;
	if (pos->type != TokenType::NEWLINE) {
		throw std::invalid_argument("NEWLINE expected");
	}
	pos++;
	root->children[1] = compound_stmt();
	return root;
}

SharedTreeNode Parser::else_clause()
{
	if (pos->type != TokenType::ELSE) {
		return nullptr;
	}

	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::ELSE;
	root->lineNo = pos->line;

	if (pos->type != TokenType::COLON) {
		throw std::invalid_argument("COLON expected");
	}
	pos++;
	if (pos->type != TokenType::NEWLINE) {
		throw std::invalid_argument("NEWLINE expected");
	}
	pos++;
	root->children[0] = compound_stmt();


	return root;
}

SharedTreeNode Parser::return_stmt() {
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::RETURN;
	root->lineNo = pos->line;
	if (pos->type != TokenType::RETURN)
	{
		throw std::invalid_argument("return expected");
	}
	pos++;
	if (pos->type != TokenType::NEWLINE)
	{
		root->children[0] = expression();
	}
	if (pos->type != TokenType::NEWLINE)
	{
		throw std::invalid_argument("NEWLINE expected");
	}
	pos++;
	return root;
}
SharedTreeNode Parser::decl_stmt() {
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::STMT;
	root->kind.stmt = StmtKind::DECL;
	root->lineNo = pos->line;
	if (pos->type != TokenType::ID)
	{
		throw std::invalid_argument("identifier expected");
	}
	char* id = new char[pos->str.size() + 1];
	strcpy(id, pos->str.c_str());
	root->attr.dclAttr.name = id;
	pos++;
	if (pos->type != TokenType::COLON)
	{
		throw std::invalid_argument("COLON expected");
	}
	pos++;
	if (pos->type != TokenType::INT && pos->type != TokenType::STR && pos->type != TokenType::NUM) {
		throw std::invalid_argument("type specifier expected");
	}
	else
	{
		root->attr.dclAttr.type = toExprType(pos->type);
	}
	pos++;
	if (pos->type != TokenType::NEWLINE) {
		throw std::invalid_argument("NEWLINE expected");
	}
	pos++;
	return root;
}
SharedTreeNode Parser::param_list()
{
	auto root = make_shared<TreeNode>();
	auto curParamPos = &root->children[0]; // address of write position
	while (pos != content.cend() && pos->type != TokenType::FEOF && pos->type!=TokenType::COMMA)
	{
		*curParamPos = param_list();
		curParamPos = &((*curParamPos)->rSibling);
		pos++;
	}
	if (pos->type == TokenType::COMMA || pos==content.cbegin()) {
		pos++;
		root->children[1] = expression();
	}
	return root;
}
SharedTreeNode Parser::arg_list()
{
	auto root = make_shared<TreeNode>();
	auto curArgPos = &root->children[0]; // address of write position
	while (pos != content.cend() && pos->type != TokenType::FEOF && pos->type != TokenType::COMMA)
	{
		*curArgPos = arg_list();
		curArgPos = &((*curArgPos)->rSibling);
		pos++;
	}
	if (pos->type == TokenType::COMMA || pos == content.cbegin()) {
		pos++;
		root->children[1] = expression();
	}
	return root;
}

SharedTreeNode Parser::or_expr()
{
	
	return expr_op_lassoc(or_op,[this] {return and_not_expr(); });
}

SharedTreeNode Parser::and_not_expr()
{
	return expr_op_lassoc(and_not_op, [this] {return not_expr(); });
}

SharedTreeNode Parser::not_expr()
{
	if (pos->type != TokenType::NOT)
	{
		return relational_expr();
	}
	pos++;
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::EXPR;
	root->kind.expr = ExprKind::OP;
	root->attr.exprAttr.op = TokenType::NOT;
	root->lineNo = pos->line;
	root->children[0] = not_expr();
	return root;
}

SharedTreeNode Parser::relational_expr()
{
	return expr_op_lassoc(rela_op, [this] {return add_min_expr(); });
}

SharedTreeNode Parser::add_min_expr()
{
	return expr_op_lassoc(add_min_op, [this] {return mul_div_expr(); });
}

SharedTreeNode Parser::mul_div_expr()
{
	return expr_op_lassoc(mul_div_op, [this] {return sign_expr(); });
}

SharedTreeNode Parser::sign_expr()
{
	if (!inOpList(pos->type, add_min_op))
	{
		return factor();
	}
	
	auto root = make_shared<TreeNode>();
	root->nodeKind = NodeKind::EXPR;
	root->kind.expr = ExprKind::OP;
	root->attr.exprAttr.op = pos->type;
	root->lineNo = pos->line;
	pos++;
	root->children[0] = sign_expr();
	return root;
}