#pragma once
#include "ChessPiece.h"
class Knight : public ChessPiece
{
public:
	virtual std::vector<RECT> MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y]);
};

