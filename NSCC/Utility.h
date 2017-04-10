#pragma once
#ifndef __UTILITY
#define __UTILITY

#include "Nscc.h"

namespace nscc
{
	bool is_letter(const char_t c);
	bool is_number(const char_t c);
	bool is_hex_number(const char_t c);
}

#endif