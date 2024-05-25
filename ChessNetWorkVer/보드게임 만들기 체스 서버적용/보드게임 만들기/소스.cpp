#define _WINSOCK_DEPRECATED_NO_WARNINGS		//Winsock2보다 위에 있어야된다.
#pragma comment(lib,"ws2_32.lib")			//윈속 라이브러리 사용을 알린다
#include <WinSock2.h>						//윈속 사용을 위하여 헤더파일 추가.
///////////////////////
#include<Windows.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "GameManager.h"

#define MAX_LOADSTRING 100
#define WM_SOCKET (WM_USER + 1)
#define SCREENWIDTH  900
#define SCREENHEIGHT 710

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("First");
TCHAR str[256];
SOCKET g_sock;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ErrorQuit(const char* caption);
void ErrorDisplay(HWND hwnd, const char* caption);
void ErrorDisplay(HWND hwnd, const int& errorCode, const char* caption);
void SocketProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

enum Packet_ID { user, play };
#pragma pack(1) //byte지정을 1byte단위로
struct BasePacket		
{
	Packet_ID id;		//user인지 play인지 
	int reciveByte;		//더 추가로 받아야할 Byte수, user일때는 clientNumber인 4Byte, PlayType일때는 bool,int,int 라서 9Byte로 Send에서 정해야한다
		 
};
struct Play_Packet : BasePacket		//패킷
{
	bool turn;
	int x;
	int y;
};
struct User_Packet : BasePacket			//패킷
{
	int ClientNum;
};
#pragma pack()  //byte지정 원래대로 되돌리기
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//아이콘 모양
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;	//프로세스 함수 호출
	WndClass.lpszClassName = lpszClass;	//클레스 이름
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, SCREENWIDTH, SCREENHEIGHT, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, nCmdShow);


	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == g_sock) ErrorQuit("socket()");

	int retval = WSAAsyncSelect(g_sock, hWnd, WM_SOCKET, FD_CONNECT);   //논블러킹 소켓으로 만들기
	if (SOCKET_ERROR == retval)
	{
		closesocket(g_sock);
		ErrorQuit("WSAAsynSelect()");
	}

	//서버와의 연결을위한 데이터 설정
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	connect(g_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	ULONGLONG check, limt = GetTickCount64();
	int fps = 1000 / 120;
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	GameManager::Get_Instance()->Init(hWnd);

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			check = GetTickCount64();
			if (limt <= check)
			{
				float deltaTime = ((check - limt) + fps) * 0.001f;
				limt = check + fps;

				GameManager::Get_Instance()->Update(deltaTime);
				GameManager::Get_Instance()->Draw();
			}
		}
	}

	WSACleanup();

	GameManager::Get_Instance()->Release(hWnd);


	return (int)msg.wParam;
}
void SocketProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))  //오류 발생여부 확인
	{
		ErrorDisplay(hwnd, WSAGETSELECTERROR(lParam), "ERROR");
		closesocket(g_sock);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		if (SOCKET_ERROR == WSAAsyncSelect(g_sock, hwnd, WM_SOCKET, FD_CLOSE | FD_READ))
		{
			closesocket(g_sock);
			ErrorQuit("WSAAsyncSelect()");
		}
		break;
	case FD_CLOSE:
		ErrorQuit("서버와 연결이 끊어졌습니다.");
		closesocket(g_sock);
		break;
	case FD_READ:
	{
		char buf[sizeof(Play_Packet)];
		ZeroMemory(buf, sizeof(Play_Packet));

		int retval = recv(g_sock, buf, sizeof(BasePacket), 0);
		if (SOCKET_ERROR == retval)
		{
			ErrorDisplay(hwnd, "FD_READ");	// 클라이언트가 READ를 한번더 호출 이유를 못찾음 게임플레이엔 이상이 없음
			return;
		}

		std::string log = "1번째 recv(), retval : " + std::to_string(retval)+"\n";
		OutputDebugStringA(log.c_str());
		BasePacket* pack = (BasePacket*)buf;
		
		switch (pack->id)
		{
		case Packet_ID::play:
		{
			
			int byte = pack->reciveByte - retval;
			retval = recv(g_sock, buf + retval, byte, 0);		//tempBuf에 9BYTE만큼 저장
			log = "2번째 recv(), retval " + std::to_string(retval) + "\n";
			OutputDebugStringA(log.c_str());
			
			Play_Packet* p = (Play_Packet*)(buf);

			POINT point;
			point.x = p->x;
			point.y = p->y;

			GameManager::Get_Instance()->SetTurn(p->turn);
			GameManager::Get_Instance()->ColliderCheck(point);
			break;
		}
		case Packet_ID::user:
		{
			int byte = pack->reciveByte - retval;
			retval = recv(g_sock, buf + retval, byte, 0);		//tempBuf에 4BYTE만큼 저장
			
			User_Packet* p = (User_Packet*)buf;
			std::string Buf = "ClientNum : " + std::to_string(p->ClientNum) + "\n";
			OutputDebugStringA(Buf.c_str());
			GameManager::Get_Instance()->SetClientNumberPlayer(p->ClientNum);
			log =  "2번째 recv(), retval " + std::to_string(retval) + "\n";
			OutputDebugStringA(log.c_str());
			break;
		}
			
		}

		if (SOCKET_ERROR == retval)
		{
			std::cout << 1;
		}
		break;
	}
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_SOCKET:
		SocketProc(hWnd, iMessage, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
	{
			Play_Packet pack;
			pack.x = LOWORD(lParam);
			pack.y = HIWORD(lParam);
			pack.id = Packet_ID::play;
			pack.reciveByte = sizeof(Play_Packet);
			POINT p;
			p.x = pack.x;
			p.y = pack.y;
			pack.turn = GameManager::Get_Instance()->GetTurn();
		if (GameManager::Get_Instance()->IsMyTurn(p))
		{
			send(g_sock, (char*)&pack, sizeof(Play_Packet), 0);
		}
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
	return 0;
}
void ErrorQuit(const char* caption)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, caption, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);

}
void ErrorDisplay(HWND hWnd, const char* caption)
{
	ErrorDisplay(hWnd, WSAGetLastError(), caption);
}
void ErrorDisplay(HWND hWnd, const int& errorCode, const char* caption)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf,
		0, NULL);
	char buf[256];
	ZeroMemory(&buf, sizeof(buf));
	sprintf_s(buf, "[%s] %s\n", caption, (LPCTSTR)lpMsgBuf);
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf, caption, MB_ICONERROR);
	OutputDebugStringA(buf);
	//InvalidateRect(hWnd, NULL, true);
	LocalFree(lpMsgBuf);
}