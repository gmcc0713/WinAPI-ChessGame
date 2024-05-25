#include "Bishop.h"
std::vector<RECT> Bishop::MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y])
{
	RECT rect;
	std::vector<RECT> moveableRect;
	int offset = 1;
	bool leftdown = true, rightdown = true, leftup = true, rightup = true;
	int x, y;
	while (leftdown || rightdown || leftup || rightup)
	{
		if (leftdown)																															//좌 하 체크
		{
			x = m_Pos.Pos_X - offset;
			y = m_Pos.Pos_Y + offset;
			if (x >= 0 && y <= (int)MAP_SIZE::SIZE_X-1 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x,y))				//범위를 안벗어나고 아군이 아니면 저장
				moveableRect.push_back(ChangePosToRect(x,y));
			if (x < 0 || y>(int)MAP_SIZE::SIZE_X-1 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x,y))			//범위를 벗어나거나 아군이거나 적군이면 false
				leftdown = false;
		}
		if (rightdown)																															//오른쪽 체크
		{
			x = m_Pos.Pos_X + offset;
			y = m_Pos.Pos_Y + offset;
			if (x <= (int)MAP_SIZE::SIZE_X-1 && y <= (int)MAP_SIZE::SIZE_Y-1 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//범위를 안벗어나고아군이 아니면 저장
				moveableRect.push_back(ChangePosToRect(x, y));
			if (x > (int)MAP_SIZE::SIZE_X-1 || y > (int)MAP_SIZE::SIZE_Y-1 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//범위를 벗어나거나 아군이거나 적군이면 false
				rightdown = false;
		}
		if (leftup)																															//위쪽 체크
		{
			x = m_Pos.Pos_X - offset;
			y = m_Pos.Pos_Y - offset;
			if (x >= 0 && y >= 0 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//범위를 안벗어나고 아군이 아니면 저장
				moveableRect.push_back(ChangePosToRect(x, y));
			if (x < 0 || y < 0 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//범위를 벗어나거나 아군이거나 적군이면 false
				leftup = false;
		}
		if (rightup)																															//아래쪽 체크	
		{
			x = m_Pos.Pos_X + offset;
			y = m_Pos.Pos_Y -offset;
			if (x <= (int)MAP_SIZE::SIZE_X-1 && y - offset >= 0&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY,x,y))				//범위를 안벗어나고 아군이 아니면 저장
				moveableRect.push_back(ChangePosToRect(x, y));
			if (x > (int)MAP_SIZE::SIZE_X-1 ||y <0||!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//범위를 벗어나거나 아군이거나 적군이면 false
				rightup = false;
		}
		offset++;
	}

	return moveableRect;
}