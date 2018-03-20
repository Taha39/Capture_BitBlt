#ifndef __MONITOR_POPULATOR__
#define __MONITOR_POPULATOR__

#include <Windows.h>

namespace util
{
	class monitor_populator
	{
	public:
		monitor_populator();
		RECT get_primary_rect() const;
		RECT get_secondary_rect() const;
		bool is_secondary_exist() const;
	private:
		RECT primary_rect_{};
		RECT secondary_rect_{};
		bool is_secondary_exist_{ false };
	};
}
#endif