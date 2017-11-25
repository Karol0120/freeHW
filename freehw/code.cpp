#define _WIN32_WINNT 0x500
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

void kliknij(HWND, int delay, int x1, int x2, int y1, int y2)
{
	Sleep(delay); //op�nienie klikania od uruchomienia si� procesu Hauptwerk.exe
	SetCursorPos(x1, y1); //przemieszczenie kursora nad punkt
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //pojedyncze klikni�cie nad tym punktem

	SetCursorPos(x2, y2); //przemieszczenie kursora nad punkt 2
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //pojedyncze klikni�cie nad tym punktem

	SetCursorPos(0, 0); //myszka wraca do pozycji 0, 0
}
int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE); //te 2 linie ukrywaj� okno programu exe, jest niewidoczne gdy dzia�a

	bool run = false; //zmienna okre�la stan czy ju� znaleziono Hauptwerk.exe w�r�d listy dzia�aj�cych proces�w
	FILE *plik; //handler dla odczytu pliku ini
	int zmienne[5] = { 0 }, x = 0, delay, x1, x2, y1, y2; //zmienne pomocnicze przy odczycie z pliku

	fopen_s(&plik, "freehw_conf.ini", "r"); //czytanie pliku
	if (plik == NULL) //je�li nie ma pliku ini lub inny b��d
	{
		delay = 5000; //wtedy wczytujemy domy�lne dla 1024x768 warto�ci
		x1 = 290;
		y1 = 494;
		x2 = 624;
		y2 = 607;
	} else { //je�li odczyt poprawny, zapisujemy warto�ci z pliku do zmiennych
	for (int i = 0; i<5; i++)
	{
		fscanf_s(plik, "%d", &x);
		zmienne[i] = x;
	}
	fclose(plik);

	delay = zmienne[0];
	x1 = zmienne[1];
	y1 = zmienne[2];
	x2 = zmienne[3];
	y2 = zmienne[4];
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
			if (_stricmp(entry.szExeFile, "Hauptwerk.exe") == 0) //je�li znaleziono proces to true
			{
					kliknij(FindWindowA(0, "Hauptwerk.exe"), delay, x1, x2, y1, y2); //wykonujemy klikanie
					run = true; //proces znaleziony, zmienna zmienia warto��
					break; //wychodzimy z p�tli while (Process32Next...
			}
		}
	}

	CloseHandle(snapshot);
	Sleep(250); //op�nienie aby nie robi� snapshot�w proces�w non-stop, wtedy proces obici��a procesor 25%
}

	return 0;
}