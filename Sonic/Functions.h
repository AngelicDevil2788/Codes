#pragma once

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <wchar.h>

#define SN_UP "2070UP925"
#define SN_DOWN "2070DOWN925"
#define SN_LEFT "2070LEFT925"
#define SN_RIGHT "2070RIGHT925"
#define SN_UP_RIGHT "2070UP_RIGHT925"
#define SN_UP_LEFT "2070UP_LEFT925"
#define SN_DOWN_RIGHT "2070DOWN_RIGHT925"
#define SN_DOWN_LEFT "2070DOWN_LEFT925"

using namespace std;
class Sonic
{
public:
	Sonic();
	virtual ~Sonic();
	class Buffer
	{
	public:
		Buffer();
		virtual ~Buffer();
		int nScreenWidth = 170;
		int nScreenHeight = 44;
		wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
		HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		DWORD dwBytesWritten = 0;
		void Display();
		void Draw(int, int, wchar_t);
		void Create(int, int);
		void Fill(wchar_t);
		void Clear(wchar_t);
		void Draw_Side(string, wchar_t);
		void Type(int, int, LPCWSTR);
	};
	Buffer buffer;

	class Player
	{
	public:
		Buffer buffer;
		Player();
		virtual ~Player();
		LPCWSTR name = L"NONE";
		int age = 0;
		float height = 0.0f;
		int MaxHP = 0;
		void Create(LPCWSTR);
		void Set_Age(int);
		void Set_Height(float);
		void Set_MaxHP(int);
	};
	Player player;
};