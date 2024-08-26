#pragma once

#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <list>
#include <thread>
#include <string>

using namespace std;

#define MAX_THREAD_COUNTS (4)
#define BUFSIZE (256)

class server {
	SOCKET lsock;
	HANDLE m_iocp;
	list<thread> worker_threads;
	list<SOCKET> client_list;
public:
	server();
	server(const char* addr, int port);

	~server() {

	}
	//threaproc - 
	// by packetheader recv & send to all 

	int init_server();

	int bind_server(const char* addr, int port);

	int run_server();

	void close_server();

	void worker_thread();

	void send_all(mypacket& buf, SOCKET sender);

};

typedef struct mypacket {
	int type;
	char buf[256];
};

typedef struct CLIENTSOCKET_OL {
	OVERLAPPED ol;
	SOCKET sock;
	mypacket buf;
};

typedef enum packettype {
	TYPE_CHAR = 1001,
	TYPE_POS = 1002,
};

 