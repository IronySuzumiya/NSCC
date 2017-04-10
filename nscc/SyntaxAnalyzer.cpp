#include "SyntaxAnalyzer.h"

namespace nscc
{
	ASTNode::Child nscc::SyntaxAnalyzer::Parse(CodeFile::Ptr codeFile)
	{
		auto ast = make_shared<ASTNode>();
		stack<CodeTokenType> token_type_stack;
		for (auto line : codeFile->lines)
		{
			for (auto token : line->tokens)
			{
				switch (token.type)
				{
				case STATIC:
				case EXTERN:
				case AUTO:
				case CONST:
				case VOLATILE:
				case INLINE:

				}
			}
		}
		return ast;
	}
}
