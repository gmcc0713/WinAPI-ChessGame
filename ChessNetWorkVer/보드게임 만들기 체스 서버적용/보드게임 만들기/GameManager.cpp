#include "GameManager.h"
GameManager* GameManager::m_myGameManager = NULL;
GameManager::GameManager()
{
	m_buttonRect[(int)BUTTON::BUTTON_GAMESTART] = { 620, 300, 750, 400 };
	m_buttonRect[(int)BUTTON::BUTTON_EXIT] = { 750, 430, 860, 500 };
	m_buttonRect[(int)BUTTON::BUTTON_WHITE_SELECT] = { 150, 350, 300, 430 };
	m_buttonRect[(int)BUTTON::BUTTON_BLACK_SELECT] = { 560, 350, 710, 430 };
	m_buttonRect[(int)BUTTON::BUTTON_WHITE_READY] = { 150, 470, 300, 550 };
	m_buttonRect[(int)BUTTON::BUTTON_BLACK_READY] = { 560, 470, 710, 550 };

	m_eScene = GAME_SCENE::SCENE_MENU;
	m_eturn =PLAYER::PLAYER_WHITE;
	m_fTimer = 0;
	m_player = PLAYER::PLAYER_NON;
	m_arrIsReady[0] = false;
	m_arrIsReady[1] = false;
}
void GameManager::Init(HWND hwnd)
{
	SetWindowMiddle(hwnd);
	BitmapManager::Get_Instance()->InIt(hwnd);

	for (int i = 0; i < (int)TILE::TILE_COUNT; i++)
		m_ImageTile[i] = BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_TILE1 +i);		//타일 이미지 저장
	for (int i = 0; i < (int)IMAGE_BG::BG_COUNT; i++)
		m_ImageBG[i] = BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_BG_0 + i);		//배경 이미지 저장
	for (int i = 0; i < (int)PLAYER::PLAYER_COUNT; i++)			//각 플레이어 남은시간 세팅
		m_arrPlayerPlayTime[i] = GAME_LEFT_TIME;

	for (int i = 0; i < 2; i++)		//
		m_ImageButton[i] = *BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_WHITE_BTN+i);
	for (int i = 2; i < 4; i++)
		m_ImageButton[i] = *BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_READY);
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	m_iclientRight = clientRect.right + 1;
	m_iclientBottom = clientRect.bottom + 1;
	m_hdc = GetDC(hwnd);
	m_backDC = CreateCompatibleDC(m_hdc);

}
void GameManager::SetWindowMiddle(HWND hwnd)
{
	int x, y, width, height;
	RECT rtDesk, rtWindow;
	GetWindowRect(GetDesktopWindow(), &rtDesk);
	GetWindowRect(hwnd, &rtWindow);
	width = rtWindow.right - rtWindow.left;
	height = rtWindow.bottom - rtWindow.top;
	x = (rtDesk.right - width) / 2;
	y = (rtDesk.bottom - height) / 2;
	MoveWindow(hwnd, x, y, width, height, TRUE);
}
void GameManager::SettingChessBoard()
{
	for (int x = 0; x < (int)MAP_SIZE::SIZE_X; x++)				//초기값 세팅
	{
		for (int y = 0; y < (int)MAP_SIZE::SIZE_Y; y++)
		{
			m_Board[y][x] = CHESS_INDEX::INDEX_EMPTY;
		}
	}

	ChessPiece* chessPiece;
	for (int j = 0; j < 2; j++)		//폰 세팅
	{
		for (int x = 0; x < (int)MAP_SIZE::SIZE_X; x++)
		{
			chessPiece = new Pawn;
			chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_PAWN), (PLAYER)j, x, 1 + 5 * j);
			m_vecChessPiece.push_back(chessPiece);
			if(j==0)
				m_Board[1 + 5 * j][x] = CHESS_INDEX::INDEX_BLACK_PAWN;
			else
				m_Board[1 + 5 * j][x] = CHESS_INDEX::INDEX_WHITE_PAWN;
		}
	}

	for (int j = 0; j < 2; j++)		
	{
		chessPiece = new Rook;
		chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_ROOK), (PLAYER)j, 0, j * 7);		//룩 세팅
		m_vecChessPiece.push_back(chessPiece);
		m_Board[j * 7][0] =(CHESS_INDEX)(j*6 +(int)CHESS_INDEX::INDEX_BLACK_ROOK);

		chessPiece = new Knight;
		chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_KNIGHT), (PLAYER)j, 1,j * 7);		//나이트 세팅
		m_vecChessPiece.push_back(chessPiece);
		m_Board[j * 7][1] = (CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_KNIGHT);

		chessPiece = new Bishop;
		chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_BISHOP), (PLAYER)j, 2,j * 7);		//비숍 세팅
		m_vecChessPiece.push_back(chessPiece);
		m_Board[j * 7][2] = (CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_BISHOP);

		chessPiece = new King;
		chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_KING), (PLAYER)j, 3, j * 7);		//킹 세팅
		m_vecChessPiece.push_back(chessPiece);
		m_Board[j * 7][3] = (CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_KING);

		chessPiece = new Queen;
		chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_QUEEN), (PLAYER)j, 4,j * 7);		//퀸 세팅
		m_vecChessPiece.push_back(chessPiece);
		m_Board[j * 7][4] = (CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_QUEEN);

		chessPiece = new Bishop;
		chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_BISHOP), (PLAYER)j, 5, j * 7);		//비숍 세팅
		m_vecChessPiece.push_back(chessPiece);
		m_Board[j * 7][5] = (CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_BISHOP);

		chessPiece = new Knight;
		chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_KNIGHT), (PLAYER)j, 6,j * 7);		//나이트 세팅
		m_vecChessPiece.push_back(chessPiece);
		m_Board[j * 7][6] = (CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_KNIGHT);

		chessPiece = new Rook;
		chessPiece->Init((CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_ROOK), (PLAYER)j, 7,j * 7);		//룩 세팅
		m_vecChessPiece.push_back(chessPiece);
		m_Board[j * 7][7] = (CHESS_INDEX)(j * 6 + (int)CHESS_INDEX::INDEX_BLACK_ROOK);
	}
}

void GameManager::Draw()
{
	HBITMAP backBitmap = MyCreateDIBSection(m_hdc, m_iclientRight, m_iclientBottom);
	SelectObject(m_backDC, backBitmap);

	switch (m_eScene)
	{
	case GAME_SCENE::SCENE_GAMEPLAY:
		DrawGamePlayScene(m_backDC);
		break;
	case GAME_SCENE::SCENE_MENU:
		DrawMenuScene(m_backDC);
		break;
	case GAME_SCENE::SCENE_GAMEEND:
		DrawGameEndScene(m_backDC);
		break;
	}

	BitBlt(m_hdc, 0, 0, m_iclientRight, m_iclientBottom, m_backDC, 0, 0, SRCCOPY);

	DeleteObject(backBitmap);
}
void GameManager::DrawMenuScene(HDC hdc)
{
	m_ImageBG[4]->Draw(hdc, 0, 0);

	m_ImageButton[0].Draw(hdc, 150, 350, 150, 80);		//준비하기
	m_ImageButton[1].Draw(hdc, 560, 350, 150, 80);

	m_ImageButton[2].Draw(hdc, 150, 470, 150, 80);		//접속상태
	m_ImageButton[3].Draw(hdc, 560, 470, 150, 80);

}
void GameManager::DrawGamePlayScene(HDC hdc)		//게임 플레이 화면 그리기
{
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawBackGround(hdc);			//배경&보드판
	for (ChessPiece* chessPiece : m_vecChessPiece)		//체스말 그리기
		chessPiece->Draw(hdc);
	for (const RECT& rect : m_vecMoveableRect)			//이동가능 상자 그리기
		m_ImageTile[(int)TILE::TILE_MOVEABLE]->Draw(hdc, rect.left, rect.top, 75,75, true);
}
void GameManager::DrawGameEndScene(HDC hdc)		//게임종료 신
{
	if (m_eturn == PLAYER::PLAYER_WHITE)
		m_ImageBG[(int)IMAGE_BG::BG_BLACKWIN]->Draw(hdc, 0, 0);
	else
		m_ImageBG[(int)IMAGE_BG::BG_WHITEWIN]->Draw(hdc, 0, 0);
}
void GameManager::DrawBackGround(HDC hdc)			//배경그리기
{
	std::wstring str[8] = { L"a", L"b",L"c",L"d",L"e",L"f",L"g",L"h"};	
	
	m_ImageTile[(int)TILE::TILE_BACKGROUND3]->Draw(hdc, 0, 0, 900,700, false);
	m_ImageBG[1]->Draw(hdc, 675, 0);
	
	int text_pos = 75 * 0.5f;
	for (int y = 0; y < 8; y++)							//보드판 타일그리기
	{
		for (int x = 0; x < (int)MAP_SIZE::SIZE_X; x++)
		{
			if((x+y) % 2==0)
				m_ImageTile[(int)TILE::TILE_BACKGROUND1]->Draw(hdc,75+ 75 *x,75 + 75 * y,75,75,false);
			else
				m_ImageTile[(int)TILE::TILE_BACKGROUND2]->Draw(hdc, 75 + 75 * x,75 + 75 * y, 75, 75, false);
		}
	}

	for (int i = 0; i <= (int)MAP_SIZE::SIZE_X; i++)		//포드판 선긋기
	{
		MoveToEx(hdc, 75 + 75 * i, 0, NULL);
		LineTo(hdc, 75 + 75 * i, 675);
		MoveToEx(hdc, 0, 75 * i, NULL);
		LineTo(hdc, 675, 75 * i);
	}	
	for (int i = 0; i < 8; i++)
	{
		
		TextOutW(hdc, text_pos,75 + text_pos + 75* i, std::to_wstring(i+1).c_str(), 1);
		TextOutW(hdc, 75 + text_pos + 75 * i, text_pos, str[i].c_str(), 1);
	}
	MoveToEx(hdc,0,0, NULL);
	LineTo(hdc, 75,75);
	
	if (m_eturn == PLAYER::PLAYER_BLACK)
		TextOut(hdc, 760, 320, " : Black", 8);
	else
		TextOut(hdc, 760, 320, " : White", 8);
	TextOut(hdc, 730, 320, "Turn",4);
	if(m_player == PLAYER::PLAYER_BLACK)
		TextOut(hdc, 730, 420, " 나 : Black", 11);
	else
		TextOut(hdc, 730, 420, " 나 : White", 11);
	std::string timestr = "남은시간 : "+std::to_string(m_arrPlayerPlayTime[(int)PLAYER::PLAYER_BLACK]);
	TextOut(hdc, 730, 100, timestr.c_str(), timestr.size());
	timestr = "남은시간 : " + std::to_string(m_arrPlayerPlayTime[(int)PLAYER::PLAYER_WHITE]);
	TextOut(hdc, 730, 600, timestr.c_str(), timestr.size());

	
}

bool GameManager::ColliderCheck(POINT point)
{
	switch (m_eScene)
	{
	case GAME_SCENE::SCENE_MENU:
		return ClickInMenuScene(point);
	case GAME_SCENE::SCENE_GAMEPLAY:
		return ClickInPlayScene(point);
	case GAME_SCENE::SCENE_GAMEEND:
		return ClickInGameEndScene(point);
	}
	return false;
}
bool GameManager::ClickInPlayScene(POINT point)
{
	if (m_bMoveable)			//이동가능 상자 표시상태일때
	{
		for (const RECT &moveablerect : m_vecMoveableRect)	//이동 가능 위치 반복
		{
			
			if (PtInRect(&moveablerect, point))			//이동가능 위치 클릭
			{
				KillPieceCheck(moveablerect.left/75-1,moveablerect.top/75 - 1);	// 상대 말을 없애는지 체크
				m_Board[m_MoveablePiece->GetPos().Pos_Y][m_MoveablePiece->GetPos().Pos_X] = CHESS_INDEX::INDEX_EMPTY;			//게임판의 기존 위치에있던 말 삭제
				if (m_MoveablePiece->Move(moveablerect))						//폰이 끝에 도달했는가
					PromotionPawn();
				else
					m_Board[m_MoveablePiece->GetPos().Pos_Y][m_MoveablePiece->GetPos().Pos_X] = m_MoveablePiece->GetPiece();						//게임판의 이동 위치의 말 세팅
				m_bMoveable = false;
				m_vecMoveableRect.clear();
				if (m_eturn == PLAYER::PLAYER_WHITE)
					m_eturn = PLAYER::PLAYER_BLACK;
				else
					m_eturn = PLAYER::PLAYER_WHITE;
				return true;
			}
		m_bMoveable = false;
		}
		m_vecMoveableRect.clear();
		return false;
		
	}
	else						//이동가능 상자 표시상태아닐때
	{
		RECT rect;
		for (ChessPiece* chessPiece : m_vecChessPiece)
		{
			rect = chessPiece->GetRect();
			if (PtInRect(&rect, point))		//현재 자기 차례이면서 말을 눌렀을때
			{
				if (chessPiece->GetPlayer() == m_eturn)
				{
					m_vecMoveableRect = chessPiece->MoveablePositionSet(m_Board);
					if (m_vecMoveableRect.size() != 0)
					{
						m_bMoveable = true;
						m_MoveablePiece = chessPiece;
						if(m_eturn==PLAYER::PLAYER_WHITE)
						return true;
					}
					break;
				}
				
			}
		}
		return false;
	}

}
bool GameManager::ClickInGameEndScene(POINT point)
{
	if (PtInRect(&m_buttonRect[(int)BUTTON::BUTTON_EXIT], point))
	{
		GameClear();
		m_eScene = GAME_SCENE::SCENE_MENU;
		return true;
	}
	return false;
}

void GameManager::PromotionPawn()
{
	ChessPiece* queenPiece = new Queen;	//퀸 생성
	queenPiece->Init((CHESS_INDEX)((int)m_MoveablePiece->GetPlayer() * 6 + (int)CHESS_INDEX::INDEX_BLACK_QUEEN), m_MoveablePiece->GetPlayer(), m_MoveablePiece->GetPos().Pos_X, m_MoveablePiece->GetPos().Pos_Y);
	m_vecChessPiece.push_back(queenPiece);//퀸 삽입
	m_Board[queenPiece->GetPos().Pos_Y][queenPiece->GetPos().Pos_X] = queenPiece->GetPiece();
	for (auto iter = m_vecChessPiece.begin(); iter != m_vecChessPiece.end(); iter++)
	{
		if ((*iter)->GetPos().Pos_X == queenPiece->GetPos().Pos_X && (*iter)->GetPos().Pos_Y == queenPiece->GetPos().Pos_Y)			//퀸으로 면한 폰 찾기
		{
			m_vecChessPiece.erase(iter);					//그 위치의 폰 삭제
			break;
		}
	}
}
bool GameManager::ClickInMenuScene(POINT point)
{
	if (PtInRect(&m_buttonRect[(int)BUTTON::BUTTON_WHITE_READY], point))
	{
		m_ImageButton[2] = *BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_READY_END);
		m_arrIsReady[0] = true;
		return false;
	}
	else if (PtInRect(&m_buttonRect[(int)BUTTON::BUTTON_BLACK_READY], point))
	{
		m_ImageButton[3] = *BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_READY_END);
		m_arrIsReady[1] = true;
		return false;
	}
	else if (m_arrIsReady[(int)PLAYER::PLAYER_WHITE] && m_arrIsReady[(int)PLAYER::PLAYER_BLACK])
	{
		return true;
	}
	return false;
}

void GameManager::KillPieceCheck(int x,int y)
{
	if (m_Board[y][x] != CHESS_INDEX::INDEX_EMPTY)		//이동할 자리가 비어있지 않을때
	{
		for (auto iter = m_vecChessPiece.begin(); iter != m_vecChessPiece.end(); iter++)
		{
			if ((*iter)->GetPos().Pos_X ==x && (*iter)->GetPos().Pos_Y ==y)			//그 위치의 상대말 삭제
			{
				if ((*iter)->GetPiece() == CHESS_INDEX::INDEX_BLACK_KING || (*iter)->GetPiece() == CHESS_INDEX::INDEX_WHITE_KING)
				{
					m_eScene = GAME_SCENE::SCENE_GAMEEND;
				}
				m_vecChessPiece.erase(iter);
				break;
			}
		}
	}
}
void GameManager::GameClear()
{
	m_vecChessPiece.clear();
	m_vecMoveableRect.clear();
	m_eturn = PLAYER::PLAYER_WHITE;
	for (int i = 0; i < (int)PLAYER::PLAYER_COUNT; i++)
		m_arrPlayerPlayTime[i] = GAME_LEFT_TIME;
}
bool GameManager::Update(float deltatime)
{	
	switch (m_eScene)
	{
	case GAME_SCENE::SCENE_MENU:
		if (m_arrIsReady[0] && m_arrIsReady[1])
		{
			m_eScene = GAME_SCENE::SCENE_GAMEPLAY;
			SettingChessBoard();
			return true;
		}
		return false;
	case GAME_SCENE::SCENE_GAMEPLAY:
		m_fTimer += deltatime;
		if (m_fTimer >= 1)
		{
			m_fTimer -= 1;
			if (m_eturn == PLAYER::PLAYER_BLACK)
				m_arrPlayerPlayTime[(int)PLAYER::PLAYER_BLACK]--;
			else
				m_arrPlayerPlayTime[(int)PLAYER::PLAYER_WHITE]--;

			if (m_arrPlayerPlayTime[(int)PLAYER::PLAYER_BLACK] < 0 || m_arrPlayerPlayTime[(int)PLAYER::PLAYER_WHITE] < 0)
				m_eScene = GAME_SCENE::SCENE_GAMEEND;
			return true;
		}
	}

	return false;
}
void GameManager::Release(HWND hwnd)
{
	delete BitmapManager::Get_Instance();

	DeleteDC(m_backDC);
	ReleaseDC(hwnd, m_hdc);

	delete m_myGameManager;
	m_myGameManager = NULL;
}
HBITMAP GameManager::MyCreateDIBSection(HDC hdc, int width, int height)
{
	BITMAPINFO bm_info;
	ZeroMemory(&bm_info.bmiHeader, sizeof(BITMAPINFOHEADER));

	bm_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	bm_info.bmiHeader.biBitCount = 24;
	bm_info.bmiHeader.biWidth = width;
	bm_info.bmiHeader.biHeight = height;
	bm_info.bmiHeader.biPlanes = 1;

	LPVOID pBits;
	return CreateDIBSection(hdc, &bm_info, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
}
void GameManager::SetClientNumberPlayer(int p)
{
	if (p == 1)
	{
		m_player = PLAYER::PLAYER_WHITE;
		m_ImageButton[(int)PLAYER::PLAYER_BLACK] = *BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_ME);
		m_ImageButton[(int)PLAYER::PLAYER_WHITE] = *BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_ENEMY);
	}
	else if(p == 0)
	{
		m_player = PLAYER::PLAYER_BLACK;
		m_ImageButton[(int)PLAYER::PLAYER_WHITE] = *BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_ME);
		m_ImageButton[(int)PLAYER::PLAYER_BLACK] = *BitmapManager::Get_Instance()->GetImage((int)CHESS_INDEX::INDEX_ENEMY);

	}
}
bool GameManager::IsMyTurn(POINT p)
{
	switch (m_eScene)
	{
	case GAME_SCENE::SCENE_MENU:
	{
		if (m_player == PLAYER::PLAYER_WHITE && p.x <= 450)
		{
			return true;
		}
		else if (m_player == PLAYER::PLAYER_BLACK && p.x >= 450)
		{
			return true;
		}
		return false;
	}
	case GAME_SCENE::SCENE_GAMEPLAY:
		return m_eturn == m_player;
	}

}
GameManager::~GameManager()
{
	
}