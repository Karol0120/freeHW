#define _WIN32_WINNT 0x500
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

void kliknij(HWND, int delay, int x1, int x2, int y1, int y2)
{
	Sleep(delay); //opóŸnienie klikania od uruchomienia siê procesu Hauptwerk.exe
	SetCursorPos(x1, y1); //przemieszczenie kursora nad punkt
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //pojedyncze klikniêcie nad tym punktem

	SetCursorPos(x2, y2); //przemieszczenie kursora nad punkt 2
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //pojedyncze klikniêcie nad tym punktem

	SetCursorPos(0, 0); //myszka wraca do pozycji 0, 0
}
int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE); //te 2 linie ukrywaj¹ okno programu exe, jest niewidoczne gdy dzia³a

	bool run = false; //zmienna okreœla stan czy ju¿ znaleziono Hauptwerk.exe wœród listy dzia³aj¹cych procesów
	FILE *plik; //handler dla odczytu pliku ini
	int zmienne[5] = { 0 }, x = 0, delay, x1, x2, y1, y2; //zmienne pomocnicze przy odczycie z pliku

	fopen_s(&plik, "freehw_conf.ini", "r"); //czytanie pliku
	if (plik == NULL) //jeœli nie ma pliku ini lub inny b³¹d
	{
		delay = 5000; //wtedy wczytujemy domyœlne dla 1024x768 wartoœci
		x1 = 290;
		y1 = 494;
		x2 = 624;
		y2 = 607;
	} else { //jeœli odczyt poprawny, zapisujemy wartoœci z pliku do zmiennych
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


while(run == false) //jeœli zmienna run jest false czyli nie znaleziono jeszcze procesu HW to powtarzamy
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE) //do tego miejsca zapisywany jest snapshot procesów
		{
			if (_stricmp(entry.szExeFile, "Hauptwerk.exe") == 0) //jeœli znaleziono proces to true
			{
					kliknij(FindWindowA(0, "Hauptwerk.exe"), delay, x1, x2, y1, y2); //wykonujemy klikanie
					run = true; //proces znaleziony, zmienna zmienia wartoœæ
					break; //wychodzimy z pêtli while (Process32Next...
			}
		}
	}

	CloseHandle(snapshot);
	Sleep(250); //opóŸnienie aby nie robiæ snapshotów procesów non-stop, wtedy proces obici¹¿a procesor 25%
}

	return 0;
}