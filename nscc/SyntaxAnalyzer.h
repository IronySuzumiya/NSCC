#pragma once
#ifndef __SYNTAX_ANALYZER
#define __SYNTAX_ANALYZER
#include "Nscc.h"
#include "LexicalAnalyzer.h"

namespace nscc
{
	struct ASTNode
	{
		typedef shared_ptr<ASTNode> Child;
	};
	
	struct Token
	{
		CodeToken token;
	};

	struct Modifiers : virtual ASTNode
	{
		vector<Child> ids;
	};

	struct Type : virtual ASTNode
	{
		Child id;
	};

	struct Expression : virtual ASTNode
	{
		Child op;
	};

	struct BinaryExpression : virtual Expression
	{
		Child rhs;
	};

	struct UnaryExpression : virtual Expression
	{
		Child lhs;
		Child rhs;
	};
	
	struct VariableDeclaration : virtual ASTNode
	{
		Child modifiers;
		Child type;
		Child id;
		Child initialized;
	};

	struct FunctionDeclaration : virtual ASTNode
	{
		Child modifiers;
		Child type;
		Child id;
		Child initialized;
	};

	class SyntaxAnalyzer
	{
	public:
		ASTNode::Child Parse(CodeFile::Ptr codeFile);
	private:
		ASTNode::Child Eat_Declaration(CodeLine::List lines);
	};
}

#endif
