#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#include "Protokol.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib,"ws2_32.lib") 

class Klient
{
public:
	Klient(std::string IP, int PORT);
	bool Polacz();
	bool zakonczPolaczenie();
	int zwrocID();
	bool wyslijProtokol(Protocol * protocol);
	bool polaczony;
	SOCKET polaczenie;
	Historia history;
private:
	WSAData wsaData;
	WORD DllVersion;
	SOCKADDR_IN adres;
	u_int64 id;
	Protocol * recvProtocol();
	bool processProtocol(Protocol * protocol);
	static void ClientThread();
};

static Klient * klientWsk;

