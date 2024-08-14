#include "MYIOCP.h"


void server::worker_thread() {
	DWORD byteTrans = 0, flags = 0;
	ULONG_PTR key;
	LPOVERLAPPED ol;
	CLIENTSOCKET_OL *cs_ol;

	while (true) {
		GetQueuedCompletionStatus(m_iocp, &byteTrans, &key, &ol, INFINITE);
		if (byteTrans > 0) {
			cs_ol = (CLIENTSOCKET_OL*)key; // same with cs_ol = (CLIENTSOCKET_OL*)ol;

			cout <<"type : " << cs_ol->buf.type << " / buf : " << cs_ol->buf.buf << endl;
		}
		else {
			closesocket(cs_ol->sock);
			delete cs_ol;
		}
		
	}
}

server::server() {
	init_server();
	bind_server("127.0.0.1", 8888);
	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	// workerthread
	for (int i = 0; i < MAX_THREAD_COUNTS; i++) {
		worker_threads.emplace_back(&server::worker_thread, this); // 인자가 필요하면 this 뒤에 사용.
		// 만약 다른 클래스 함수 접근 시 (&클래스명::함수이름, 멤버클래스변수, 인자) 형태로 사용할 수 있슴
	}
	run_server();

	closesocket(lsock);
}

server::server(const char* addr, int port) {
	init_server();
	bind_server(addr, port);
	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	// workerthread
	for (int i = 0; i < MAX_THREAD_COUNTS; i++) {
		worker_threads.emplace_back(&server::worker_thread, this);
	}
	run_server();

	closesocket(lsock);
}

int server::init_server() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return -1;
	}

	lsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (lsock == INVALID_SOCKET) {
		return -1;
	}

	return 0;
}

int server::bind_server(const char* addr, int port) {
	SOCKADDR_IN saddr;

	saddr.sin_family = AF_INET;
	saddr.sin_addr.S_un.S_addr = inet_addr(addr);
	saddr.sin_port = htons(port);

	if (bind(lsock, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}

int server::run_server() {
	if (listen(lsock, SOMAXCONN) == SOCKET_ERROR) {
		return -1;
	}

	SOCKET csock;
	SOCKADDR_IN caddr;
	int caddr_size = sizeof(caddr);

	CLIENTSOCKET_OL *cs_ol;
	WSABUF wsabuf;
	DWORD byterecv = 0, flags = 0;


	while (true) {
		csock = WSAAccept(lsock, (SOCKADDR*)&caddr, &caddr_size, 0, 0);
		if (csock == INVALID_SOCKET) {
			return -1;
		}
		client_list.push_back(csock);

		cs_ol = new CLIENTSOCKET_OL;
		ZeroMemory(cs_ol, sizeof(CLIENTSOCKET_OL));
		cs_ol->sock = csock;
		wsabuf.buf = (char*)&cs_ol->buf;
		wsabuf.len = BUFSIZE;
		CreateIoCompletionPort((HANDLE)csock, m_iocp, (ULONG_PTR)cs_ol, 0);


		WSARecv(csock, &wsabuf, 1, &byterecv, &flags, &cs_ol->ol, 0);
	}
}