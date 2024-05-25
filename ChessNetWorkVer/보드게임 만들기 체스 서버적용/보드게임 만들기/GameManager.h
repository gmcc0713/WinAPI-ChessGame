#pragma once
#include <iostream>
#include<Windows.h>
#include <string>
#include "Mecro.h"
#include "ChessPieceList.h"
#include "ChessPiece.h"
#define GAME_LEFT_TIME 100

enum class TILE
{
	TILE_BACKGROUND1 = 0,
	TILE_BACKGROUND2,
	TILE_BACKGROUND3,

	TILE_MOVEABLE,
	TILE_COUNT,
};
enum class IMAGE_BG
{
	BG_GAMEMENU=0,
	BG_GAMEPLAYBG,
	BG_BLACKWIN,
	BG_WHITEWIN,
	BG_SELECT,
	BG_COUNT,
};
enum class BUTTON
{
	BUTTON_GAMESTART = 0,
	BUTTON_EXIT,
	BUTTON_WHITE_SELECT,
	BUTTON_BLACK_SELECT,
	BUTTON_WHITE_READY,
	BUTTON_BLACK_READY,
	BUTTON_COUNT
};
enum class GAME_SCENE
{
	SCENE_MENU,
	SCENE_GAMEPLAY,
	SCENE_GAMEEND,
};

class GameManager
{
private:
	static GameManager* m_myGameManager;
	GameManager();
	GAME_SCENE m_eScene;

	CHESS_INDEX m_Board[8][8];
	std::vector<ChessPiece*> m_vecChessPiece;
	int m_arrPlayerPlayTime[(int)PLAYER::PLAYER_COUNT];
	float m_fTimer;
	PLAYER m_player;

	HDC m_hdc;
	HDC m_backDC;
	int m_iclientRight;
	int m_iclientBottom;

	RECT m_buttonRect[(int)BUTTON::BUTTON_COUNT];
	Bitmap* m_ImageTile[(int)TILE::TILE_COUNT];
	Bitmap* m_ImageBG[(int)IMAGE_BG::BG_COUNT];
	Bitmap m_ImageButton[4];
	
	std::vector<RECT> m_vecMoveableRect;	//이동가능 위치 저장공간
	bool m_bMoveable;						//현재 이동가능상태인지
	ChessPiece* m_MoveablePiece;
	PLAYER m_eturn;
	bool m_arrIsReady[2];

	HBITMAP MyCreateDIBSection(HDC hdc, int width, int height);
	void SettingChessBoard();

	bool ClickInPlayScene(POINT point);
	bool ClickInMenuScene(POINT point);
	bool ClickInGameEndScene(POINT point);

	
	void KillPieceCheck(int x, int y);
	void GameClear();
	void PromotionPawn();

	void SetWindowMiddle(HWND hwnd);
	void DrawGameEndScene(HDC hdc);
	void DrawMenuScene(HDC hdc);	//게임 메뉴 씬
	void DrawGamePlayScene(HDC hdc);	//게임플레이 씬

	void DrawBackGround(HDC hdc);	//배경 그리기
public:
	~GameManager();
	static GameManager* Get_Instance()
	{
		if (m_myGameManager == NULL)
			m_myGameManager = new GameManager;
		return m_myGameManager;
	}

	bool Update(float deltatime);
	bool ColliderCheck(POINT point);
	void Draw();
	void Init(HWND hwnd);
	void Release(HWND hwnd);
	int GetTurn() { return (int)m_eturn; }
	void SetTurn(bool t) { m_eturn = (PLAYER)t; }
	int GetPlayer() { return (int)m_player; }
	void SetClientNumberPlayer(int p);
	bool IsMyTurn(POINT p);

};
