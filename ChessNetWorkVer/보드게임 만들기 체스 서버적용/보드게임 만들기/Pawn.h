#pragma once
#include "ChessPiece.h"
class Pawn : public ChessPiece
{
private:
	bool m_bFirstMove;
public:
	Pawn() : m_bFirstMove(true) {}
	virtual bool Move(RECT moveableRect);
	virtual std::vector<RECT> MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y]);
};

