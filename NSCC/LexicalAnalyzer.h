#pragma once
#ifndef __LEXCICAL_ANALYZER
#define __LEXCICAL_ANALYZER
#include "Nscc.h"

namespace nscc
{
	enum CodeTokenType
	{
		UNKNOWN,
		INTEGER,
		FLOAT,
		CHAR,
		STRING,
		IDENTIFIER,
		SEL_MEM,					// .
		SEL_MEM_WITH_PTR,			// ->
		BRACKET_BEGIN,				// (
		BRACKET_END,				// )
		INDEX_BEGIN,				// [
		INDEX_END,					// ]
		LOGICAL_NOT,				// !
		ADD__POSITIVE,				// +
		SUB__NEGATIVE,				// -
		NOT,						// ~
		INCREMENT,					// ++
		DECREMENT,					// --
		MUL__DEREFERENCE,			// *
		AND__REFERENCE,				// &
		DIV,						// /
		MOD,						// %
		LEFT_SHIFT,					// <<
		RIGHT_SHIFT,				// >>
		LESS_THAN,					// <
		GREATER_THAN,				// >
		LESS_EQUAL,					// <=
		GREATER_EQUAL,				// >=
		EQUAL_TO,					// ==
		NOT_EQUAL_TO,				// !=
		XOR,						// ^
		OR,							// |
		LOGICAL_AND,				// &&
		LOGICAL_OR,					// ||
		ASSIGNMENT,					// =
		COMMA,						// ,
		JUDGEMENT,					// ?
		CONDITION_SEPARATOR,		// :
		END_OF_SENTENCE,			// ;
		BLOCK_BEGIN,				// {
		BLOCK_END,					// }
	};

	enum TokenizationState
	{
		BEGIN,
		IN_INTEGER,
		IN_FLOAT,
		IN_CHAR,
		IN_STRING,
		IN_IDENTIFIER,
		IN_COMMENT,				// //
		IN_MULTILINE_COMMENT,	// /* */
		IN_BRACKET,				// ()
		IN_SQUARE_BRACKET,		// []
		IN_BRACE,				// {}
	};

	struct CodeToken
	{
		CodeTokenType type;
		string_t value;
		ptrdiff_t colno;
	};

	struct CodeError
	{
		string_t message;
		ptrdiff_t colno;
	};

	struct CodeLine
	{
		typedef std::shared_ptr<CodeLine> Ptr;
		typedef std::vector<Ptr> List;

		std::vector<CodeToken> tokens;
		std::vector<CodeError> errors;
	};

	struct CodeFile
	{
		typedef std::shared_ptr<CodeFile> Ptr;
		typedef std::vector<Ptr> List;

		std::vector<CodeLine::Ptr> lines;
	};

	class LexicalAnalyzer
	{
	public:
		static CodeFile::Ptr Tokenize(istream_t & iss);
		static CodeFile::Ptr Tokenize(ifstream_t & ifs);
		static CodeFile::Ptr Tokenize(const string_t & input);
		static CodeFile::Ptr Tokenize(const std::vector<string_t> & lines);
	private:
		static CodeLine::Ptr Tokenize_Line(const string_t & line, std::stack<TokenizationState> & state_stack);
	};
}

#endif
