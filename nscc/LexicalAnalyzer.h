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
		SIZE_OF,					// sizeof

		// Reserved words
		IF,							// if
		ELSE,						// else
		SWITCH,						// switch
		CASE,						// case
		DEFAULT,					// default
		BREAK,						// break
		WHILE,						// while
		FOR,						// for
		DO,							// do
		CONTINUE,					// continue
		RETURN,						// return
		TYPEDEF,					// typedef

		// Modifers
		STATIC,						// static
		EXTERN,						// extern
		AUTO,						// auto
		CONST,						// const
		VOLATILE,					// volatile
		INLINE,						// inline

		// Types
		VOID_TYPE,					// void
		CHAR_TYPE,					// char
		INT_TYPE,					// int
		SHORT_TYPE,					// short
		LONG_TYPE,					// long
		FLOAT_TYPE,					// float
		DOUBLE_TYPE,				// double
		STRUCT_TYPE,				// struct
		UNION_TYPE,					// union
		ENUM_TYPE,					// enum
		BOOL_TYPE,					// bool
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
		CodeTokenType type{ UNKNOWN };
		string_t value;
		ptrdiff_t rowno;
		ptrdiff_t colno;
	};

	struct CodeError
	{
		string_t message;
		ptrdiff_t rowno;
		ptrdiff_t colno;
	};

	struct CodeFile
	{
		typedef shared_ptr<CodeFile> Ptr;
		typedef vector<Ptr> List;

		vector<CodeToken> tokens;
		vector<CodeError> errors;
	};

	class LexicalAnalyzer
	{
	public:
		static CodeFile::Ptr Tokenize(istream_t & iss);
		static CodeFile::Ptr Tokenize(ifstream_t & ifs);
		static CodeFile::Ptr Tokenize(const string_t & input);
		static CodeFile::Ptr Tokenize(const vector<string_t> & lines);
	private:
		static void Tokenize_Line(CodeFile::Ptr codeFile, const string_t & line, ptrdiff_t rowno, stack<TokenizationState> & state_stack);
	};

	bool is_modifier(CodeToken token);
	bool is_type(CodeToken token);
	bool is_binary_operator(CodeToken token, size_t level = 13);
	bool is_unary_operator(CodeToken token, size_t level = 2);
}

#endif
