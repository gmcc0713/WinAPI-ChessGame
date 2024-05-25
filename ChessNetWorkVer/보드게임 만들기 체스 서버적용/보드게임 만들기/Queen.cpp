#include "Queen.h"
std::vector<RECT> Queen::MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y])
{
	RECT rect;
	std::vector<RECT> moveableRect;
	int offset = 1;
	bool left = true, right = true, up = true, down = true;
	int x, y;

	for (int i = 0; i < 2; i++)
	{
		while (left || right || up || down)
		{
			if (left)																															//좌,좌하  체크
			{
				x = m_Pos.Pos_X - offset;
				y = m_Pos.Pos_Y + offset * i;
				if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 
					&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//범위를 안벗어나고 아군이 아니면 저장
					moveableRect.push_back(ChangePosToRect(x, y));
				if (x < 0 || y>(int)MAP_SIZE::SIZE_Y - 1 || y < 0 || x>(int)MAP_SIZE::SIZE_X - 1 ||
					!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//범위를 벗어나거나 아군이거나 적군이면 false
					left = false;
			}
			if (right)																															//하,우하 체크
			{
				x = m_Pos.Pos_X + offset*i;
				y = m_Pos.Pos_Y + offset;
				if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 
					&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//범위를 안벗어나고아군이 아니면 저장
					moveableRect.push_back(ChangePosToRect(x, y));
				if (x < 0 || y>(int)MAP_SIZE::SIZE_Y - 1 || y < 0 || x>(int)MAP_SIZE::SIZE_X - 1 ||
					!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//범위를 벗어나거나 아군이거나 적군이면 false
					right = false;
			}
			if (up)																															//상, 좌상 체크
			{
				x = m_Pos.Pos_X - offset * i;
				y = m_Pos.Pos_Y - offset;
				if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 
					&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//범위를 안벗어나고 아군이 아니면 저장
					moveableRect.push_back(ChangePosToRect(x, y));
				if (x < 0 || y>(int)MAP_SIZE::SIZE_Y - 1 || y < 0 || x>(int)MAP_SIZE::SIZE_X - 1 ||
					!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//범위를 벗어나거나 아군이거나 적군이면 false
					up = false;
			}
			if (down)																															//우 우상	
			{
				x = m_Pos.Pos_X + offset;
				y = m_Pos.Pos_Y - offset * i;
				if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 
					&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//범위를 안벗어나고 아군이 아니면 저장
					moveableRect.push_back(ChangePosToRect(x, y));
				if (x < 0 || y>(int)MAP_SIZE::SIZE_Y - 1 || y < 0 || x>(int)MAP_SIZE::SIZE_X - 1 ||
					!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//범위를 벗어나거나 아군이거나 적군이면 false
					down = false;
			}
			offset++;
		}
		left = true, right = true, up = true, down = true;
		offset = 1;
	}
	return moveableRect;
}