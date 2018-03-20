#include "monitor_populator.h"
#include <iostream>
#include <cassert>

namespace util {
	struct MonitorDetail
	{
		bool bPrimary{ false };
		UINT left{ 0 };
		UINT top{ 0 };
		UINT right{ 0 };
		UINT bottom{ 0 };
	};
	struct MonitorList
	{
		UINT8 count{ 0 };
		MonitorDetail list[2];	
	};

	BOOL CALLBACK MonitorEnumProcCallback(_In_  HMONITOR hMonitor, _In_  HDC DevC, _In_  LPRECT lprcMonitor, _In_  LPARAM dwData)
	{
		MonitorList *pList = (MonitorList*)dwData;
		if (!pList)
			return false;

		if (pList->count >= 2)
			return true;

		MONITORINFO  info;
		info.cbSize = sizeof(MONITORINFO);
		BOOL monitorInfo = GetMonitorInfo(hMonitor, &info);

		UINT8 i = pList->count;
		if (info.dwFlags == 1)
			pList->list[i].bPrimary = true;
		else
			pList->list[i].bPrimary = false;

		pList->list[i].left = info.rcMonitor.left;
		pList->list[i].top = info.rcMonitor.top;
		pList->list[i].right = info.rcMonitor.right;
		pList->list[i].bottom = info.rcMonitor.bottom;

		pList->count++;
		return TRUE;
	}

	BOOL PopulateMonitorInfo(MonitorList* pList)
	{
		HDC m_DevC = GetDC(GetDesktopWindow());
		BOOL bStatus = EnumDisplayMonitors(m_DevC, NULL, MonitorEnumProcCallback, (LPARAM)pList);
		return bStatus;
	}


	monitor_populator::monitor_populator()
	{
		MonitorList monitor_list_;
		PopulateMonitorInfo(&monitor_list_);
		if (monitor_list_.count == 2)
			is_secondary_exist_ = true;

		for (int i = 0; i < monitor_list_.count; i++)
		{
			if (monitor_list_.list[i].bPrimary)
			{
				primary_rect_.left = monitor_list_.list[i].left;
				primary_rect_.right = monitor_list_.list[i].right;
				primary_rect_.top = monitor_list_.list[i].top;
				primary_rect_.bottom = monitor_list_.list[i].bottom;

			}
			else
			{
				secondary_rect_.left = monitor_list_.list[i].left;
				secondary_rect_.right = monitor_list_.list[i].right;
				secondary_rect_.top = monitor_list_.list[i].top;
				secondary_rect_.bottom = monitor_list_.list[i].bottom;

			}
			/*printf("Display[%d]: Primary[%d] L,T[%d,%d] WxH[%dx%d]",
				i, monitor_list_.list[i].bPrimary, monitor_list_.list[i].left,
				monitor_list_.list[i].top,
				(monitor_list_.list[i].right - monitor_list_.list[i].left),
				(monitor_list_.list[i].bottom - monitor_list_.list[i].top));*/
		}
	}
	RECT monitor_populator::get_primary_rect() const
	{
		return primary_rect_;
	}
	RECT monitor_populator::get_secondary_rect() const
	{
		assert(is_secondary_exist());
		return secondary_rect_;
	}
	bool monitor_populator::is_secondary_exist() const
	{
		return is_secondary_exist_;
	}
}