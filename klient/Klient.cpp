#include "stdafx.h"
#include "Klient.h"
#include <iostream>
#include <string>
#pragma warning(disable:4996)

Klient::Klient(std::string IP, int PORT) //tworzenie połączenia
{
	DllVersion = MAKEWORD(2, 2);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}
	
	inet_pton(AF_INET, IP.c_str(), &(adres.sin_addr));
	adres.sin_port = htons(PORT);
	adres.sin_family = AF_INET;
	id = 0ll;
	klientWsk = this;
	polaczony = false;
}

bool Klient::Polacz() //nawiązywanie połączenia
{
	polaczenie = socket(AF_INET, SOCK_STREAM, NULL);
	int kod = connect(polaczenie, (SOCKADDR*)&adres, sizeof(adres));
	if (kod != 0)
	{
		kod = WSAGetLastError();
		std::cout << "Nie mozna polaczyc z serwerem, kod: " << kod; //próba połączenia zwróciła błąd
		return false;
	}

	std::cout << "Polaczono z serwerem!" << std::endl;
	polaczony = true;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	return wyslijProtokol(new Protocol("wyslijId","wyslanoZadanie",0,0,"0","0", time(0),0)); //wysyłanie protokołu
}

bool Klient::zakonczPolaczenie()
{
	wyslijProtokol(new Protocol("rozlaczSie","wyslanoZadanie",this->id,0,"0", "0", time(0),0));
	if (closesocket(polaczenie) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK){
			polaczony = false;
			return true;
		}
		cout << "Wystapil blad: " << WSAGetLastError() << ". #\n";
		return false;
	}
	polaczony = false;
	return true;
}

int Klient::zwrocID()
{
	return id;
}

Protocol * Klient::recvProtocol()
{
	Protocol * pr;
	int rozmiarBufora = 1500;
	char * bufor = new char[rozmiarBufora];
	int check = recv(polaczenie, bufor, rozmiarBufora, NULL);
	if (check == SOCKET_ERROR)
		return nullptr;

	pr = new Protocol(bufor);
	delete[] bufor;
	return pr;
}

bool Klient::wyslijProtokol(Protocol * pr)
{

	std::string str = pr->doWyslania();
	int size = str.size();
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	int check = send(polaczenie, cstr, size, NULL);
	if (check == SOCKET_ERROR)
	{
		delete pr;
		return false;
	}
	delete[] cstr;
	delete pr;
	return true;
}

bool Klient::processProtocol(Protocol * pr) //wybranie dalszych działań na podstawie otrzymanego komunikatu OP
{
	if (pr == nullptr)
		return false;

	std::string OP = pr->operacja();
	int ID = pr->idSesji();
	
	string data = "";

	if (OP == "wyslijId") {
		id = pr->idSesji();
		Sleep(100);
		cout << "Identyfikator sesji: " << id << " \n";
	}

	if (OP == "poteguj" || OP == "logarytmuj" || OP == "pomnoz" || OP == "podziel") {
		pr->zwrocWynik(&history);
	}

	if (OP == "historia" || pr->operacja() == "wysylamHistorie") {
		pr->obslugaHistorii();
	}


	return true;
}

void Klient::ClientThread()
{
	Protocol * protocol;
	while (true)
	{
		protocol = klientWsk->recvProtocol();
		if (protocol == nullptr)
			break;
		if (!klientWsk->processProtocol(protocol))
			break;
		delete protocol;
	}
	klientWsk->zakonczPolaczenie();
	klientWsk->polaczony = false;
}
