#pragma once
#include <vector>
#include "stdafx.h"

struct obliczenie {
	std::string oper;
	std::string SeID;
	std::string ObID;
	long float Resu;
	long int Num1;
	long int Num2;
	std::string Czas;
};

class Historia {
private:
	std::vector<obliczenie> histWektor;
public:
	void dodajDoHistorii(std::string OP, std::string ID, std::string OD, std::string RS, std::string L1, std::string L2, std::string TM);
	void wyswietlHistorieID(std::string ID);
	void wyswietlHistorieOD(std::string OD);

};