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
		if (leftdown)																															//�� �� üũ
		{
			x = m_Pos.Pos_X - offset;
			y = m_Pos.Pos_Y + offset;
			if (x >= 0 && y <= (int)MAP_SIZE::SIZE_X-1 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x,y))				//������ �ȹ���� �Ʊ��� �ƴϸ� ����
				moveableRect.push_back(ChangePosToRect(x,y));
			if (x < 0 || y>(int)MAP_SIZE::SIZE_X-1 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x,y))			//������ ����ų� �Ʊ��̰ų� �����̸� false
				leftdown = false;
		}
		if (rightdown)																															//������ üũ
		{
			x = m_Pos.Pos_X + offset;
			y = m_Pos.Pos_Y + offset;
			if (x <= (int)MAP_SIZE::SIZE_X-1 && y <= (int)MAP_SIZE::SIZE_Y-1 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//������ �ȹ����Ʊ��� �ƴϸ� ����
				moveableRect.push_back(ChangePosToRect(x, y));
			if (x > (int)MAP_SIZE::SIZE_X-1 || y > (int)MAP_SIZE::SIZE_Y-1 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//������ ����ų� �Ʊ��̰ų� �����̸� false
				rightdown = false;
		}
		if (leftup)																															//���� üũ
		{
			x = m_Pos.Pos_X - offset;
			y = m_Pos.Pos_Y - offset;
			if (x >= 0 && y >= 0 && !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//������ �ȹ���� �Ʊ��� �ƴϸ� ����
				moveableRect.push_back(ChangePosToRect(x, y));
			if (x < 0 || y < 0 || !BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//������ ����ų� �Ʊ��̰ų� �����̸� false
				leftup = false;
		}
		if (rightup)																															//�Ʒ��� üũ	
		{
			x = m_Pos.Pos_X + offset;
			y = m_Pos.Pos_Y -offset;
			if (x <= (int)MAP_SIZE::SIZE_X-1 && y - offset >= 0&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY,x,y))				//������ �ȹ���� �Ʊ��� �ƴϸ� ����
				moveableRect.push_back(ChangePosToRect(x, y));
			if (x > (int)MAP_SIZE::SIZE_X-1 ||y <0||!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//������ ����ų� �Ʊ��̰ų� �����̸� false
				rightup = false;
		}
		offset++;
	}

	return moveableRect;
}