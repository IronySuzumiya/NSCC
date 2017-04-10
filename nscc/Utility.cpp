#include "Utility.h"

namespace nscc
{
	bool is_letter(const char_t c)
	{
		return (c >= T('a') && c <= T('z')) || (c >= T('A') && c <= T('Z'));
	}
	bool is_number(const char_t c)
	{
		return c >= T('0') && c <= T('9');
	}
	bool is_hex_number(const char_t c)
	{
		return is_number(c) || (c >= T('a') && c <= T('f') || (c >= T('A') && c <= T('F')));
	}
}
