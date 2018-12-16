#include "stdafx.h"
#include "Protocol.h"
#include <iostream>
#include <string>
#include <regex>

Protocol::Protocol(std::string operacja, std::string status, int sesId, int obId, float result, int liczba1, int liczba2, time_t time)
{
	OP = operacja;
	ST = status;
	ID = std::to_string(sesId);
	OD = std::to_string(obId);
	RS = std::to_string(result);
	L1 = std::to_string(liczba1);
	L2 = std::to_string(liczba2);
	std::string timeS = std::to_string(time);
	sendThis = "OP=" + OP + "$ST=" + ST + "$ID=" + ID + "$OD=" + OD + "$TM=" + timeS + "$L1=" + L1 + "$L2=" + L2 + "$RS=" + RS +"$";
}

std::string Protocol::doWyslania() {
	return sendThis;
}

std::string Protocol::operacja() {
	return OP;
}

std::string Protocol::val1() {
	return L1;
}

std::string Protocol::val2() {
	return L2;
}

int Protocol::idSesji() {
	return std::stoi(ID);
}

Protocol::Protocol(char* otrzymane) {
	std::string s(otrzymane);
	int j = 0;
	int k = 0;
	int l = 0;
	for (int i = 0; i < 150; i++) {
		if (l == 7 && s[i] == '=') {
			std::string rsresult = "";
			int cnt = j + 5;
			while (s[cnt] != '.' || cnt < 16) {
				rsresult += s[cnt];
				cnt++;
			}
			rsresult += s.substr(cnt, 4);
			if (rsresult[0] == '=') rsresult = rsresult.substr(1, rsresult.size());
			RS = rsresult;
		}
		else if (s[i] == '=') j = i + 1;
		else if (s[i] == '$') {
			k = i - j;
			switch (l) {
			case 0: OP = s.substr(j, k); break;
			case 1: ST = s.substr(j, k); break;
			case 2: ID = s.substr(j, k); break;
			case 3: OD = s.substr(j, k); break;
			case 4: TM = s.substr(j, k); break;
			case 5: L1 = s.substr(j, k); break;
			case 6: L2 = s.substr(j, k); break;
			}
			l++;
		}
	}
}


int Protocol::obliczID() {
	return std::stoi(OD);
}

int Protocol::res() {
	return std::stoi(RS);
}

Protocol::~Protocol()
{

}
