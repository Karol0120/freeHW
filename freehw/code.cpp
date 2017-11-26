#define _WIN32_WINNT 0x500
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

void kliknij(int x1, int y1)
{
	SetCursorPos(x1, y1); //przemieszczenie kursora nad punkt
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //pojedyncze klikni�cie nad tym punktem
}
void main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE); //te 2 linie ukrywaj� okno programu exe, jest niewidoczne gdy dzia�a

	bool run = false; //zmienna okre�la stan czy ju� znaleziono Hauptwerk.exe w�r�d listy dzia�aj�cych proces�w
	FILE *plik; //handler dla odczytu pliku ini
	int zmienne[2] = { 0 }, x = 0, x1, y1; //zmienne pomocnicze przy odczycie z pliku

	fopen_s(&plik, "freehw_conf.ini", "r"); //czytanie pliku
	if (plik == NULL) //je�li nie ma pliku ini lub inny b��d
	{
		x1 = 425;
		y1 = 470;
	} else {
	for (int i = 0; i<2; i++) //je�li odczyt poprawny, zapisujemy warto�ci z pliku do zmiennych
	{
		fscanf_s(plik, "%d", &x);
		zmienne[i] = x;
	}
	fclose(plik);

	x1 = zmienne[0];
	y1 = zmienne[1];
	}

while(run == false) //je�li zmienna run jest false czyli nie znaleziono jeszcze procesu HW to powtarzamy
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE) //do tego miejsca zapisywany jest snapshot proces�w
		{
			if (_stricmp(entry.szExeFile, "Hauptwerk.exe") == 0) //je�li znaleziono proces to true, odliczamy do klikania
			{
				//ten fragment pobiera rozdzielczo�� ekranu
				HWND window = GetDesktopWindow(); //pobieranie uchwytu ca�o�ci widzianego pulpitu
				RECT rectangle; //deklaracja zmiennej pod GetWindowRect
				GetWindowRect(window, &rectangle); //u�ycie funkcji GetWindowRect

				int x_roz = rectangle.right - rectangle.left;
				int y_roz = rectangle.bottom - rectangle.top;

				HWND okno = FindWindowA(0, "Welcome to Hauptwerk"); // handler dla okna
				RECT prostokat; //definicja typu prostok�t dla wsp�rz�dnych uzyskiwanych przez GetWindowRect
				GetWindowRect(okno, &prostokat); //zapisuje w prostokat parametry okna
				int szerokosc = prostokat.left; //odleg�o�� okna od lewej kraw�dzi ekranu
				int wysokosc = prostokat.top; //odleglosc okna od g�ry ekranu
				while (abs(szerokosc) > x_roz || abs(wysokosc) > y_roz || szerokosc == 0 || wysokosc == 0)
				{
					okno = FindWindowA(0, "Welcome to Hauptwerk");
					GetWindowRect(okno, &prostokat); //zapisuje w prostokat parametry okna
					szerokosc = prostokat.left; //odleg�o�� okna od lewej kraw�dzi ekranu
					wysokosc = prostokat.top; //odleglosc okna od g�ry ekranu

					Sleep(100);
				}

				kliknij(szerokosc+x1, wysokosc+y1); //wykonujemy klikanie

				run = true; //proces znaleziony, zmienna zmienia warto��
				break; //wychodzimy z p�tli while (Process32Next...)
			}
		}
	}
	CloseHandle(snapshot);
	Sleep(100); //op�nienie aby nie robi� snapshot�w proces�w non-stop, wtedy proces obci��a procesor 25%
}
}