#include "ChessPiece.h"
void ChessPiece::Draw(HDC hdc)
{
	m_bitMap->Draw(hdc,m_Rect.left, m_Rect.top, 75, 75, false);
}
void ChessPiece::Init(CHESS_INDEX Index, PLAYER player, int x, int y)
{
	m_bitMap = BitmapManager::Get_Instance()->GetImage((int)Index);
	m_myPiece = Index;
	m_Player = player;
	m_Pos.Pos_X = x;
	m_Pos.Pos_Y = y;
	m_isize.cx = m_bitMap->GetSize().cx * 0.6f;
	m_isize.cy = m_bitMap->GetSize().cy * 0.6f;
	m_Rect.left = (m_Pos.Pos_X+1) * 75;
	m_Rect.top = (m_Pos.Pos_Y+1) * 75;
	m_Rect.right = (m_Pos.Pos_X + 2) * 75;
	m_Rect.bottom = (m_Pos.Pos_Y + 2 ) * 75;
	m_bMoveableType = false; 
}
bool ChessPiece::Move(RECT moveableRect)
{
	m_Rect = moveableRect;												//이동된 위치 세팅
	m_Pos.Pos_X = m_Rect.left / 75 - 1;									//말이 이동한 x위치 세팅
	m_Pos.Pos_Y = m_Rect.top / 75 - 1;									//말이 이동한 y위치 세팅
	return false;
}
RECT ChessPiece::ChangePosToRect(int x, int y)
{
	RECT rect = { (x + 1) * 75, (y + 1) * 75, (x + 2) * 75, (y + 2) * 75 };
	return rect;
}
bool ChessPiece::BoardPieceCheck(CHESS_INDEX board[(int)MAP_SIZE::SIZE_X][(int)MAP_SIZE::SIZE_Y], BOARD_PIECE piece, int x, int y)
{
	int enemyCheck = (6 - (int)m_Player * 6 + (int)CHESS_INDEX::INDEX_BLACK_PAWN);
	switch (piece)
	{
		case BOARD_PIECE::PIECE_MY:
			return board[y][x] > (CHESS_INDEX)(5 - enemyCheck) &&
			board[y][x] <= (CHESS_INDEX)(11 - enemyCheck);			//아군 말이있으면
		case BOARD_PIECE::PIECE_ENEMY:
			return board[y][x] >= (CHESS_INDEX)enemyCheck &&
			board[y][x] <= (CHESS_INDEX)(enemyCheck + 5);		//적 말이있으면
		case BOARD_PIECE::PIECE_EMPTY:
			return board[y][x] == CHESS_INDEX::INDEX_EMPTY;
	}
	return false;
}