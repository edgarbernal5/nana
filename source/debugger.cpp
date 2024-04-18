#include <nana/debugger.hpp>

namespace nana
{
	bool debugger::enabled_debug = false;

	bool debugger::is_enabled_print_debug()
	{
		return enabled_debug;
	}

	void debugger::enable_print_debug(bool newValue)
	{
		enabled_debug = newValue;
	}
}

