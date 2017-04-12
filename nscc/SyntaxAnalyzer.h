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
		Child lhs;
		Child rhs;
		BinaryExpression(Child lhs, CodeToken op, Child rhs)
		{
			this->lhs = lhs;
			this->op = op;
			this->rhs = rhs;
		}
	};

	struct UnaryExpression : virtual Expression
	{
		Child rhs;
		UnaryExpression(CodeToken op, Child rhs)
		{
			this->op = op;
			this->rhs = rhs;
		}
	};

	struct VariableItem : virtual ASTNode
	{
		CodeToken var;
		VariableItem(CodeToken var)
		{
			this->var = var;
		}
	};

	struct ValueItem : virtual ASTNode
	{
		CodeToken value;
		ValueItem(CodeToken value)
		{
			this->value = value;
		}
	};

	struct FunctionCall : virtual ASTNode
	{
		vector<Child> params;
		CodeToken id;
	};
	
	struct VariableDeclaration : virtual ASTNode
	{
		vector<CodeToken> modifiers;
		CodeToken type;
		CodeToken id;
		Child initialized;
	};

	struct FunctionParameter : virtual ASTNode
	{
		CodeToken type;
		CodeToken id;
		FunctionParameter(CodeToken id, CodeToken type)
		{
			this->id = id;
			this->type = type;
		}
	};

	struct FunctionDeclaration : virtual ASTNode
	{
		vector<CodeToken> modifiers;
		CodeToken type;
		CodeToken id;
		vector<Child> params;
	};

	struct FunctionImplementation : virtual ASTNode
	{
		vector<CodeToken> modifiers;
		CodeToken type;
		CodeToken id;
		vector<Child> params;
		Child body;
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
		ASTNode::Child eat_expression(vector<CodeToken>::const_iterator token, vector<CodeError> & errors, size_t level = 13);
	};
}

#endif
