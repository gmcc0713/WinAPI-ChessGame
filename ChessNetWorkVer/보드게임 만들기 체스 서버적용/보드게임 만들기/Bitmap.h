#pragma once
#pragma comment(lib, "msimg32.lib") 
#include <string>
#include <Windows.h>
class Bitmap
{
private:
	HDC m_memDC;
	HBITMAP m_myBitmap;
	SIZE m_Size;
	BLENDFUNCTION m_bf;
public:
	Bitmap();
	SIZE GetSize()
	{
		return m_Size;
	}
	void InIt(HDC hdc, char * FileName);
	void Draw(HDC hdc, int x, int y, int size_x, int size_y, bool isAlpha = false);
	void Draw(HDC hdc,int x, int y);
	~Bitmap();
};

