// CPPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>

using namespace std;

struct PACKET_STRUCT {
    int type;
    char data[256];
};

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET lsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.S_un.S_addr = ntohl(INADDR_ANY);
    saddr.sin_port = ntohs(8888);

    bind(lsock, (SOCKADDR*)&saddr, sizeof(saddr));

    listen(lsock, SOMAXCONN);

    SOCKET csock;
    SOCKADDR_IN caddr;
    int size_caddr = sizeof(caddr);

    while (1) {
        csock = accept(lsock, (SOCKADDR*)&caddr, &size_caddr);
        cout << "new client accepted." << endl;
        PACKET_STRUCT* ps = new PACKET_STRUCT;
        int byte;
        while (1) {
            ZeroMemory(ps, sizeof(PACKET_STRUCT));
            byte = recv(csock, (char*)ps, sizeof(PACKET_STRUCT), 0);
            cout << "type : " << ps->type << ", data : " << ps->data << endl;

            getchar();
            /*if (byte < 1) {
                break;
            }
            if (ps->type == 11) {
                cout << "11 : " << ps->data << endl;
            }
            else {
                cout << "22 : " << ps->data << endl;
            }
            
            
            send(csock, (char*)ps, sizeof(PACKET_STRUCT), 0);*/
        }
        cout << "client disconnected." << endl;
        closesocket(csock);
    }


    closesocket(lsock);
    WSACleanup();
}
