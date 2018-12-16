#include "stdafx.h"
#include "History.h"
#include <vector>
#include <string>
#pragma warning(disable:4996)
#include <time.h>

void retDate(std::string time) {
	const char * tim = time.c_str();
	time_t t = (time_t)strtol(tim, NULL, 10);
	struct tm tm = *localtime(&t);

	printf("%d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}


void Historia::dodajDoHistorii(std::string OP, std::string ID, std::string OD, std::string RS, std::string L1, std::string L2, std::string TM) {
	int la = std::stoi(L1);
	int lb = std::stoi(L2);
	long float res = std::stof(RS);
	obliczenie toAdd;
	toAdd.Czas = TM;
	toAdd.Num1 = la;
	toAdd.Num2 = lb;
	toAdd.ObID = OD;
	toAdd.oper = OP;
	toAdd.Resu = res;
	toAdd.SeID = ID;
	 
	histWektor.push_back(toAdd);
}
void Historia::wyswietlHistorieID(std::string ID) {
	int cnt = 1;
	for (auto i = histWektor.begin(); i != histWektor.end(); i++) {
		if (i->SeID == ID) {
			std::cout << "\n---" << cnt << "--------------\n";
			retDate(i->Czas);
			if (i->oper == "poteguj") std::cout << " POTEGOWANIE " << i->Num1 << "^" << i->Num2 << "=" << i->Resu << ", identyfikator wyniku: " << i->ObID << "\n";
			else if (i->oper == "pomnoz") std::cout << " MNOZENIE " << i->Num1 << "*" << i->Num2 << "=" << i->Resu << ", identyfikator wyniku: " << i->ObID << "\n";
			else if (i->oper == "podziel") std::cout << " DZIELENIE " << i->Num1 << "/" << i->Num2 << "=" << i->Resu << ", identyfikator wyniku: " << i->ObID << "\n";
			else if (i->oper == "logarytmuj") std::cout << " LOGARYTMOWANIE log(" << i->Num1 << "," << i->Num2 << ")=" << i->Resu << ", identyfikator wyniku: " << i->ObID << "\n";
			cnt++;
		}
	}
	std::cout << "\n";
}

void Historia::wyswietlHistorieOD(std::string ID) {
	int cnt = 1;
	for (auto i = histWektor.begin(); i != histWektor.end(); i++) {
		if (i->ObID == ID) {
			std::cout << "\n---" << cnt << "--------------\n";
			retDate(i->Czas);
			if (i->oper == "poteguj") std::cout << " POTEGOWANIE " << i->Num1 << "^" << i->Num2 << "=" << i->Resu << ", identyfikator wyniku: " << i->ObID << "\n";
			else if (i->oper == "pomnoz") std::cout << " MNOZENIE " << i->Num1 << "*" << i->Num2 << "=" << i->Resu << ", identyfikator wyniku: " << i->ObID << "\n";
			else if (i->oper == "podziel") std::cout << " DZIELENIE " << i->Num1 << "/" << i->Num2 << "=" << i->Resu << ", identyfikator wyniku: " << i->ObID << "\n";
			else if (i->oper == "logarytmuj") std::cout << " LOGARYTMOWANIE log(" << i->Num1 << "," << i->Num2 << ")=" << i->Resu << ", identyfikator wyniku: " << i->ObID << "\n";
			cnt++;
		}
	}
	std::cout << "\n";
}
void wyswietlHistorieOD(std::string OD);