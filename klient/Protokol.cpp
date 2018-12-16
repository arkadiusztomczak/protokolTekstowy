#include "stdafx.h"
#include "Protokol.h"
#include <iostream>
#include <string>
#pragma warning(disable:4996)


void zwrocDate(std::string time) {
	const char * tim = time.c_str();
	time_t t = (time_t)strtol(tim, NULL, 10);
	struct tm tm = *localtime(&t);

	printf("%d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

Protocol::Protocol(std::string operacja, std::string status, int sesId, float result, std::string liczba1, std::string liczba2, time_t time, int od)
{

	OP = operacja;
	ST = status;
	ID = std::to_string(sesId);
	RS = std::to_string(result);
	std::string timeS = std::to_string(time);
	OD = std::to_string(od);
	sendThis = "OP=" + OP + "$ST=" + ST + "$ID=" + ID + "$OD=" + OD + "$TM=" + timeS +"$L1=" + liczba1 + "$L2=" + liczba2 + "$RS=" + RS + "$"; //Tworzenie komunikatu
}

std::string Protocol::doWyslania() {
	return sendThis;
}

std::string Protocol::operacja() {
	return OP;
}

int Protocol::idSesji() {
	return std::stoi(ID);
}

Protocol::Protocol(char* otrzymane) { //wyłuskiwanie danych z otrzymanego komunikatu
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
			int k = 0;
			while (!isdigit(rsresult[k])) k++;
			rsresult = rsresult.substr(k, rsresult.size());
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

Protocol::~Protocol()
{

}

void showDate(std::string time) {
	const char * tim = time.c_str();
	time_t t = (time_t)strtol(tim, NULL, 10);
	struct tm tm = *localtime(&t);

	printf("%d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void Protocol::zwrocWynik(Historia* h) {
	std::cout << "\n";
	if (ST == "bladObliczen") {
		std::cout << "BLAD OBLICZEN: ";
		int code = std::stoi(RS);
		if (code == 1) std::cout << "Dzielenie przez 0!";
		else if (code == 2) std::cout << "Podstawa logarytmu mniejsza lub rowna 0!";
		else if (code == 3) std::cout << "Wartosc logarytmu mniejsza lub rowna 0!";
		else if (code == 4) std::cout << "Podstawa logarytmu rowna 1!";
		else if (code == 7) std::cout << "Przekroczono wartosc zmiennej!";
		else std::cout << code;
	}
	else {
		if (OP == "poteguj") std::cout << "Wynik dzialania " << std::stoi(L1) << "^" << std::stoi(L2) << " to " << std::stof(RS) << "\nIdentyfikator wyniku: " << OD << "\n";
		else if (OP == "logarytmuj") std::cout << "Wynik dzialania log(" << std::stoi(L1) << "," << std::stoi(L2) << ") to " << std::stof(RS) << "\nIdentyfikator wyniku: " << OD << "\n";
		else if (OP == "pomnoz") std::cout << "Wynik dzialania " << std::stoi(L1) << "*" << std::stoi(L2) << " to " << std::stof(RS) << "\nIdentyfikator wyniku: " << OD << "\n";
		else if (OP == "podziel") std::cout << "Wynik dzialania " << std::stoi(L1) << "/" << std::stoi(L2) << " to " << std::stof(RS) << "\nIdentyfikator wyniku: " << OD << "\n";
		else std::cout << "Nie rozpoznano operacji '" << OP << "'!";

		h->dodajDoHistorii(OP, ID, OD, RS, L1, L2, TM);
	}
	std::cout << "Otrzymano:";
	showDate(TM);
	std::cout << "\n\n";
}

void Protocol::obslugaHistorii() {
	if (OP == "historia" && ST == "bladObliczen") {
		int code = std::stoi(RS);
		if (code == 5) std::cout << "Nie masz uprawnien do wskazanych obliczen!\n";
		else if (code == 6) std::cout << "Nie odnaleziono obliczen!\n";
	}

	else if (ST == "wysylamHistorie") {
		int cnt = 1;
				zwrocDate(TM);
				if (OP == "poteguj") std::cout << " POTEGOWANIE " << L1 << "^" << L2 << "=" << RS << ", identyfikator wyniku: " << OD << "\n";
				else if (OP == "pomnoz") std::cout << " MNOZENIE " << L1 << "*" << L2 << "=" << RS << ", identyfikator wyniku: " << OD << "\n";
				else if (OP == "podziel") std::cout << " DZIELENIE " << L1 << "/" << L2 << "=" << RS << ", identyfikator wyniku: " << OD << "\n";
				else if (OP == "logarytmuj") std::cout << " LOGARYTMOWANIE log(" << L1 << "," << L2 << ")=" << RS << ", identyfikator wyniku: " << OD << "\n";
	}
}
