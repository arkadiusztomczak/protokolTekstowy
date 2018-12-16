#pragma once
#include "stdafx.h"
#include "History.h"
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib") 

class Protocol
{
private:
	std::string sendThis;
	std::string OP;
	std::string ST;
	std::string ID;
	std::string OD;
	std::string L1;
	std::string L2;
	std::string RS;
	std::string TM;
	time_t time;
public:
	Protocol(std::string operacja, std::string status, int sesId, int obId, float result, int liczba1, int liczba2, time_t time);
	Protocol(char* otrzymane);
	std::string doWyslania();
	std::string operacja();
	int idSesji();
	std::string val1();
	std::string val2();
	int res();
	int obliczID();
	~Protocol();
};

