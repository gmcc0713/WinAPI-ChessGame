#include "King.h"
std::vector<RECT> King::MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y])
{
	RECT rect;
	std::vector<RECT> moveableRect;
	Position posArr[8] = {
		{m_Pos.Pos_X - 1,m_Pos.Pos_Y + 1},
		{m_Pos.Pos_X - 1,m_Pos.Pos_Y - 1},
		{m_Pos.Pos_X ,m_Pos.Pos_Y + 1},
		{m_Pos.Pos_X ,m_Pos.Pos_Y - 1},
		{m_Pos.Pos_X - 1,m_Pos.Pos_Y },
		{m_Pos.Pos_X + 1,m_Pos.Pos_Y },
		{m_Pos.Pos_X + 1,m_Pos.Pos_Y + 1},
		{m_Pos.Pos_X + 1,m_Pos.Pos_Y - 1},
	};
	for (int i = 0; i < 8; i++)
	{
		if (posArr[i].Pos_X <= (int)MAP_SIZE::SIZE_X-1 && posArr[i].Pos_Y <= (int)MAP_SIZE::SIZE_Y-1 && posArr[i].Pos_X >= 0 && posArr[i].Pos_Y >= 0)
		{
			if (!BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, posArr[i].Pos_X, posArr[i].Pos_Y))		//비어있거나, 적군이있을때 
				moveableRect.push_back(ChangePosToRect(posArr[i].Pos_X, posArr[i].Pos_Y));
		}
	}
	return moveableRect;
}