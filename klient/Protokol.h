#pragma once
#include "stdafx.h"
#include "History.h"
#include <WinSock2.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib") 

class Protocol
{
private:
	std::string sendThis;
	std::string OP;
	std::string ST;
	std::string ID;
	std::string OD;
	std::string RS;
	std::string L1;
	std::string L2;
	std::string TM;
	time_t time;
	
public:
	Protocol(std::string operacja, std::string status, int sesId, float result, std::string liczba1, std::string liczba2, time_t time, int od);
	Protocol(char* otrzymane);
	std::string doWyslania();
	std::string operacja();
	void zwrocWynik(Historia* h);
	int idSesji();
	int idOperacji();
	void obslugaHistorii();
	~Protocol();
};

