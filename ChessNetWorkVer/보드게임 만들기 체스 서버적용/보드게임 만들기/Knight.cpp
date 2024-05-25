#include "Knight.h"
std::vector<RECT> Knight::MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y])
{
	RECT rect;
	std::vector<RECT> moveableRect;
	const int endindexSize = (int)MAP_SIZE::SIZE_X-1;
	int x_pos,y_pos;
		for (int i = 1; i <= 2; i++)
		{
			x_pos = m_Pos.Pos_X + i;
			y_pos = m_Pos.Pos_Y + (3 - i);
			if (x_pos <= endindexSize && y_pos <= endindexSize && x_pos >= 0 && y_pos >= 0)
			{
				if (!BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x_pos, y_pos))		//비어있거나, 적군이있을때 
						moveableRect.push_back(ChangePosToRect(x_pos, y_pos));
			}
			x_pos = m_Pos.Pos_X + i;
			y_pos = m_Pos.Pos_Y - (3 - i);
			if (x_pos <= endindexSize && y_pos <= endindexSize && x_pos >= 0 && y_pos >= 0)
			{
				if (!BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x_pos, y_pos))		//비어있거나, 적군이있을때 
				moveableRect.push_back(ChangePosToRect(x_pos, y_pos));
			}
			x_pos = m_Pos.Pos_X - i;
			y_pos = m_Pos.Pos_Y + (3 - i);
			if (x_pos <= endindexSize && y_pos <= endindexSize && x_pos >= 0 && y_pos >= 0)
			{
				if (!BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x_pos, y_pos))		//비어있거나, 적군이있을때 
					moveableRect.push_back(ChangePosToRect(x_pos, y_pos));
			}
			x_pos = m_Pos.Pos_X - i;
			y_pos = m_Pos.Pos_Y - (3 - i);
			if (x_pos <= endindexSize && y_pos <= endindexSize && x_pos >= 0 && y_pos >= 0)
			{
				if (!BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x_pos, y_pos))		//비어있거나, 적군이있을때 
					moveableRect.push_back(ChangePosToRect(x_pos, y_pos));
			}
	}
	return moveableRect;
}