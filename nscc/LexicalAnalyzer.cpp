#include "LexicalAnalyzer.h"
#include "Utility.h"

using std::make_shared;

namespace nscc
{
	CodeFile::Ptr LexicalAnalyzer::Tokenize(istream_t & iss)
	{
		std::vector<string_t> lines;
		string_t line;
		while (std::getline(iss, line))
		{
			lines.push_back(line);
		}
		return Tokenize(lines);
	}

	CodeFile::Ptr LexicalAnalyzer::Tokenize(ifstream_t & ifs)
	{
		std::vector<string_t> lines;
		string_t line;
		while (std::getline(ifs, line))
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
		std::stack<TokenizationState> state_stack;
		state_stack.push(BEGIN);
		for (auto line : lines)
		{
			codeFile->lines.push_back(Tokenize_Line(line, state_stack));
		}
		return codeFile;
	}

	CodeLine::Ptr LexicalAnalyzer::Tokenize_Line(const string_t & line, std::stack<TokenizationState> & state_stack)
	{
		auto codeLine = make_shared<CodeLine>();
		auto line_start = line.c_str();
		auto reader = line_start;
		auto token_start = reader;
		auto add_token = [&](CodeTokenType type, string_t::size_type length)
		{
			auto token = string_t(token_start, length);
			if (type == IDENTIFIER)
			{
				type = token == T("if") ? IF :
					token == T("else") ? ELSE :
					token == T("switch") ? SWITCH :
					token == T("case") ? CASE :
					token == T("default") ? DEFAULT :
					token == T("break") ? BREAK :
					token == T("while") ? WHILE :
					token == T("for") ? FOR :
					token == T("do") ? DO :
					token == T("continue") ? CONTINUE :
					token == T("return") ? RETURN :
					token == T("typedef") ? TYPEDEF :
					token == T("static") ? STATIC :
					token == T("extern") ? EXTERN :
					token == T("auto") ? AUTO :
					token == T("const") ? CONST :
					token == T("volatile") ? VOLATILE :
					token == T("inline") ? INLINE :
					token == T("void") ? VOID_TYPE :
					token == T("char") ? CHAR_TYPE :
					token == T("unsigned") ? UNSIGNED :
					token == T("signed") ? SIGNED :
					token == T("int") ? INT_TYPE :
					token == T("short") ? SHORT_TYPE :
					token == T("long") ? LONG_TYPE :
					token == T("float") ? FLOAT_TYPE :
					token == T("double") ? DOUBLE_TYPE :
					token == T("struct") ? STRUCT_TYPE :
					token == T("union") ? UNION_TYPE :
					token == T("enum") ? ENUM_TYPE :
					token == T("bool") ? BOOL_TYPE :
					IDENTIFIER;
			}
			codeLine->tokens.push_back
			(CodeToken{ type, token, token_start - line_start });
		};
		auto add_error = [&](string_t message)
		{
			codeLine->errors.push_back
			(CodeError{ message, reader - line_start });
		};

		while (true)
		{
			switch (state_stack.top())
			{
			case IN_BRACE:
				if (*reader == T('}'))
				{
					token_start = reader;
					add_token(BLOCK_END, 1);
					state_stack.pop();
					break;
				}
			case IN_BRACKET:
				if (*reader == T(')'))
				{
					token_start = reader;
					add_token(BRACKET_END, 1);
					state_stack.pop();
					break;
				}
			case IN_SQUARE_BRACKET:
				if (*reader == T(']'))
				{
					token_start = reader;
					add_token(INDEX_END, 1);
					state_stack.pop();
					break;
				}
			case BEGIN:
				token_start = reader;
				switch (*reader)
				{
				case T(';'):
					add_token(END_OF_SENTENCE, 1);
					break;
				case T('{'):
					add_token(BLOCK_BEGIN, 1);
					state_stack.push(IN_BRACE);
					break;
				case T('\''):
					++token_start;
					++reader;
					state_stack.push(IN_CHAR);
					break;
				case T('"'):
					++token_start;
					state_stack.push(IN_STRING);
					break;
				case T('('):
					add_token(BRACKET_BEGIN, 1);
					state_stack.push(IN_BRACKET);
					break;
				case T('['):
					add_token(INDEX_BEGIN, 1);
					state_stack.push(IN_SQUARE_BRACKET);
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
						state_stack.push(IN_COMMENT);
						break;
					case T('*'):
						++reader;
						state_stack.push(IN_MULTILINE_COMMENT);
						break;
					default:
						add_token(DIV, 1);
					}
					break;
				case T('-'):
					switch (reader[1])
					{
					case T('>'):
						add_token(SEL_MEM_WITH_PTR, 2);
						++reader;
						break;
					case T('-'):
						add_token(DECREMENT, 2);
						++reader;
						break;
					default:
						add_token(SUB__NEGATIVE, 1);
					}
					break;
				case T('+'):
					if (reader[1] == T('+'))
					{
						add_token(INCREMENT, 2);
						++reader;
					}
					else
					{
						add_token(ADD__POSITIVE, 1);
					}
					break;
				case T('<'):
					switch (reader[1])
					{
					case T('='):
						add_token(LESS_EQUAL, 2);
						++reader;
						break;
					case T('<'):
						add_token(LEFT_SHIFT, 2);
						++reader;
						break;
					default:
						add_token(LESS_THAN, 1);
					}
					break;
				case T('>'):
					switch (reader[1])
					{
					case T('='):
						add_token(GREATER_EQUAL, 2);
						++reader;
						break;
					case T('>'):
						add_token(RIGHT_SHIFT, 2);
						++reader;
						break;
					default:
						add_token(GREATER_THAN, 1);
					}
					break;
				case T('='):
					if (reader[1] == T('='))
					{
						add_token(EQUAL_TO, 2);
						++reader;
					}
					else
					{
						add_token(ASSIGNMENT, 1);
					}
					break;
				case T('!'):
					if (reader[1] == T('='))
					{
						add_token(NOT_EQUAL_TO, 2);
						++reader;
					}
					else
					{
						add_token(LOGICAL_NOT, 1);
					}
					break;
				case T('&'):
					if (reader[1] == T('&'))
					{
						add_token(LOGICAL_AND, 2);
						++reader;
					}
					else
					{
						add_token(AND__REFERENCE, 1);
					}
					break;
				case T('|'):
					if (reader[1] == T('|'))
					{
						add_token(LOGICAL_OR, 2);
						++reader;
					}
					else
					{
						add_token(OR, 1);
					}
					break;
				case T('.'):
					add_token(SEL_MEM, 1);
					break;
				case T('~'):
					add_token(NOT, 1);
					break;
				case T('*'):
					add_token(MUL__DEREFERENCE, 1);
					break;
				case T('%'):
					add_token(MOD, 1);
					break;
				case T('^'):
					add_token(XOR, 1);
					break;
				case T(','):
					add_token(COMMA, 1);
					break;
				case T('?'):
					add_token(JUDGEMENT, 1);
					break;
				case T(':'):
					add_token(CONDITION_SEPARATOR, 1);
					break;
				case T('\0'):
					break;
				default:
					if (is_number(*reader))
					{
						state_stack.push(IN_INTEGER);
					}
					else if (is_letter(*reader) || *reader == T('_'))
					{
						state_stack.push(IN_IDENTIFIER);
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
					state_stack.pop();
					state_stack.push(IN_FLOAT);
				}
				else if (is_number(*reader))
				{
				}
				else
				{
					add_token(INTEGER, reader - token_start);
					--reader;
					state_stack.pop();
				}
				break;
			case IN_FLOAT:
				if (is_number(*reader))
				{
				}
				else
				{
					add_token(FLOAT, reader - token_start);
					--reader;
					state_stack.pop();
				}
				break;
			case IN_CHAR:
				if (*token_start == T('\\') && *reader == T('\''))
				{
					if (reader - token_start > 2)
					{
						add_error(T("Char including more than one characters"));
					}
					add_token(CHAR, reader - token_start);
					state_stack.pop();
				}
				else if(*reader == T('\''))
				{
					if (reader - token_start > 1)
					{
						add_error(T("Char including more than one characters"));
					}
					add_token(CHAR, reader - token_start);
					state_stack.pop();
				}
				else if (*reader == T('\0'))
				{
					add_error(T("Expecting a ' to end the char"));
					state_stack.pop();
				}
				break;
			case IN_STRING:
				if (*reader == T('\\'))
				{
					++reader;
				}
				else if (*reader == T('"'))
				{
					add_token(STRING, reader - token_start);
					state_stack.pop();
				}
				else if (*reader == T('\0'))
				{
					add_error(T("Expecting a \" to end the string"));
					state_stack.pop();
				}
				break;
			case IN_IDENTIFIER:
				if (is_letter(*reader) || is_number(*reader) || *reader == T('_'))
				{
				}
				else
				{
					add_token(IDENTIFIER, reader - token_start);
					--reader;
					state_stack.pop();
				}
			case IN_COMMENT:
				break;
			case IN_MULTILINE_COMMENT:
				if (*reader == T('\0'))
				{
				}
				else if (reader[0] == T('*') && reader[1] == T('/'))
				{
					++reader;
					state_stack.pop();
				}
				break;
			}
			if (*reader == T('\0'))
			{
				break;
			}
			++reader;
		}
		auto final_state = state_stack.top();
		_ASSERT_NSCC(final_state == BEGIN || final_state == IN_BRACKET
			|| final_state == IN_SQUARE_BRACKET || final_state == IN_BRACE);
		_ASSERT_NSCC(codeLine->errors.size() == 0);
		return codeLine;
	}
}
