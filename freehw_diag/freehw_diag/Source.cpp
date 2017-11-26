#define _WIN32_WINNT 0x500
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <time.h>


using namespace std;
void main()
{
	time_t time,tStart;

	bool run = false; //zmienna okreœla stan czy ju¿ znaleziono Hauptwerk.exe wœród listy dzia³aj¹cych procesów
	FILE *plik; //handler dla odczytu pliku ini
	int zmienne[2] = { 0 }, x = 0, x1, y1; //zmienne pomocnicze przy odczycie z pliku

	fopen_s(&plik, "freehw_conf.ini", "r"); //czytanie pliku
	if (plik == NULL) //jeœli nie ma pliku ini lub inny b³¹d
	{
		x1 = 425;
		y1 = 470;
	}
	else {
		for (int i = 0; i<2; i++) //jeœli odczyt poprawny, zapisujemy wartoœci z pliku do zmiennych
		{
			fscanf_s(plik, "%d", &x);
			zmienne[i] = x;
		}
		fclose(plik);

		x1 = zmienne[0];
		y1 = zmienne[1];
	}
	tStart = clock();
	while (run == false) //jeœli zmienna run jest false czyli nie znaleziono jeszcze procesu HW to powtarzamy
	{
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE) //do tego miejsca zapisywany jest snapshot procesów
			{
				if (_stricmp(entry.szExeFile, "Hauptwerk.exe") == 0) //jeœli znaleziono proces to true, odliczamy do klikania
				{
					HWND window = GetDesktopWindow(); //pobieranie uchwytu ca³oœci widzianego pulpitu
					RECT rectangle; //deklaracja zmiennej pod GetWindowRect
					GetWindowRect(window, &rectangle); //u¿ycie funkcji GetWindowRect

					int x_roz = rectangle.right - rectangle.left;
					int y_roz = rectangle.bottom - rectangle.top;

					HWND okno = FindWindowA(0, "Welcome to Hauptwerk"); // handler dla okna
					RECT prostokat;
					GetWindowRect(okno, &prostokat); //zapisuje w prostokat parametry okna
					int szerokosc = prostokat.left; //odleg³oœæ okna od lewej krawêdzi ekranu
					int wysokosc = prostokat.top; //odleglosc okna od góry ekranu
				    cout << "X: " << x_roz << " Y: " << y_roz << endl;
					cout << "X: " << szerokosc << " Y: " << wysokosc << endl;
					while(abs(szerokosc) > x_roz || abs(wysokosc) > y_roz || szerokosc==0 || wysokosc==0)
					{
						okno = FindWindowA(0, "Welcome to Hauptwerk");
						GetWindowRect(okno, &prostokat); //zapisuje w prostokat parametry okna
						szerokosc = prostokat.left; //odleg³oœæ okna od lewej krawêdzi ekranu
						wysokosc = prostokat.top; //odleglosc okna od góry ekranu
						
						cout << "X: " << szerokosc << " Y: " << wysokosc << endl;
						
						
						Sleep(100);
					}
					time = (clock() - tStart);
					okno = FindWindowA(0, "Welcome to Hauptwerk");
					GetWindowRect(okno, &prostokat); //zapisuje w prostokat parametry okna
					szerokosc = prostokat.left; //odleg³oœæ okna od lewej krawêdzi ekranu
					wysokosc = prostokat.top; //odleglosc okna od góry ekranu
					cout << "Czas wyniosl: " << 1000. / CLOCKS_PER_SEC*time << " ms" << endl;
					cout << "Przycisk:: X: " << szerokosc + x1 << " Y: " << wysokosc + y1 << endl;

					run = true; //proces znaleziony, zmienna zmienia wartoœæ
					break; //wychodzimy z pêtli while (Process32Next...)
				}
			}
		}
		CloseHandle(snapshot);
		Sleep(100); //opóŸnienie aby nie robiæ snapshotów procesów non-stop, wtedy proces obci¹¿a procesor 25%
	}

	system("pause");
}