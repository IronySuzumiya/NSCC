#include "Utility.h"

namespace nscc
{
	bool is_letter(const char_t c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	bool is_number(const char_t c)
	{
		return c >= '0' && c <= '9';
	}
}
