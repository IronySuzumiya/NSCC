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
			auto state = TokenizationState::BEGIN;
			auto add_token = [&] (const char_t* start, const char_t* end, CodeTokenType type)
			{
				codeLine->tokens.push_back
					(CodeToken{ type, string_t(start, end), start - line_start });
			};
			auto add_error = [&](string_t message, const char_t* position)
			{
				codeLine->errors.push_back
					(CodeError{ message, position - line_start });
			};

			while (reader)
			{
				switch (state)
				{
				case BEGIN:
					switch (*reader)
					{
					case T('\''):
						state = TokenizationState::IN_CHAR;
						break;
					case T('"'):
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
							add_token(reader, reader + 1, CodeTokenType::DIV);
						}
						break;
					case T('-'):
						switch (reader[1])
						{
						case T('>'):
							++reader;
							add_token(reader, reader + 2, CodeTokenType::SEL_MEM_WITH_PTR);
							break;
						case T('-'):
							++reader;
							add_token(reader, reader + 2, CodeTokenType::DECREMENT);
							break;
						default:
							add_token(reader, reader + 1, CodeTokenType::SUB__NEGATIVE);
						}
						break;
					}
				}
			}
		}
	}
}
