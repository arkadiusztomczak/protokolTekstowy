#include "stdafx.h"
#include "Server.h"
#include <regex>
#include <time.h>
#include "History.h"
#include <thread>

void menuSwitcher(Historia* history) {

	int a;
	while (true) {
		std::cout <<
			"1 - sprawdz historie przez ID sesji\n" <<
			"2 - sprawdz historie przez ID obliczen\n" <<
			"3 - wyswietl cala historie\n";
		std::cin >> a;
		switch (a) {
		case 1: {
			int b;
			std::cout << "Wprowadz ID: ";
			std::cin >> b;
			history->wyswietlHistorieID(b);
			break;
		}
		case 2: {
			int b;
			std::cout << "Wprowadz ID: ";
			std::cin >> b;
			history->wyswietlHistorieOD(b);
			break;
		}
		case 3: {
			history->wyswietlHistorieALL();
			break;
		}

		}
	}
}

int main()
{
	Historia history;
	Server myServer(62081, 1, &history);
	

	std::thread t(&menuSwitcher, &history);

	for (int i = 0; i < 100; i++)
	{
		myServer.ListenForNewConnections();
	}
	return 0;
}