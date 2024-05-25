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
			if (left)																															//��,����  üũ
			{
				x = m_Pos.Pos_X - offset;
				y = m_Pos.Pos_Y + offset * i;
				if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 
					&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//������ �ȹ���� �Ʊ��� �ƴϸ� ����
					moveableRect.push_back(ChangePosToRect(x, y));
				if (x < 0 || y>(int)MAP_SIZE::SIZE_Y - 1 || y < 0 || x>(int)MAP_SIZE::SIZE_X - 1 ||
					!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//������ ����ų� �Ʊ��̰ų� �����̸� false
					left = false;
			}
			if (right)																															//��,���� üũ
			{
				x = m_Pos.Pos_X + offset*i;
				y = m_Pos.Pos_Y + offset;
				if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 
					&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//������ �ȹ����Ʊ��� �ƴϸ� ����
					moveableRect.push_back(ChangePosToRect(x, y));
				if (x < 0 || y>(int)MAP_SIZE::SIZE_Y - 1 || y < 0 || x>(int)MAP_SIZE::SIZE_X - 1 ||
					!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//������ ����ų� �Ʊ��̰ų� �����̸� false
					right = false;
			}
			if (up)																															//��, �»� üũ
			{
				x = m_Pos.Pos_X - offset * i;
				y = m_Pos.Pos_Y - offset;
				if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 
					&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//������ �ȹ���� �Ʊ��� �ƴϸ� ����
					moveableRect.push_back(ChangePosToRect(x, y));
				if (x < 0 || y>(int)MAP_SIZE::SIZE_Y - 1 || y < 0 || x>(int)MAP_SIZE::SIZE_X - 1 ||
					!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//������ ����ų� �Ʊ��̰ų� �����̸� false
					up = false;
			}
			if (down)																															//�� ���	
			{
				x = m_Pos.Pos_X + offset;
				y = m_Pos.Pos_Y - offset * i;
				if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 
					&& !BoardPieceCheck(board, BOARD_PIECE::PIECE_MY, x, y))				//������ �ȹ���� �Ʊ��� �ƴϸ� ����
					moveableRect.push_back(ChangePosToRect(x, y));
				if (x < 0 || y>(int)MAP_SIZE::SIZE_Y - 1 || y < 0 || x>(int)MAP_SIZE::SIZE_X - 1 ||
					!BoardPieceCheck(board, BOARD_PIECE::PIECE_EMPTY, x, y))			//������ ����ų� �Ʊ��̰ų� �����̸� false
					down = false;
			}
			offset++;
		}
		left = true, right = true, up = true, down = true;
		offset = 1;
	}
	return moveableRect;
}