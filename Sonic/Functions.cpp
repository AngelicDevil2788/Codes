#include "Functions.h"

/*
------------------------------
Initializing the *Sonic* class
------------------------------
*/
Sonic::Sonic()
{
}

Sonic::~Sonic()
{
}
/*
===============================================
The *Buffer* Class for creating the game window
===============================================
*/

/*
-------------------------------
Initializing the *Buffer* class
-------------------------------
*/
Sonic::Buffer::Buffer()
{
}

Sonic::Buffer::~Buffer()
{
}

/*
-------------------------------
Functions in the *Buffer* class
-------------------------------
*/

void Sonic::Buffer::Create(int nScreenWidth1, int nScreenHeight1)
{
	nScreenWidth = nScreenWidth1;
	nScreenHeight = nScreenHeight1;
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	SetConsoleActiveScreenBuffer(hConsole);
}

void Sonic::Buffer::Display()
{
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
}

void Sonic::Buffer::Draw(int PosWidth, int PosHeight, wchar_t difference)
{
	screen[(PosHeight - 1) * nScreenWidth + (PosWidth - 1)] = difference;
}

void Sonic::Buffer::Fill(wchar_t what)
{
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = what;
}

void Sonic::Buffer::Clear(wchar_t)
{
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
}

void Sonic::Buffer::Draw_Side(string place, wchar_t difference)
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

void Sonic::Buffer::Type(int PosWidth, int PosHeight, LPCWSTR tipe)
{
	wsprintf(&screen[(PosHeight - 1) * nScreenWidth + PosWidth - 1], tipe);
}

/*
====================================================
The *Player* Class for creating a Player in the Game
====================================================
*/

/*
-------------------------------
Initializing the *Player* class
-------------------------------
*/
Sonic::Player::Player()
{
}

Sonic::Player::~Player()
{
}

/*
-------------------------------
Functions in the *Player* class
-------------------------------
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