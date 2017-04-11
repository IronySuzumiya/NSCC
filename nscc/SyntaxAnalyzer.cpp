#include "SyntaxAnalyzer.h"

namespace nscc
{
	ASTNode::Child nscc::SyntaxAnalyzer::Parse(CodeFile::Ptr codeFile)
	{
		auto ast = make_shared<ASTNode>();
		auto token = codeFile->tokens.cbegin();
		auto errors = codeFile->errors;

		CodeToken variable_owner;

		auto add_error = [&](string_t message = T("error"))
		{
			errors.push_back({ message, token->rowno, token->colno });
		};

		auto eat_token = [&](bool condition = true, string_t error_message = T(""))
		{
			if (condition)
			{
				return *token++;
			}
			else
			{
				add_error(error_message);
				return CodeToken{ UNKNOWN, T(""), token->rowno, token->colno };
			}
		};

		auto eat_expression = std::function<ASTNode::Child(size_t)>();
		eat_expression = [&](size_t level)
		{
			if (level > 2 && level < 14)
			{
				auto exp = eat_expression(level - 1);
				while (is_binary_operator(*token, level))
				{
					auto op = eat_token();
					auto rhs = eat_expression(level - 1);
					exp = make_shared<BinaryExpression>(exp, op, rhs);
				}
				return exp;
			}
			else if(level == 2)
			{
				if (is_unary_operator(*token, level))
				{
					auto op = eat_token();
					return make_shared<UnaryExpression>(op, eat_expression(level - 1));
				}
				else
				{

				}
			}
			else if(level == 1)
			{

			}
			else
			{
				add_error(T("unknown expression level"));
			}
		};

		auto eat_expression = [&]()
		{
			return eat_expression(13);
		};

		auto eat_declaration = [&]()
		{
			vector<CodeToken> modifiers;
			while (is_modifier(*token))
			{
				modifiers.push_back(*token);
				++token;
			}
			auto type = eat_token(is_type(*token));
			auto id = eat_token(token->type == IDENTIFIER);
			if (token->type != BRACKET_BEGIN)
			{
				if (token->type == ASSIGNMENT)
				{
					++token;
					auto initialized = eat_expression();
					eat_token(token->type == END_OF_SENTENCE, T("need ; here"));
					auto result = make_shared<VariableDeclaration>();
					result->id = id;
					result->modifiers = modifiers;
					result->type = type;
					result->initialized = initialized;
				}
				else if (token->type == END_OF_SENTENCE)
				{
					++token;
					auto result = make_shared<VariableDeclaration>();
					result->id = id;
					result->modifiers = modifiers;
					result->type = type;
				}
				else if (token->type == BRACKET_BEGIN)
				{

				}
			}
		};

		return ast;
	}
}
