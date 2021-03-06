#include "Klient.h"
#include <string>
#include "stdafx.h"
#include <time.h>
#include <string> 




int main()
{
	Klient client("192.168.1.13", 62081);
	if (!client.Polacz())
	{
		
		std::cout << "Nie polaczono z serwerem." << std::endl;
		getchar();
		return 0;
	}
	bool program = true;
	std::string bufor;
	char wybor;
	cout <<
		" 1 - potegowanie,\n" <<
		" 2 - logarytmowanie, \n" <<
		" 3 - mnozenie, \n" <<
		" 4 - dzielenie, \n" <<
		" 5 - przegladaj historie, \n";
		" 6 - zakoncz polaczenie \n";

	while (program)
	{
		Sleep(10);
		getline(cin, bufor);
		wybor = bufor[0];
		switch (wybor)
		{
		case '1':
		{
			if (client.polaczony == false)
			{
				cout << "Nie jestes podlaczony do sewera.\n";
				break;
			}
			std::string a, b;
				std::cout << "Wprowadz liczbe: ";
				std::cin >> a;
				std::cout << "Wprowadz potege: ";
				std::cin >> b;
			client.wyslijProtokol(new Protocol("poteguj", "wyslanoZadanie", client.zwrocID(), 0, a, b, time(0),0));
			
			break;
		}
		case '2':
		{
			if (client.polaczony == false)
			{
				cout << "Nie jestes podlaczony do sewera.\n";
				break;
			}
			std::string a, b;
			std::cout << "Wprowadz liczbe: ";
			std::cin >> a;
			std::cout << "Wprowadz podstawe: ";
			std::cin >> b;
			client.wyslijProtokol(new Protocol("logarytmuj", "wyslanoZadanie", client.zwrocID(), 0, a, b, time(0),0));

			break;
		}
		case '3':
		{
			if (client.polaczony == false)
			{
				cout << "Nie jestes podlaczony do sewera.\n";
				break;
			}
			std::string a, b;
			std::cout << "Wprowadz 1. wartosc: ";
			std::cin >> a;
			std::cout << "Wprowadz 2. wartosc: ";
			std::cin >> b;
			client.wyslijProtokol(new Protocol("pomnoz", "wyslanoZadanie", client.zwrocID(), 0, a, b, time(0),0));
			break;
		}
		case '4':
		{
			if (client.polaczony == false)
			{
				cout << "Nie jestes podlaczony do sewera.\n";
				break;
			}
			std::string a, b;

				std::cout << "Wprowadz 1. wartosc: ";
				std::cin >> a;
				std::cout << "Wprowadz 2. wartosc: ";
				std::cin >> b;

			client.wyslijProtokol(new Protocol("podziel", "wyslanoZadanie", client.zwrocID(), 0, a, b, time(0),0));
			break;
		}
		case '5':
		{
				cout <<
					" 1 - na podstawie id sesji,\n" <<
					" 2 - na podstawie id obliczen\n" <<
				" 0 - powrot\n";

				getline(cin, bufor);
				wybor = bufor[0];
				switch (wybor) {
				case '1': {
					std::cout << "Wprowadz id sesji (obecna sesja to " << client.zwrocID() <<"): ";
					getline(cin, bufor);
					client.wyslijProtokol(new Protocol("historia", "wyslanoZadanie", client.zwrocID(), std::stoi(bufor), "0", "0", time(0),0));
					break; }
				case '2': {
					std::cout << "Wprowadz id obliczen: ";
					getline(cin, bufor);
					client.wyslijProtokol(new Protocol("historia", "wyslanoZadanie", client.zwrocID(), 0, "0", "0", time(0), std::stoi(bufor)));
					break; }
				case '0': break;
				}
		}
		case '6':
		{
			client.wyslijProtokol(new Protocol("rozlaczSie", "wyslanoZadanie", client.zwrocID(), 0, 0, 0, time(0), 0));
			std::cout << "Rozlaczono z serwerem ";
			break;
		}
		default:
		{
			cout <<
				" 1 - potegowanie,\n" <<
				" 2 - logarytmowanie, \n" <<
				" 3 - mnozenie, \n" <<
				" 4 - dzielenie, \n" <<
				" 5 - przegladaj historie, \n";
				" 6 - zakoncz polaczenie \n";
		}
		}
		Sleep(10);
	}
	return EXIT_SUCCESS;
}

