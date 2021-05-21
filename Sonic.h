#pragma once

#ifndef SONIC.H
#define SONIC_H

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <wchar.h>
#include <fstream>
#include <vector>

#define SN_UP "2070UP925"
#define SN_DOWN "2070DOWN925"
#define SN_LEFT "2070LEFT925"
#define SN_RIGHT "2070RIGHT925"
#define SN_UP_RIGHT "2070UP_RIGHT925"
#define SN_UP_LEFT "2070UP_LEFT925"
#define SN_DOWN_RIGHT "2070DOWN_RIGHT925"
#define SN_DOWN_LEFT "2070DOWN_LEFT925"

using namespace std;
namespace Sonic
{
	/*
	====================
	The Buffer namespace
	====================
	*/
	namespace Screen
	{
		int nScreenWidth = 170;
		int nScreenHeight = 44;
		wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
		HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		DWORD dwBytesWritten = 0;
		bool is_open = false;
		void Display();
		void Draw(wchar_t, int, int);
		void Draw_Pos(wchar_t, int);
		void Create(int, int);
		void Fill(wchar_t);
		void Clear();
		void Print(LPCWSTR, int, int);
		void Print_Pos(LPCWSTR, int);
		void Draw_Side(string, wchar_t);
	};
	/*
	====================
	The Player namespace
	====================
	*/
	namespace Player
	{
		LPCWSTR name = L"NONE";
		int age = 0;
		float height = 0.0f;
		int MaxHP = 0;
		int MaxMP = 0;
		void Create(LPCWSTR);
		void Set_Age(int);
		void Set_Height(float);
		void Set_MaxHP(int);
		void Set_MAxMP(int);
	};

	/*
	====================
	The Vertex Namespace
	====================
	*/

	namespace Vertex
	{
		struct i2
		{
			int F1;
			int F2;
		};
		struct i3
		{
			int F1;
			int F2;
			int F3;
		};
		struct i4
		{
			int F1;
			int F2;
			int F3;
			int F4;
		};
		struct i5
		{
			int F1;
			int F2;
			int F3;
			int F4;
			int F5;
		};
		struct c2
		{
			char F1;
			char F2;
		};
		struct c3
		{
			char F1;
			char F2;
			char F3;
		};
		struct c4
		{
			char F1;
			char F2;
			char F3;
			char F4;
		};
		struct c5
		{
			char F1;
			char F2;
			char F3;
			char F4;
			char F5;
		};
		struct s2
		{
			LPCWSTR F1;
			LPCWSTR F2;
		};
		struct s3
		{
			LPCWSTR F1;
			LPCWSTR F2;
			LPCWSTR F3;
		};
		struct s4
		{
			LPCWSTR F1;
			LPCWSTR F2;
			LPCWSTR F3;
			LPCWSTR F4;
		};
		struct s5
		{
			LPCWSTR F1;
			LPCWSTR F2;
			LPCWSTR F3;
			LPCWSTR F4;
			LPCWSTR F5;
		};
		struct f2
		{
			float F1;
			float F2;
		};
		struct f3
		{
			float F1;
			float F2;
			float F3;
		};
		struct f4
		{
			float F1;
			float F2;
			float F3;
			float F4;
		};
		struct f5
		{
			float F1;
			float F2;
			float F3;
			float F4;
			float F5;
		};
	};

	/*
	======================
	The *system* Namespace
	======================
	*/

	namespace system
	{
		void pause(int);
		void set_FPS(int);
		void EndIfKEyPressed(int);
		int SetPos(int, int);
	}
};

/*
===================================================
The *Buffer* Namespace for creating the game window
===================================================
*/

/*
-----------------------------------
Functions in the *Buffer* namespace
-----------------------------------
*/

void Sonic::Screen::Create(int nScreenWidth1, int nScreenHeight1)
{
	nScreenWidth = nScreenWidth1;
	nScreenHeight = nScreenHeight1;
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	SetConsoleActiveScreenBuffer(hConsole);
	is_open = true;
}

void Sonic::Screen::Display()
{
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
}

void Sonic::Screen::Draw(wchar_t difference, int PosWidth, int PosHeight)
{
	screen[(PosHeight - 1) * nScreenWidth + (PosWidth - 1)] = difference;
}

void Sonic::Screen::Draw_Pos(wchar_t difference, int Pos)
{
	screen[Pos] = difference;
}

void Sonic::Screen::Fill(wchar_t what)
{
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = what;
}

void Sonic::Screen::Clear()
{
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
}

void Sonic::Screen::Draw_Side(string place, wchar_t difference)
{
	if (place == "2070UP925")
		for (int w = 1; w < nScreenWidth - 1; w++)
			screen[w] = difference;
	else if (place == "2070DOWN925")
		for (int w = 1; w < nScreenWidth - 1; w++)
			screen[(nScreenHeight - 1) * nScreenWidth + w] = difference;
	else if (place == "2070LEFT925")
		for (int h = 1; h < nScreenHeight - 1; h++)
			screen[h * nScreenWidth] = difference;
	else if (place == "2070RIGHT925")
		for (int h = 1; h < nScreenHeight - 1; h++)
			screen[h * nScreenWidth + nScreenWidth - 1] = difference;
	else if (place == "2070UP_RIGHT925")
		screen[nScreenWidth - 1] = difference;
	else if (place == "2070UP_LEFT925")
		screen[0] = difference;
	else if (place == "2070DOWN_RIGHT925")
		screen[nScreenWidth * nScreenHeight - 1] = difference;
	else if (place == "2070DOWN_LEFT925")
		screen[nScreenWidth * (nScreenHeight - 1)] = difference;
}

void Sonic::Screen::Print(LPCWSTR tipe, int PosWidth = 0, int PosHeight = 0)
{
	wsprintf(&screen[(PosHeight - 1) * nScreenWidth + PosWidth - 1], tipe);
}

void Sonic::Screen::Print_Pos(LPCWSTR tipe, int Pos)
{
	wsprintf(&screen[Pos], tipe);
}

/*
========================================================
The *Player* Namespace for creating a Player in the Game
========================================================
*/

/*
-----------------------------------
Functions in the *Player* namespace
-----------------------------------
*/

void Sonic::Player::Create(LPCWSTR name1)
{
	name = name1;
}

void Sonic::Player::Set_Age(int Age)
{
	age = Age;
}

void Sonic::Player::Set_Height(float Height)
{
	height = Height;
}

void Sonic::Player::Set_MaxHP(int maxhp)
{
	MaxHP = maxhp;
}

void Sonic::Player::Set_MAxMP(int maxmp)
{
	MaxMP = maxmp;
}

/*
==========================================================
The *system* Namespace for putting timings and setting FPS
==========================================================
*/

/*
-----------------------------------
Functions in the *system* namespace
-----------------------------------
*/

void Sonic::system::pause(int key)
{
	while (true)
	{
		if (GetAsyncKeyState(key))
		{
			break;
		}
	}
}

void Sonic::system::set_FPS(int frames)
{
	Sleep(1000 / frames);
}

int Sonic::system::SetPos(int PosWidth, int PosHeight)
{
	int Pos = (PosHeight - 1) * Screen::nScreenWidth + (PosWidth - 1);
	return Pos;
}

void Sonic::system::EndIfKEyPressed(int key)
{
	if (GetAsyncKeyState(key))
		Sonic::Screen::is_open = false;
}

#endif