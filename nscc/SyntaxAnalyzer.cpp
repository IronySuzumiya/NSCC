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
				if (token != codeFile->tokens.cend())
				{
					return *token++;
				}
			}
			else
			{
				add_error(error_message);
				return CodeToken{ UNKNOWN, T(""), token->rowno, token->colno };
			}
		};

		auto eat_function_params_declr = [&]()
		{
			vector<ASTNode::Child> params;
			eat_token(); // (
			auto type = eat_token(is_type(*token), T("need type here"));
			if (type.type == VOID_TYPE)
			{
				if (token->type == BRACKET_END)
				{
					eat_token();
					return params;
				}
				else
				{
					add_error(T("need ) here"));
					do
					{
						eat_token();
					} while (token->type != BRACKET_END && token != codeFile->tokens.cend());
					eat_token();
					return params;
				}
			}
			else
			{
				CodeToken id{ IDENTIFIER, T(""), token->rowno, token->colno };
				if (token->type == IDENTIFIER)
				{
					id = eat_token();
				}
				params.push_back(make_shared<FunctionParameter>(id, type));
				while (token->type == COMMA)
				{
					auto type = eat_token(is_type(*token), T("need type here"));
					if (type.type != VOID_TYPE)
					{
						add_error(T("invalid type here"));
						while (token->type != COMMA && token->type != BRACKET_END);
						{
							eat_token();
						}
					}
					else
					{
						CodeToken id{ IDENTIFIER, T(""), token->rowno, token->colno };
						if (token->type == IDENTIFIER)
						{
							id = eat_token();
						}
						params.push_back(make_shared<FunctionParameter>(id, type));
					}
				}
				eat_token(BRACKET_END, T("need ) here"));
				return params;
			}
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
					auto initialized = eat_expression(token, errors);
					eat_token(token->type == END_OF_SENTENCE, T("need ; here"));
					auto result = make_shared<VariableDeclaration>();
					result->id = id;
					result->modifiers = modifiers;
					result->type = type;
					result->initialized = initialized;
					return std::dynamic_pointer_cast<ASTNode, VariableDeclaration>(result);
				}
				else if (token->type == END_OF_SENTENCE)
				{
					++token;
					auto result = make_shared<VariableDeclaration>();
					result->id = id;
					result->modifiers = modifiers;
					result->type = type;
					return std::dynamic_pointer_cast<ASTNode, VariableDeclaration>(result);
				}
			}
			else if (token->type == BRACKET_BEGIN)
			{
				auto params = eat_function_params_declr();
				if (token->type == END_OF_SENTENCE)
				{
					auto result = make_shared<FunctionDeclaration>();
					result->modifiers = modifiers;
					result->id = id;
					result->type = type;
					result->params = params;
					return std::dynamic_pointer_cast<ASTNode, FunctionDeclaration>(result);
				}
				else if (token->type == BLOCK_BEGIN)
				{
					// Function Implementation
				}
			}
		};

		return ast;
	}

	ASTNode::Child SyntaxAnalyzer::eat_expression(vector<CodeToken>::const_iterator token, vector<CodeError> & errors, size_t level = 13)
	{
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

		auto eat_function_params = [&]()
		{
			vector<ASTNode::Child> params;
			eat_token(); // (
			if (token->type != BRACKET_END)
			{
				params.push_back(eat_expression(token, errors));
			}
			while (token->type == COMMA)
			{
				eat_token();
				params.push_back(eat_expression(token, errors));
			}
			eat_token(BRACKET_END, T("need ) here"));
			return params;
		};

		auto eat_primary = [&]()
		{
			if (token->type == IDENTIFIER)
			{
				auto id = eat_token();
				if (token->type == BRACKET_BEGIN)
				{
					auto params = eat_function_params();
					auto function_call = make_shared<FunctionCall>();
					function_call->id = id;
					function_call->params = params;
					return std::dynamic_pointer_cast<ASTNode, FunctionCall>(function_call);
				}
				else
				{
					return std::dynamic_pointer_cast<ASTNode, VariableItem>(make_shared<VariableItem>(id));
				}
			}
			else if (token->type == BRACKET_BEGIN)
			{
				eat_token();
				auto exp = eat_expression(token, errors);
				eat_token(token->type == BRACKET_END, T("need ) here"));
				return exp;
			}
			else if (is_value(*token))
			{
				return std::dynamic_pointer_cast<ASTNode, ValueItem>(make_shared<ValueItem>(eat_token()));
			}
			else
			{
				auto exp = make_shared<Expression>();
				exp->op = eat_token(false, T("need expression here"));
				return std::dynamic_pointer_cast<ASTNode, Expression>(exp);
			}
		};

		if (level > 2 && level < 14)
		{
			auto exp = eat_expression(token, errors, level - 1);
			while (is_binary_operator(*token, level))
			{
				auto op = eat_token();
				auto rhs = eat_expression(token, errors, level - 1);
				exp = make_shared<BinaryExpression>(exp, op, rhs);
			}
			return exp;
		}
		else if (level == 2)
		{
			if (is_unary_operator(*token, level))
			{
				auto op = eat_token();
				return std::dynamic_pointer_cast<ASTNode, UnaryExpression>(make_shared<UnaryExpression>(op, eat_expression(token, errors, level)));
			}
			else
			{
				return eat_expression(token, errors, level - 1);
			}
		}
		else if (level == 1)
		{
			if (is_unary_operator(*token, level))
			{
				auto op = eat_token();
				return std::dynamic_pointer_cast<ASTNode, UnaryExpression>(make_shared<UnaryExpression>(op, eat_expression(token, errors, level)));
			}
			else
			{
				return eat_primary();
			}
		}
		else
		{
			auto exp = make_shared<Expression>();
			exp->op = eat_token(false, T("unknown expression level"));
			return exp;
		}
	}
}
