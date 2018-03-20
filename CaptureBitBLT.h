#ifndef CAPTURE_BLT_H
#define CAPTURE_BLT_H
#include <Windows.h>

class CaptureBitBLt
{
public:
	CaptureBitBLt(/*int width, int height, int left, int top, */RECT rect);
	~CaptureBitBLt();
	bool init();
	int capture_frame(UINT8* CaptureData);
	void error_check();
	
private:
	void close();

private:
	HDC capture_dc_{ nullptr };
	HDC dev_cap_{ nullptr };
	HBITMAP capture_bitmap_{ nullptr };
	RECT rect_{};
	int width_{ 0 };
	int height_{ 0 };
	int left_{ 0 };
	int top_{ 0 };
};
#endif