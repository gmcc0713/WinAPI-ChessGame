#include "Pawn.h"
bool Pawn::Move(RECT moveableRect)
{
	m_Rect = moveableRect;
	m_Pos.Pos_X = m_Rect.left / 75 - 1;									//말이 이동한 x위치 세팅
	m_Pos.Pos_Y = m_Rect.top / 75 - 1;									//말이 이동한 y위치 세팅

	if (m_bFirstMove)
		m_bFirstMove = false;											//첫이동일시 첫이동체크 false

	return m_Pos.Pos_Y == -7 * (int)m_Player + 7;								//black일때는 y좌표가 7 white일때는 y좌표가 0일때
}
std::vector<RECT> Pawn::MoveablePositionSet(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y])
{
	RECT rect;
	std::vector<RECT> moveableRect;
	int index = (-2 * (int)m_Player + 1);
	int x = m_Pos.Pos_X;
	int y = (int)m_Pos.Pos_Y + index;
	if (board[y][x] == CHESS_INDEX::INDEX_EMPTY)			//이동 한칸 앞이 비어있을때
	{
		moveableRect.push_back(ChangePosToRect(x, y));										//한칸앞 이동가능공간 저장

		if (m_bFirstMove && board[y + index ][x] == CHESS_INDEX::INDEX_EMPTY)	//첫 이동이고 두칸 앞이 비어있을때 
			moveableRect.push_back(ChangePosToRect(x, y+index));		//두칸앞 이동가능공간 저장 
	}
	for (int i = -1; i < 2; i += 2)	
	{
		x = (int)m_Pos.Pos_X + i;

		if (x >= 0 && y <= (int)MAP_SIZE::SIZE_Y - 1 && y >= 0 && x <= (int)MAP_SIZE::SIZE_X - 1 &&
			BoardPieceCheck(board, BOARD_PIECE::PIECE_ENEMY, x, y))											//대각선에 적 말이있으면
			moveableRect.push_back(ChangePosToRect(x, y));
	}

	return moveableRect;
}