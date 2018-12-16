#pragma once

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Protocol.h"
#include "Client.h"
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

#pragma comment(lib,"ws2_32.lib") 

class Server
{
public:
	Server(int PORT, Historia* history);
	int losowa();
	bool ListenForNewConnections();

private:
	static std::vector<Client*> clients;
	WSAData wsaData;
	WORD DllVersion;
	SOCKADDR_IN addr; 	SOCKET sListen;
	int addrLen;
	
	static void HistoryThread();
	void MenuSwitch(int n);
	bool wyslijProtokol(Client * client, Protocol * protocol);
	Historia *history;
	Protocol * odbierzProtokol(Client * client);
	bool przetworz(Client * client, Protocol * protocol);
	static void obslugaKlienta(Client * client);
};

static Server * serverPtr;