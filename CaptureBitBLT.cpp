
#include "CaptureBitBLT.h"
#include <iostream>
#include <cassert>

CaptureBitBLt::CaptureBitBLt(/*int width, int height, int left, int top, */RECT rect) :rect_{rect}
{	
	width_ = rect_.right - rect_.left;//width;
	height_ = rect_.bottom - rect_.top;//height;
	left_ = rect_.left;//left;
	top_ = rect_.top;//top;
}

CaptureBitBLt::~CaptureBitBLt()
{
	close();
}


bool CaptureBitBLt::init()
{
	HWND DesktopHwnd = GetDesktopWindow();
	dev_cap_ = GetDC(DesktopHwnd);
	if (dev_cap_ == NULL)
		return false;

	//DWORD FileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (sizeof(RGBTRIPLE) + 1 * (width_*height_ * 4));
	//char *BmpFileData = (char*)GlobalAlloc(0x0040, FileSize);
	//PBITMAPFILEHEADER BFileHeader = (PBITMAPFILEHEADER)BmpFileData;
	//PBITMAPINFOHEADER  BInfoHeader = (PBITMAPINFOHEADER)&BmpFileData[sizeof(BITMAPFILEHEADER)];

	//BFileHeader->bfType = 0x4D42; // BM
	//BFileHeader->bfSize = sizeof(BITMAPFILEHEADER);
	//BFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//BInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	//BInfoHeader->biPlanes = 1;
	//BInfoHeader->biBitCount = 32;
	//BInfoHeader->biCompression = BI_RGB;
	//BInfoHeader->biHeight = -height_;
	//BInfoHeader->biWidth = width_;

	//RGBTRIPLE *Image = (RGBTRIPLE*)&BmpFileData[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//RGBTRIPLE color;

	capture_dc_ = CreateCompatibleDC(dev_cap_);
	if (capture_dc_ == NULL)
		return false;

	capture_bitmap_ = CreateCompatibleBitmap(dev_cap_, width_, height_);
	if (capture_bitmap_ == NULL)
		return false;

	HGDIOBJ ret = SelectObject(capture_dc_, capture_bitmap_);
	if (ret == NULL || ret == HGDI_ERROR)
		return false;

	return true;
}

int CaptureBitBLt::capture_frame(UINT8* CaptureData)
{
	assert(dev_cap_);
	assert(capture_dc_);
	assert(capture_bitmap_);
	static int count = 1;
	count++;
	if (count % 80 == 0)
		DeleteObject(capture_dc_);

	LONG byteCopied{ 0 };
	int size = height_*width_ * 4;
	if (BitBlt(capture_dc_, 0, 0, width_, height_, dev_cap_, left_, top_, SRCCOPY))
	{
		byteCopied = GetBitmapBits(capture_bitmap_, size, CaptureData);
	}
	else
	{
		/*DWORD errorMessageID = ::GetLastError();
		
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);
		std::cout << message.c_str();*/
		capture_bitmap_ = nullptr;
		capture_dc_ = nullptr;
		dev_cap_ = nullptr;
	}
	return ((byteCopied == size) ? 1 : 0);
}

void CaptureBitBLt::error_check()
{
	if ((!dev_cap_) || (!capture_dc_) || (!capture_bitmap_))
	{
		close();
		init();
	}
}

void CaptureBitBLt::close()
{
	//if (capture_dc_)
	DeleteObject(capture_dc_);

	//if (capture_bitmap_)
	DeleteObject(capture_bitmap_);

	//if (dev_cap_)
	ReleaseDC(NULL, dev_cap_);
}