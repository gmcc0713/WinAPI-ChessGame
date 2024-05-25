#include "BitmapManager.h"
BitmapManager* BitmapManager::m_myBitmapManager = NULL;
BitmapManager::BitmapManager()
{

}
void BitmapManager::InIt(HWND hWnd)
{
	char buf[256];
	HDC hdc = GetDC(hWnd);
	for (int i = (int)CHESS_INDEX::INDEX_BLACK_PAWN; i <= (int)CHESS_INDEX::INDEX_BLACK_KING; i++)	//검은색 체스 말
	{
		sprintf_s(buf, "RES///block_b_%d.bmp", i);
		m_arrBitmap[i].InIt(hdc, buf);
	}
	for (int i = (int)CHESS_INDEX::INDEX_WHITE_PAWN; i <= (int)CHESS_INDEX::INDEX_WHITE_KING; i++) //흰색 체스 말
	{
		sprintf_s(buf, "RES///block_w_%d.bmp",i- (int)CHESS_INDEX::INDEX_WHITE_PAWN);
		m_arrBitmap[i].InIt(hdc, buf);
	}
	for (int i = 0; i < 3; i++)			//배경 타일
	{
		sprintf_s(buf, "RES//tile_%d.bmp", i);
		m_arrBitmap[(int)CHESS_INDEX::INDEX_TILE1+i].InIt(hdc, buf);
	}
	for (int i = 0; i < 5; i++)			//배경
	{
		sprintf_s(buf, "RES//BG_%d.bmp", i);
		m_arrBitmap[(int)CHESS_INDEX::INDEX_BG_0+i].InIt(hdc, buf);
	}
	for (int i = 0; i < 6; i++)			//배경
	{
		sprintf_s(buf, "RES//btn_%d.bmp", i+1);
		m_arrBitmap[(int)CHESS_INDEX::INDEX_WHITE_BTN + i].InIt(hdc, buf);
	}
	sprintf_s(buf, "RES//MoveableTileMask.bmp");
	m_arrBitmap[(int)CHESS_INDEX::INDEX_MOVEABLETILE].InIt(hdc, buf);		//이동가능한 위치 표시 타일
	ReleaseDC(hWnd, hdc);

}
BitmapManager::~BitmapManager()
{
	m_myBitmapManager = NULL;
}