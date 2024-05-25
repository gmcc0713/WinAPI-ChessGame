#pragma once
#include"Mecro.h"
#include "Bitmap.h"
#include "Mecro.h"
#include <string>
class BitmapManager
{
private:
	Bitmap m_arrBitmap[28];
	static BitmapManager* m_myBitmapManager;
	BitmapManager();
public:
	~BitmapManager();
	static BitmapManager* Get_Instance()
	{
		if (m_myBitmapManager == NULL)
			m_myBitmapManager = new BitmapManager;
		return m_myBitmapManager;
	}
	Bitmap* GetImage(int i ) {return &m_arrBitmap[i];}
	void InIt(HWND hWnd);
};

