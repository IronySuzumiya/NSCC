#include "LexicalAnalyzer.h"
#include "Utility.h"

using std::make_shared;

namespace nscc
{
	CodeFile::Ptr LexicalAnalyzer::Tokenize(istream_t & is)
	{
		std::vector<string_t> lines;
		string_t line;
		while (std::getline(is, line))
		{
			lines.push_back(line);
		}
		return Tokenize(lines);
	}

	CodeFile::Ptr LexicalAnalyzer::Tokenize(const string_t & input)
	{
		ifstream_t is(input);
		std::vector<string_t> lines;
		string_t line;
		while (std::getline(is, line))
		{
			lines.push_back(line);
		}
		return Tokenize(lines);
	}

	CodeFile::Ptr LexicalAnalyzer::Tokenize(const std::vector<string_t>& lines)
	{
		auto codeFile = make_shared<CodeFile>();
		for (auto line : lines)
		{
			auto codeLine = make_shared<CodeLine>();
			auto line_start = line.c_str();
			auto reader = line_start;
			auto token_start = reader;
			auto state = TokenizationState::BEGIN;
			auto add_token = [&] (CodeTokenType type, string_t::size_type length)
			{
				codeLine->tokens.push_back
					(CodeToken{ type, string_t(token_start, length), token_start - line_start });
			};
			auto add_error = [&](string_t message)
			{
				codeLine->errors.push_back
					(CodeError{ message, reader - line_start });
			};

			while (reader)
			{
				switch (state)
				{
				case BEGIN:
					token_start = reader;
					switch (*reader)
					{
					case T('\''):
						++token_start;
						state = TokenizationState::IN_CHAR;
						break;
					case T('"'):
						++token_start;
						state = TokenizationState::IN_STRING;
						break;
					case T('('):
						state = TokenizationState::IN_BRACKET;
						break;
					case T('['):
						state = TokenizationState::IN_SQUARE_BRACKET;
						break;
					case T(' '):
					case T('\t'):
					case T('\r'):
						break;
					case T('/'):
						switch (reader[1])
						{
						case T('/'):
							++reader;
							state = TokenizationState::IN_COMMENT;
							break;
						case T('*'):
							++reader;
							state = TokenizationState::IN_MULTILINE_COMMENT;
							break;
						default:
							add_token(CodeTokenType::DIV, 1);
						}
						break;
					case T('-'):
						switch (reader[1])
						{
						case T('>'):
							add_token(CodeTokenType::SEL_MEM_WITH_PTR, 2);
							++reader;
							break;
						case T('-'):
							add_token(CodeTokenType::DECREMENT, 2);
							++reader;
							break;
						default:
							add_token(CodeTokenType::SUB__NEGATIVE, 1);
						}
						break;
					case T('+'):
						if (reader[1] == T('+'))
						{
							add_token(CodeTokenType::INCREMENT, 2);
							++reader;
						}
						else
						{
							add_token(CodeTokenType::ADD__POSITIVE, 1);
						}
						break;
					case T('<'):
						switch (reader[1])
						{
						case T('='):
							add_token(CodeTokenType::LESS_EQUAL, 2);
							++reader;
							break;
						case T('<'):
							add_token(CodeTokenType::LEFT_SHIFT, 2);
							++reader;
							break;
						default:
							add_token(CodeTokenType::LESS_THAN, 1);
						}
						break;
					case T('>'):
						switch (reader[1])
						{
						case T('='):
							add_token(CodeTokenType::GREATER_EQUAL, 2);
							++reader;
							break;
						case T('>'):
							add_token(CodeTokenType::RIGHT_SHIFT, 2);
							++reader;
							break;
						default:
							add_token(CodeTokenType::GREATER_THAN, 1);
						}
						break;
					case T('='):
						if (reader[1] == T('='))
						{
							add_token(CodeTokenType::EQUAL_TO, 2);
							++reader;
						}
						else
						{
							add_token(CodeTokenType::ASSIGNMENT, 1);
						}
						break;
					case T('!'):
						if (reader[1] == T('='))
						{
							add_token(CodeTokenType::NOT_EQUAL_TO, 2);
							++reader;
						}
						else
						{
							add_token(CodeTokenType::LOGICAL_NOT, 1);
						}
						break;
					case T('&'):
						if (reader[1] == T('&'))
						{
							add_token(CodeTokenType::LOGICAL_AND, 2);
							++reader;
						}
						else
						{
							add_token(CodeTokenType::AND__REFERENCE, 1);
						}
						break;
					case T('|'):
						if (reader[1] == T('|'))
						{
							add_token(CodeTokenType::LOGICAL_OR, 2);
							++reader;
						}
						else
						{
							add_token(CodeTokenType::OR, 1);
						}
						break;
					case T('.'):
						add_token(CodeTokenType::SEL_MEM, 1);
						break;
					case T('~'):
						add_token(CodeTokenType::NOT, 1);
						break;
					case T('*'):
						add_token(CodeTokenType::MUL__DEREFERENCE, 1);
						break;
					case T('%'):
						add_token(CodeTokenType::MOD, 1);
						break;
					case T('^'):
						add_token(CodeTokenType::XOR, 1);
						break;
					case T(','):
						add_token(CodeTokenType::COMMA, 1);
						break;
					default:
						if (is_number(*reader))
						{
							state = TokenizationState::IN_INTEGER;
						}
						else if (is_letter(*reader) || *reader == T('_'))
						{
							state = TokenizationState::IN_IDENTIFIER;
						}
						else
						{
							add_error(T("Found invalid symbol"));
						}
					}
					break;
				case IN_INTEGER:
					if (*reader == T('.'))
					{
						state = TokenizationState::IN_FLOAT;
					}
					else if (is_number(*reader))
					{
					}
					else
					{
						add_token(CodeTokenType::INTEGER, reader - token_start);
						--reader;
						state = TokenizationState::BEGIN;
					}
					break;
				case IN_FLOAT:
					if (is_number(*reader))
					{
					}
					else
					{
						add_token(CodeTokenType::FLOAT, reader - token_start);
						--reader;
						state = TokenizationState::BEGIN;
					}
					break;
				case IN_CHAR:
					if (*reader == T('\\'))
					{
						state = TokenizationState::IN_CHAR_ESCAPING;
					}
					else if(reader[1] == T('\''))
					{
						++reader;
						add_token(CodeTokenType::CHAR, 1);
						state = TokenizationState::BEGIN;
					}
					else
					{
						add_error(T("Type Char can only include one character"));
						state = TokenizationState::BEGIN;
					}
					break;
				case IN_CHAR_ESCAPING:
					if ((is_number(*reader) ||
						(*token_start == T('x') && is_hex_number(*reader))) &&
						reader - token_start < 3)
					{
					}
					else if(reader[1] == T('\''))
					{
						++reader;
						add_token(CodeTokenType::CHAR, reader - token_start);
						state = TokenizationState::BEGIN;
					}
					else
					{
						add_error(T("Type Char can only include one character"));
						state = TokenizationState::BEGIN;
					}

					// TODO

				}
			}
		}
	}
}
