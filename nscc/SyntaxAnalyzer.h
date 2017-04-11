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

	struct Expression : virtual ASTNode
	{
		CodeToken op;
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
		vector<CodeToken> modifiers;
		CodeToken type;
		CodeToken id;
		Child initialized;
	};

	struct FunctionDeclaration : virtual ASTNode
	{
		vector<CodeToken> modifiers;
		CodeToken type;
		CodeToken id;
		Child initialized;
	};

	struct ParsingError
	{
		string_t message;
		ptrdiff_t rowno;
		ptrdiff_t tokenno;
	};

	class SyntaxAnalyzer
	{
	public:
		ASTNode::Child Parse(CodeFile::Ptr codeFile);
	private:
		ASTNode::Child Eat_Program(vector<CodeToken>::const_iterator token, vector<CodeError> errors);
		ASTNode::Child Eat_Declaration(vector<CodeToken>::const_iterator token, vector<CodeError> errors);
	};
}

#endif
