#include "stdafx.h"
#include "Server.h"
#include <iostream>
#include <string>
#include <thread>
#pragma warning(disable:4996)

std::vector<Client*> Server::clients;



Server::Server(int PORT, Historia* his) //inicjacja serwera
{
	history = his;

	DllVersion = MAKEWORD(2, 2);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}


	addrLen = sizeof(addr);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;


	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	if (::bind(sListen, (SOCKADDR*)&addr, addrLen) == SOCKET_ERROR)
	{
		std::cout << "Blad bindowania: " << WSAGetLastError() <<".\n";
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Blad nasluchiwania: " << WSAGetLastError() << ".\n";
	}
	serverPtr = this;
	
}

int Server::losowa() {
	std::default_random_engine engine(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distro(50, 700);
	return distro(engine);
}

bool Server::ListenForNewConnections() //nasluchuuje nowych polaczen oraz tworzy watek klienta
{
	
	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrLen);
	if (newConnection == 0)
	{
		std::cout << "Blad przy akceptacji klienta: "<< WSAGetLastError() <<".\n";
		return false;
	}
	else
	{
		

		Client* newClient = new Client(newConnection,losowa());
		std::cout << "Klient polaczony!" << std::endl;
		clients.push_back(newClient);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)obslugaKlienta, (LPVOID)newClient, NULL, NULL);
		return true;
	}
}

bool Server::przetworz(Client * client, Protocol * protocol) //przetwarza otrzymany protokól, a takze wykonuje zadane operacje
{
	if (protocol == nullptr)
		return false;


	std::string OP = protocol->operacja();
	int ID = protocol->idSesji();
	int opId = losowa();

	int L1, L2;
	int err = 0;

	try {
		L1 = std::stoi(protocol->val1());
	}
	catch (std::invalid_argument& e) {
		time_t t = time(0);
		wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 8, 0, 0, t));
		err = 1;
	}
	catch (std::out_of_range& e) {
		time_t t = time(0);
		wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 7, 0, 0, t));
		err = 1;
	}
	
	try {
		L2 = std::stoi(protocol->val2());
	}
	catch (std::invalid_argument& e) {
		time_t t = time(0);
		wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 8, 0, 0, t));
		err = 1;
	}
	catch (std::out_of_range& e) {
		time_t t = time(0);
		wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 7, 0, 0, t));
		err = 1;
	}
	if (err == 0) {
		if (OP == "wyslijId") {
			opId = 0;
			wyslijProtokol(client, new Protocol("wyslijId", "IdWyslane", client->sessionID, opId, 0, 0, 0, time(0)));
			std::cout << "Wyslano klientowi ID sesji: " << client->sessionID << ".\n";
		}

		else if (OP == "rozlaczSie") {
			std::cout << "Klient o ID sesji: " << client->sessionID << " rozlaczyl sie.\n";
			closesocket(client->clientSocket);  //by³o skomentowane
		}

		else if (OP == "poteguj") {
			time_t t = time(0);
			double val = std::pow(L1, L2);
			if (val == HUGE_VAL) {
				wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 7, L1, L2, t));
			}
			else {
				float result = pow(L1, L2);
				wyslijProtokol(client, new Protocol(OP, "wynikWyslany", client->sessionID, opId, result, L1, L2, t));
				history->dodajDoHistorii(OP, client->sessionID, opId, result, L1, L2, t);
			}
		}

		else if (OP == "logarytmuj") {
			time_t t = time(0);
			if (L2 <= 0)
				wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 2, L1, L2, t));
			else if (L1 <= 0)
				wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 3, L1, L2, t));
			else if (L2 == 1)
				wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 4, L1, L2, t));
			else {
				float result = (log(L2) / log(L1));
				wyslijProtokol(client, new Protocol(OP, "wynikWyslany", client->sessionID, opId, result, L1, L2, t));
				history->dodajDoHistorii(OP, client->sessionID, opId, result, L1, L2, t);
			}
		}

		else if (OP == "pomnoz") {
			time_t t = time(0);
			float x = L1 * L2;
			if (L1 > 2147483647 || L2 > 2147483647 || L1 != 0 && x / L1 != L2) wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 7, L1, L2, t));
			else {
				float result = (L2) * (L1);
				wyslijProtokol(client, new Protocol(OP, "wynikWyslany", client->sessionID, opId, result, L1, L2, t));
				history->dodajDoHistorii(OP, client->sessionID, opId, result, L1, L2, t);
			}
		}

		else if (OP == "podziel") {
			time_t t = time(0);
			if (L2 == 0)
				wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 1, L1, L2, t));

			else {
				float result = ((L1) / (L2));
				wyslijProtokol(client, new Protocol(OP, "wynikWyslany", client->sessionID, opId, result, L1, L2, t));
				history->dodajDoHistorii(OP, client->sessionID, opId, result, L1, L2, t);

			}
		}

		else if (OP == "historia") {
			time_t t = time(0);
			int id = protocol->idSesji();
			int rs = protocol->res();
			if (protocol->obliczID() == 0) {
				if (id != rs)
					wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 5, L1, L2, t));
				else if (history->ileID(protocol->idSesji()) == 0)
					wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 6, L1, L2, t));
				else {
					std::vector<obliczenie> res = history->asID(protocol->idSesji());
					for (auto a = res.begin(); a != res.end(); a++) {
						wyslijProtokol(client, new Protocol(a->oper, "wysylamHistorie", a->SeID, a->ObID, a->Resu, a->Num1, a->Num2, a->Czas));
						Sleep(50);
					}
				}
			}

			else {
				if (history->czyOD(protocol->obliczID(), protocol->idSesji()) == 1)
					wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 5, L1, L2, t));
				else if ((history->czyOD(protocol->obliczID(), protocol->idSesji())) == 0)
					wyslijProtokol(client, new Protocol(OP, "bladObliczen", client->sessionID, opId, 6, L1, L2, t));
				else {
					std::vector<obliczenie> res = history->asOD(protocol->obliczID());
					for (auto a = res.begin(); a != res.end(); a++) {
						wyslijProtokol(client, new Protocol(a->oper, "wysylamHistorie", a->SeID, a->ObID, 1, a->Num1, a->Num2, a->Czas));
					}
				}
			}
		}
		std::cout << "Zrealizowalem operacje " << OP << " (id: " << opId << ")\n";
	}
}

bool Server::wyslijProtokol(Client * client, Protocol * protocol)
{
	std::string str = protocol->doWyslania();
	int size = str.size();
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	
	int check = send(client->clientSocket, cstr, size, NULL);
	delete[] cstr;
	if (check == SOCKET_ERROR)
		return false;
	return true;
}

Protocol *  Server::odbierzProtokol(Client * client)
{
	Protocol * protocol;
	int bufferSize = 1500;
	char * buffer = new char[bufferSize];
	int check = recv(client->clientSocket, buffer, bufferSize, NULL);
	if (check == SOCKET_ERROR || check == 0)
		return nullptr;
	protocol = new Protocol(buffer);

	delete[] buffer;

	return protocol;
}

void Server::obslugaKlienta(Client * client)
{
	Protocol * protocol;
	while (true)
	{
		protocol = serverPtr->odbierzProtokol(client);
		if (protocol == nullptr)
			break;
		if (!serverPtr->przetworz(client, protocol))
			break; 
	}
	std::cout << "Rozlaczono z klientem o ID sesji: " << client->sessionID << "." << std::endl;
	
	if(client->clientSocket != INVALID_SOCKET)
		closesocket(client->clientSocket);
	clients.erase(remove(clients.begin(),clients.end(),client),clients.end());
	delete client;
}