#include "Bitmap.h"
Bitmap::Bitmap()
{

}
void Bitmap::InIt(HDC hdc, char* FileName)
{
	m_memDC = CreateCompatibleDC(hdc);
	m_myBitmap = (HBITMAP)LoadImageA(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	SelectObject(m_memDC, m_myBitmap);
	BITMAP Bitmap_Info;
	GetObject(m_myBitmap, sizeof(Bitmap_Info), &Bitmap_Info);

	ZeroMemory(&m_bf, sizeof(m_bf));
	m_bf.SourceConstantAlpha = 30; // 알파값 설정

	m_Size.cx = Bitmap_Info.bmWidth;
	m_Size.cy = Bitmap_Info.bmHeight;
}
void Bitmap::Draw(HDC hdc, int x, int y, int size_x, int size_y,bool isAlpha)
{
	if(isAlpha)
		AlphaBlend(hdc, x, y, size_x, size_y, m_memDC, 0, 0, 32, 32, m_bf);
	else
		TransparentBlt(hdc, x, y, size_x, size_y, m_memDC, 0, 0, m_Size.cx, m_Size.cy, RGB(255, 0, 255));
}
void Bitmap::Draw(HDC hdc, int x, int y)
{
	StretchBlt(hdc, x, y, 900, 700, m_memDC, 0, 0, m_Size.cx, m_Size.cy, SRCCOPY);
}
Bitmap::~Bitmap()
{
	DeleteObject(m_myBitmap);
	DeleteDC(m_memDC);
}