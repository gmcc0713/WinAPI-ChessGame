#pragma once
#include "Mecro.h"
enum class BOARD_PIECE
{
	PIECE_ENEMY,
	PIECE_MY,
	PIECE_EMPTY
};
class ChessPiece
{
protected:
	Bitmap* m_bitMap;
	Position m_Pos;     //보드게임의 x,y 좌표0~7
	SIZE m_isize;
	RECT m_Rect;		//현재 위치 사각형
	PLAYER m_Player;
	CHESS_INDEX m_myPiece;

	bool m_bMoveableType;		//현재 움직일수있는 위치 표시 타입인지 체크
public:

	virtual std::vector<RECT> MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y])=0;
	virtual bool Move(RECT moveableRect);
	RECT GetRect() { return m_Rect; }
	PLAYER GetPlayer() { return m_Player; }
	Position GetPos() { return m_Pos; }
	CHESS_INDEX GetPiece() { return m_myPiece; }
	bool BoardPieceCheck(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y], BOARD_PIECE piece, int x, int y);

	RECT ChangePosToRect(int x,int y);
	void Init(CHESS_INDEX Index,PLAYER player, int x, int y);
	void Draw(HDC hdc);
};
//    0   ~  7
//    ~
//    7