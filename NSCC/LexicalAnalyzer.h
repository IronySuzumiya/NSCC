#pragma once
#ifndef __LEXCICAL_ANALYZER
#define __LEXCICAL_ANALYZER
#include "Nscc.h"

namespace nscc
{
	struct CodeFile
	{
		typedef std::shared_ptr<CodeFile> Ptr;
		typedef std::vector<Ptr> List;

		std::vector<CodeLine::Ptr> lines;
	};

	struct CodeLine
	{
		typedef std::shared_ptr<CodeLine> Ptr;
		typedef std::vector<Ptr> List;

		std::vector<CodeToken> tokens;
		std::vector<CodeError> errors;
	};

	struct CodeToken
	{
		CodeTokenType type = CodeTokenType::UNKNOWN;
		string_t value;
		string_t::size_type colno;
	};

	struct CodeError
	{
		string_t message;
		string_t::size_type colno;
	};

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
		INDEX,						// []
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
	};

	enum TokenizationState
	{
		BEGIN,
		IN_INTEGER,
		IN_FLOAT,
		IN_CHAR,
		IN_CHAR_ESCAPING,
		IN_STRING,
		IN_STRING_ESCAPING,
		IN_COMMENT,				// //
		IN_MULTILINE_COMMENT,	// /* */
		IN_IDENTIFIER,
		IN_BRACKET,				// ()
		IN_SQUARE_BRACKET,		// []
	};

	static class LexicalAnalyzer
	{
	public:
		CodeFile::Ptr Tokenize(istream_t & input);
		CodeFile::Ptr Tokenize(const string_t & input);
		CodeFile::Ptr Tokenize(const std::vector<string_t> & input);
	private:

	};
}

#endif
