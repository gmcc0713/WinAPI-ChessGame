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
		if(left)																															//���� üũ
		{
			x = m_Pos.Pos_X - offset;
			y = m_Pos.Pos_Y;
			if (x >= 0 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//�Ʊ��� �ƴϸ� ����
				moveableRect.push_back(ChangePosToRect(x, y));

			if (x < 0 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//�Ʊ��̰ų� �����̸� false
				left = false;
		}
		if(right)																															//������ üũ
		{
			x = m_Pos.Pos_X + offset;
			y = m_Pos.Pos_Y;
			if (x <= (int)MAP_SIZE::SIZE_X - 1 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//�Ʊ��� �ƴϸ� ����
				moveableRect.push_back(ChangePosToRect(x, m_Pos.Pos_Y));

			if (x > (int)MAP_SIZE::SIZE_X - 1 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//�Ʊ��̰ų� �����̸� false
				right = false;
		}
		if (top)																															//���� üũ
		{
			x = m_Pos.Pos_X ;
			y = m_Pos.Pos_Y - offset;
			if (y >= 0 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))										//�Ʊ��� �ƴϸ� ����
				moveableRect.push_back(ChangePosToRect(x, y));

			if(y < 0 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))									//�Ʊ��̰ų� �����̸� false
				top = false;
		}
		if (bottom)																															//�Ʒ��� üũ
		{
			x = m_Pos.Pos_X;
			y = m_Pos.Pos_Y + offset;
			if (y <= (int)MAP_SIZE::SIZE_Y - 1 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//�Ʊ��� �ƴϸ� ����
				moveableRect.push_back(ChangePosToRect(x, m_Pos.Pos_Y + offset));

			if (y > (int)MAP_SIZE::SIZE_Y - 1 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//�Ʊ��̰ų� �����̸� false
				bottom = false;
		}
		offset++;
	}

	return moveableRect;
}