// BitBLTCapture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "monitor_populator.h"
#include "CaptureBitBLT.h"
#include <vector>

int main()
{
	util::monitor_populator ob;
	RECT screen_rect = ob.get_primary_rect();
	std::vector<UINT8> v;

	int width = screen_rect.right - screen_rect.left;
	int height = screen_rect.bottom - screen_rect.top;
	v.resize(width * height * 4);

	CaptureBitBLt cap(screen_rect);
	cap.init();

	FILE* fp;
	fopen_s(&fp, "Video.rgb", "wb");
	while (1)
	{
		if (cap.capture_frame(v.data()))
		{
			fwrite(v.data(), 1, v.size(), fp);
		}
		cap.error_check();
	}
	fclose(fp);
    return 0;
}

