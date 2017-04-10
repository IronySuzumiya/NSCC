#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "LexicalAnalyzer.h"

using namespace nscc;

#ifdef _MSC_VER
int wmain(int argc, wchar_t* args[])
#else
int main()
#endif
{
	ifstream_t file;
	file.open("TestFile.txt");
	auto code_file = LexicalAnalyzer::Tokenize(file);
	file.close();

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
