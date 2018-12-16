#pragma once
#include <vector>
#include "stdafx.h"

struct obliczenie {
	std::string oper;
	unsigned int SeID;
	unsigned int ObID;
	long float Resu;
	long int Num1;
	long int Num2;
	time_t Czas;
};

class Historia {
private:
	
public:
	std::vector<obliczenie> histWektor;
	void dodajDoHistorii(std::string OP, unsigned int ID, unsigned int OD, long float RS, long int L1, long int L2, time_t TM);
	void wyswietlHistorieID(unsigned int ID);
	void wyswietlHistorieOD(unsigned int OD);
	void wyswietlHistorieALL();
	int ileID(unsigned int ID);
	int czyOD(unsigned int OD, unsigned int ID);
	std::vector<obliczenie> asID(unsigned int ID);
	std::vector<obliczenie> asOD(unsigned int OD);

};