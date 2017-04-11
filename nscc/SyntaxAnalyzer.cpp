#include "SyntaxAnalyzer.h"

namespace nscc
{
	ASTNode::Child nscc::SyntaxAnalyzer::Parse(CodeFile::Ptr codeFile)
	{
		auto ast = make_shared<ASTNode>();
		auto token = codeFile->tokens.cbegin();
		auto errors = codeFile->errors;
		auto add_error = [&](string_t message)
		{
			errors.push_back({ message, token->rowno, token->colno });
		};
		auto eat_token = [&](bool condition, string_t error_message = T(""))
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

		auto eat_expression13 = [&]()
		{
			auto lhs = eat_expression12();
			auto op = eat_token(is_binary_operator(*token));
			auto rhs = eat_expression12();
		};

		auto eat_expression = [&]()
		{
			return eat_expression13();
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
			ASTNode::Child initialized;
			if (token->type == ASSIGNMENT)
			{
				++token;
				initialized = eat_expression();
			}
			if (token->type == END_OF_SENTENCE) { token++; }
			else { add_error(T("need ; here")); }
		};

		return ast;
	}
}
