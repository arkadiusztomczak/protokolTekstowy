#include "stdafx.h"
#include "History.h"
#include <vector>
#include <string>
#pragma warning(disable:4996)
#include <time.h>

void retDate(time_t t) {
	struct tm tm = *localtime(&t);

	printf("%d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}


void Historia::dodajDoHistorii(std::string OP, unsigned int ID, unsigned int OD, long float RS, long int L1, long int L2, time_t TM) {
	obliczenie toAdd;
	toAdd.Czas = TM;
	toAdd.Num1 = L1;
	toAdd.Num2 = L2;
	toAdd.ObID = OD;
	toAdd.oper = OP;
	toAdd.Resu = RS;
	toAdd.SeID = ID;

	histWektor.push_back(toAdd); //dodaje element do historii
}

int Historia::ileID(unsigned int ID) { //zwraca informację o ilości elementów w historii
	int cnt = 0;
	for (auto i = histWektor.begin(); i != histWektor.end(); i++) {
		if (i->SeID == ID) cnt++;
	}
	return cnt;
}

int Historia::czyOD(unsigned int OD, unsigned int ID) { //sprawdza czy serwer posiada w historii identyfikator obliczenia zgodny z identyfikatorem sesji użytkownika. 2 - jeśli tak, 1 - jeśli posiada, ale ID użytkownika jest niezgodne, 0 jeśli nie posiada
	for (auto i = histWektor.begin(); i != histWektor.end(); i++) {
		if (i->ObID == OD && i->SeID == ID) return 2;
		else if (i->ObID == OD) return 1;
	}
	return 0;
}

std::vector<obliczenie> Historia::asID(unsigned int ID) { //przygotowuje wektor z historią obliczeń danego użytkownika
	std::vector<obliczenie> result;
	for (auto i = histWektor.begin(); i != histWektor.end(); i++) {
		if (i->SeID == ID) {
			obliczenie inserter;
			inserter.Czas = i->Czas;
			inserter.Num1 = i->Num1;
			inserter.Num2 = i->Num2;
			inserter.ObID = i->ObID;
			inserter.oper = i->oper;
			inserter.Resu = i->Resu;
			inserter.SeID = i->SeID;
			result.push_back(inserter);
		}
	}
	return result;
}

std::vector<obliczenie> Historia::asOD(unsigned int OD) { //przygotowuje wektor z historią obliczeń dla danego id obliczeń
	std::vector<obliczenie> result;
	for (auto i = histWektor.begin(); i != histWektor.end(); i++) {
		if (i->ObID == OD) {
			obliczenie inserter;
			inserter.Czas = i->Czas;
			inserter.Num1 = i->Num1;
			inserter.Num2 = i->Num2;
			inserter.ObID = i->ObID;
			inserter.oper = i->oper;
			inserter.Resu = i->Resu;
			inserter.SeID = i->SeID;
			result.push_back(inserter);
		}
	}
	return result;
}
void Historia::wyswietlHistorieID(unsigned int ID) { //wyświetla (po stronie serwera) historię dla danej sesji
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

void Historia::wyswietlHistorieOD(unsigned int ID) { //wyświetla (po stronie serwera) historię dla danego id obliczeń
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
void Historia::wyswietlHistorieALL() { //wyświetla (po stronie serwera) całą historię wszystkich obliczeń
	int cnt = 1;

	for (auto i = 0; i < histWektor.size(); i++) {
			std::cout << "\n---" << cnt << "--------------\n";
			retDate(histWektor[i].Czas);
			if (histWektor[i].oper == "poteguj") std::cout << " POTEGOWANIE " << histWektor[i].Num1 << "^" << histWektor[i].Num2 << "=" << histWektor[i].Resu << ", identyfikator wyniku: " << histWektor[i].ObID << "\n";
			else if (histWektor[i].oper == "pomnoz") std::cout << " MNOZENIE " << histWektor[i].Num1 << "*" << histWektor[i].Num2 << "=" << histWektor[i].Resu << ", identyfikator wyniku: " << histWektor[i].ObID << "\n";
			else if (histWektor[i].oper == "podziel") std::cout << " DZIELENIE " << histWektor[i].Num1 << "/" << histWektor[i].Num2 << "=" << histWektor[i].Resu << ", identyfikator wyniku: " << histWektor[i].ObID << "\n";
			else if (histWektor[i].oper == "logarytmuj") std::cout << " LOGARYTMOWANIE log(" << histWektor[i].Num1 << "," << histWektor[i].Num2 << ")=" << histWektor[i].Resu << ", identyfikator wyniku: " << histWektor[i].ObID << "\n";
			cnt++;
	}
	std::cout << "\n";
}
