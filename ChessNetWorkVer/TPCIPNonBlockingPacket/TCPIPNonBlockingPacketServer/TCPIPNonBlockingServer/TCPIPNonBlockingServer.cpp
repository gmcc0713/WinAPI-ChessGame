#define _WINSOCK_DEPRECATED_NO_WARNINGS		//Winsock2보다 위에 있어야된다.
#pragma comment(lib,"ws2_32.lib")			//윈속 라이브러리 사용을 알린다
#include <WinSock2.h>						//윈속 사용을 위하여 헤더파일 추가.
#include <iostream>

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
struct SOCKETINFO
{
    SOCKET sock;
    Play_Packet* bPack;
};

//listen 소켓을 포함한, 현재 접속한 client의 수
int g_nTotalSockets = 0;
//논블로킹 소켓으로 만든 소켓의 정보
SOCKETINFO* arrSocketInfo[WSA_MAXIMUM_WAIT_EVENTS];

//소켓이 어떤 처리를 할 수 있는지에 대한 이벤트
WSAEVENT arrEvent[WSA_MAXIMUM_WAIT_EVENTS];
//논블로킹 소켓으로 사용할 소켓 정보 추가
bool AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int index);
void err_display(const int& errcode);
void err_display(const char* msg);
void err_quit(const char* msg);
bool AddSocketInfo(SOCKET sock)
{
    if (WSA_MAXIMUM_WAIT_EVENTS <= g_nTotalSockets)
    {
        printf("[오류] 소켓정보를 추가할수 없습니다! \n");
        return false;
    }

    SOCKETINFO* ptr = new SOCKETINFO;
    if (NULL == ptr)
    {
        printf("[오류] 메모리가 부족합니다!\n");
        return false;
    }

    WSAEVENT hEvent = WSACreateEvent();
    if (WSA_INVALID_EVENT == hEvent)
    {
        err_display("WSACreateEvent()");
        return false;
    }

    ptr->sock = sock;
    arrSocketInfo[g_nTotalSockets] = ptr;   //소켓 정보 추가
    arrEvent[g_nTotalSockets] = hEvent; //소켓 이벤트 추가
    g_nTotalSockets++;          //소캣 갯수 추가
    return true;
}

void RemoveSocketInfo(int index)
{
    SOCKETINFO* ptr = arrSocketInfo[index];

    //클라이언트 정보 얻기
    SOCKADDR_IN addr;
    int addlen = sizeof(addr);
    getpeername(ptr->sock, (SOCKADDR*)&addr, &addlen);      //sock로 addr정보 얻기
    printf("[TCP 서버] 클라이언트 종료 : IP 주소 =%s, 포트번호 = %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    closesocket(ptr->sock);
    delete ptr;
    WSACloseEvent(arrEvent[index]);

    for (int i = index + 1; g_nTotalSockets > i; i++)
    {
        arrSocketInfo[i - 1] = arrSocketInfo[i];
        arrSocketInfo[i] = nullptr;

        arrEvent[i - 1] = arrEvent[i];
        arrEvent[i] = 0;

    }
    g_nTotalSockets--;
}
int main()
{
    
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)return -1;

    //socket() listen_socket 생성
    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);     
    if (INVALID_SOCKET == listen_socket) err_quit("socket");

    //소켓 정보 추가
    if (FALSE == AddSocketInfo(listen_socket)) return -1;

    // WSAEventSelect() : 콘솔 프로젝트에서 사용, 해당 소켓을 논브롤킹 소켓으로 만들어 준다.
    // FD_ACCEPT : 클라이언트에서 연결 요청이 왔을 때.
    // FD_CONNECT : connect()를 이용하여 서버에서 연결을 수락 했을 때.
    // FD_CLOSE : 해당 소켓이 연결을 해제 했을 때.
    // FD_READ : 소켓을 읽어 들일 데이터가 있을 때.
    // FD_WRITE : send()할 수 있을 때, 소켓의 버퍼가 비어있을 때.
    // listen_socket은 accept()와 closesocket()을 해야할 경우만 이벤트를 받도록 한다.

    int retval = WSAEventSelect(listen_socket, arrEvent[g_nTotalSockets - 1], FD_ACCEPT | FD_CLOSE);    //소켓을 논블러킹소켓으로 만듬,
    if (SOCKET_ERROR == retval)
    {
        closesocket(listen_socket);
        err_quit("WSAEventSelect()");
    }

    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9000);      //서버 포트번호
    serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);    //자신의 IP

    if (bind(listen_socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
    {
        closesocket(listen_socket);
        WSACleanup();
        err_quit("bind");
    }

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        closesocket(listen_socket);
        WSACleanup();
        err_quit("listen");
    }

    int index;
    WSANETWORKEVENTS NetworkEvents;
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addlen = sizeof(SOCKADDR_IN);
    while (true)
    {
        //이벤트 객체 관찰
        //몇번째 WSAEVENT배열에 이벤트가 발행했는지 index를알수있다.

        index = WSAWaitForMultipleEvents(g_nTotalSockets, arrEvent, FALSE, WSA_INFINITE, FALSE);
        if (WSA_WAIT_FAILED == index)
        {
            err_display("WSAWaitForMultipleEvent()");
            continue;
        }
        //반환값에 WSA_WAIT_EVENT_0을 뺀값이 배열의 index
        index -= WSA_WAIT_EVENT_0;  //이벤트가 발생하는 인덱스

        //구체적인 네트워크 이벤트 알아내기
        //해당 소켓에 발행한 이벤트를 WSANETWORKEVENTS 객체를 통해 알아온다.
        retval = WSAEnumNetworkEvents(arrSocketInfo[index]->sock, arrEvent[index], &NetworkEvents);
        if (SOCKET_ERROR == retval)
        {
            err_display("WSAEnumNetworkEvents()");
            continue;
        }
        //FD_ACCEPT 이벤트 처리
        if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
        {
            //FD_ACCEPT 이벤트에 발생한 오류를 확인
            if (0 != NetworkEvents.iErrorCode[FD_ACCEPT_BIT])
            {
                err_display(NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
                continue;
            }

            //accept() 연결대기
            client_sock = accept(arrSocketInfo[index]->sock, (SOCKADDR*)&clientaddr, &addlen);
            if (INVALID_SOCKET == client_sock)
            {

                err_display("accept()");
                continue;
            }
            printf("[TCP 서버] 클라이언트 접속 : IP주소 =%s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));;
            
            
            //접속 수 제한
            if (WSA_MAXIMUM_WAIT_EVENTS <= g_nTotalSockets)
            {
                printf("[오류] 더 이상 접속을 받아들일수 없습니다!\n");
                closesocket(client_sock);
                continue;
            }

            //클라이언트 소켓 정보 추가
            if (!AddSocketInfo(client_sock))continue;

            User_Packet* user_Packet = new User_Packet;
            user_Packet->ClientNum = g_nTotalSockets - 2;             //socket갯수 - listen 소켓 포함이라 -1, 시작인덱스가 0이라라 -1
            user_Packet->id = Packet_ID::user;
            user_Packet->reciveByte = sizeof(User_Packet);
            retval = send(arrSocketInfo[g_nTotalSockets-1]->sock, (char*)user_Packet, sizeof(User_Packet), 0);
            printf("%d번째 클라이언트 Send() : , retval : %d,clientNum : %d\n", g_nTotalSockets-1, retval, user_Packet->ClientNum);
           
            delete user_Packet;
            user_Packet = nullptr;

            //WSAEventSelect()
            //client_sock은 클라이언트에서 send()하여 recv()해야할 경우와 클라이언트가 종료되어 closesocket()을 해야할 경우만 이벤트를 받도록 한다.
            
            retval = WSAEventSelect(client_sock, arrEvent[g_nTotalSockets - 1], FD_READ | FD_CLOSE);    //read가 send이고 write가 recv
            if (SOCKET_ERROR == retval) err_quit("WSAEventSelect()");

        }//FD_ACCEPT 이벤트 처리


        //FD_READ 이벤트 처리
        if (NetworkEvents.lNetworkEvents & FD_READ)
        {
            if (0 != NetworkEvents.iErrorCode[FD_READ_BIT])
            {
                err_display(NetworkEvents.iErrorCode[FD_READ_BIT]);
                continue;
            }

            char buf[sizeof(Play_Packet)];
            ZeroMemory(buf, sizeof(Play_Packet));
            SOCKETINFO* ptr = arrSocketInfo[index];
            retval = recv(ptr->sock, buf, sizeof(buf), 0);

            if (SOCKET_ERROR == retval)
            {
                if (WSAEWOULDBLOCK != WSAGetLastError())
                {
                    err_display("recv()");
                    RemoveSocketInfo(index);
                }
                continue;
            }

            ptr->bPack = (Play_Packet*)buf;

            //ptr->buf[retval] = '\0';
            getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addlen); 
            printf("[TCP/%s : %d] x : %d, y : %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->bPack->x, ptr->bPack->y);

            int sendbyte = sizeof(Play_Packet);
            
            /*ptr->bPack->id = Packet_ID::play;
            ptr->bPack->reciveByte = sizeof(Play_Packet);*/
            for (int i = 1; i < g_nTotalSockets; i++)
            {

                retval = send(arrSocketInfo[i]->sock, (char*)ptr->bPack, sendbyte, 0);
                printf("%d번째 클라이언트 Send() : , retval : %d\n",i ,retval);

                if (SOCKET_ERROR == retval)
                {
                    if (WSAEWOULDBLOCK != WSAGetLastError())
                    {
                        err_display("send()");
                        RemoveSocketInfo(index);
                    }
                    continue;
                }
            }
        }//FD_READ 이벤트
        
        //FD_CLOSE 이벤트 처리
        if (NetworkEvents.lNetworkEvents & FD_CLOSE)
        {
            if (0 != NetworkEvents.iErrorCode[FD_CLOSE_BIT])
            {
                err_display(NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
            }
            RemoveSocketInfo(index);
        }

    }
    
    closesocket(listen_socket);
    
    //윈속 사용이 끝났다
    WSACleanup();
    return 0;
    
}
void err_display(const int& errcode)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[오류] %s", (LPCTSTR)lpMsgBuf);
    LocalFree(lpMsgBuf);
}
void err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
    LocalFree(lpMsgBuf);
}
void err_quit(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);

    MessageBox(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);

    LocalFree(lpMsgBuf);
    exit(-1);
}