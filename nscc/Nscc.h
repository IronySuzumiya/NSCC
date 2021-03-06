#pragma once
#ifndef __NSCC
#define __NSCC

#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <functional>
#include <fstream>
#include <iostream>

namespace nscc
{
#ifdef _DEBUG
#define _DEBUG_NSCC
#endif

#ifdef _DEBUG_NSCC
#define _ASSERT_NSCC _ASSERT
#else
#define _ASSERT_NSCC(...)
#endif

#ifdef _UNICODE_NSCC
	typedef wchar_t char_t;
	typedef std::wstring string_t;
	typedef std::wifstream ifstream_t;
	typedef std::wofstream ofstream_t;
	typedef std::wistream istream_t;
	typedef std::wostream ostream_t;
	typedef std::wstringstream stringstream_t;

#define T(X) L ## X
#define strtol_t wcstol
#define strtod_t wcstod
#define strtof_t wcstof
#else
	typedef char char_t;
	typedef std::string string_t;
	typedef std::ifstream ifstream_t;
	typedef std::ofstream ofstream_t;
	typedef std::istream istream_t;
	typedef std::ostream ostream_t;
	typedef std::stringstream stringstream_t;

#define T(X) X
#define strtol_t strtol
#define strtod_t strtod
#define strtof_t strtof
#endif

	using std::shared_ptr;
	using std::make_shared;
	using std::vector;
	using std::stack;
}

#endif
