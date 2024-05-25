#include "Pawn.h"
bool Pawn::Move(RECT moveableRect)
{
	m_Rect = moveableRect;
	m_Pos.Pos_X = m_Rect.left / 75 - 1;									//���� �̵��� x��ġ ����
	m_Pos.Pos_Y = m_Rect.top / 75 - 1;									//���� �̵��� y��ġ ����

	if (m_bFirstMove)
		m_bFirstMove = false;											//ù�̵��Ͻ� ù�̵�üũ false

	return m_Pos.Pos_Y == -7 * (int)m_Player + 7;								//black�϶��� y��ǥ�� 7 white�϶��� y��ǥ�� 0�϶�
}
std::vector<RECT> Pawn::MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y])
{
	RECT rect;
	std::vector<RECT> moveableRect;
	int index = (-2 * (int)m_Player + 1);
	int x = m_Pos.Pos_X;
	int y = (int)m_Pos.Pos_Y + index;
	if (board[y][x] == CHESS_INDEX::INDEX_EMPTY)			//�̵� ��ĭ ���� ���������
	{
		moveableRect.push_back(ChangePosToRect(x, y));										//��ĭ�� �̵����ɰ��� ����

		if (m_bFirstMove && board[y + index ][x] == CHESS_INDEX::INDEX_EMPTY)	//ù �̵��̰� ��ĭ ���� ��������� 
			moveableRect.push_back(ChangePosToRect(x, y+index));		//��ĭ�� �̵����ɰ��� ���� 
	}
	for (int i = -1; i < 2; i += 2)	
	{
		x = (int)m_Pos.Pos_X + i;

		if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 &&
			BoardPieceCheck(board, BOARD_PIECE::PIECE_ENEMY, x, y))											//�밢���� �� ����������
			moveableRect.push_back(ChangePosToRect(x, y));
	}

	return moveableRect;
}