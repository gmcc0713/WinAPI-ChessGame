#include "Rook.h"

std::vector<RECT> Rook::MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y])
{
	RECT rect;
	std::vector<RECT> moveableRect;
	int offset=1;
	bool left=true, right = true, top = true, bottom = true;
	int x, y;
	while (left || right || top || bottom)
	{
		if(left)																															//왼쪽 체크
		{
			x = m_Pos.Pos_X - offset;
			y = m_Pos.Pos_Y;
			if (x >= 0 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//아군이 아니면 저장
				moveableRect.push_back(ChangePosToRect(x, y));

			if (x < 0 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//아군이거나 적군이면 false
				left = false;
		}
		if(right)																															//오른쪽 체크
		{
			x = m_Pos.Pos_X + offset;
			y = m_Pos.Pos_Y;
			if (x <= (int)MAP_SIZE::SIZE_X - 1 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//아군이 아니면 저장
				moveableRect.push_back(ChangePosToRect(x, m_Pos.Pos_Y));

			if (x > (int)MAP_SIZE::SIZE_X - 1 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//아군이거나 적군이면 false
				right = false;
		}
		if (top)																															//위쪽 체크
		{
			x = m_Pos.Pos_X ;
			y = m_Pos.Pos_Y - offset;
			if (y >= 0 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))										//아군이 아니면 저장
				moveableRect.push_back(ChangePosToRect(x, y));

			if(y < 0 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))									//아군이거나 적군이면 false
				top = false;
		}
		if (bottom)																															//아래쪽 체크
		{
			x = m_Pos.Pos_X;
			y = m_Pos.Pos_Y + offset;
			if (y <= (int)MAP_SIZE::SIZE_Y - 1 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//아군이 아니면 저장
				moveableRect.push_back(ChangePosToRect(x, m_Pos.Pos_Y + offset));

			if (y > (int)MAP_SIZE::SIZE_Y - 1 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//아군이거나 적군이면 false
				bottom = false;
		}
		offset++;
	}

	return moveableRect;
}