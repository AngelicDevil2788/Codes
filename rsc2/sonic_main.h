#ifndef SONIC_MAIN.H
#define SONIC_MAIN_H

#include <cmath>
#include <Windows.h>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <array>
#include <cstring>
#include <cstdio>
#include <vector>

using namespace std;

namespace Sonic
{
	class m_Colors
	{
	public:
		static const short SN_BLACK = { 0x0000 };
		static const short SN_WHITE = { 0x0010 };
		static const short SN_GREY = { 0x0020 };
		static const short SN_RED = { 0x0030 };
		static const short SN_LIME = { 0x0040 };
		static const short SN_BLUE = { 0x0050 };
		static const short SN_YELLOW = { 0x0060 };
		static const short SN_CYAN = { 0x0070 };
		static const short SN_PINK = { 0x0080 };
		static const short SN_PURPLE = { 0x0090 };
		static const short SN_GREEN = { 0x00A0 };
		static const short SN_LIGHT_GREY = { 0x00B0 };
		static const short SN_DARK_CYAN = { 0x00C0 };
		static const short SN_ORANGE = { 0x00D0 };
		static const short SN_LIGHT_GREEN = { 0x00E0 };
		static const short SN_DARK_BLUE = { 0x00F0 };
	};

	struct COORDS
	{
		int w = 0;
		int h = 0;
	};
	struct SNWIN
	{
		static const int SN_MAX_WIDTH = 683;
		static const int SN_MAX_HEIGHT = 352;

		int m_ScreenWidth = SN_MAX_WIDTH;
		int m_ScreenHeight = SN_MAX_HEIGHT;
		WORD* buffer = new WORD[m_ScreenWidth * m_ScreenHeight];
		SMALL_RECT Window = { 0, 0, (short)m_ScreenWidth, (short)m_ScreenHeight };
		DWORD dwBytesWritten = 0;
		string name;
		short bg_Color = m_Colors::SN_BLACK;

	private:
		bool Open = false;
	public:

		const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SNWIN(COORDS Size, LPCWSTR name);
		void close() { Open = false; delete[] hConsole; };
		bool is_open()
		{
			if (Open)
				return true;
			else
				return false;
		}
	};

	struct STRIPE
	{
	public:
		COORDS size = { SNWIN::SN_MAX_WIDTH, SNWIN::SN_MAX_HEIGHT };
		WORD* data = new WORD[size.w * size.h];
		bool bg_transparent = false;
		short bg_Color;

		void Draw(COORDS pos, short color)
		{
			data[pos.h * size.w + pos.w] = color;
		}
		void Draw_Line(COORDS position1, COORDS position2, short color)
		{
			int x, y, dx, dy, dw1, dh1, px, py, xe, ye, i;
			dx = position2.w - position1.w; dy = position2.h - position1.h;
			dw1 = abs(dx); dh1 = abs(dy);
			px = 2 * dh1 - dw1;	py = 2 * dw1 - dh1;
			if (dh1 <= dw1)
			{
				if (dx >= 0)
				{
					x = position1.w; y = position1.h; xe = position2.w;
				}
				else
				{
					x = position2.w; y = position2.h; xe = position1.w;
				}

				Draw({ x, y }, color);

				for (i = 0; x < xe; i++)
				{
					x = x + 1;
					if (px < 0)
						px = px + 2 * dh1;
					else
					{
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
						px = px + 2 * (dh1 - dw1);
					}
					Draw({ x, y }, color);
				}
			}
			else
			{
				if (dy >= 0)
				{
					x = position1.w; y = position1.h; ye = position2.h;
				}
				else
				{
					x = position2.w; y = position2.h; ye = position1.h;
				}

				Draw({ x, y }, color);

				for (i = 0; y < ye; i++)
				{
					y = y + 1;
					if (py <= 0)
						py = py + 2 * dw1;
					else
					{
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
						py = py + 2 * (dw1 - dh1);
					}
					Draw({ x, y }, color);
				}
			}
		}
		void Draw_Triangle(COORDS position1, COORDS position2, COORDS position3, short color)
		{
			Draw_Line(position1, position2, color);
			Draw_Line(position2, position3, color);
			Draw_Line(position3, position1, color);
		}
		void Draw_Full_Triangle(COORDS Position1, COORDS Position2, COORDS Position3, short color)
		{
			auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
			auto drawLine = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw({ i, ny }, color); };

			int t1x, t2x, y, minx, maxx, t1xp, t2xp;
			bool changed1 = false;
			bool changed2 = false;
			int signx1, signx2, dx1, dy1, dx2, dy2;
			int e1, e2;
			// Sort vertices
			if (Position1.h > Position2.h) { SWAP(Position1.h, Position2.h); SWAP(Position1.w, Position2.w); }
			if (Position1.h > Position3.h) { SWAP(Position1.h, Position3.h); SWAP(Position1.w, Position3.w); }
			if (Position2.h > Position3.h) { SWAP(Position2.h, Position3.h); SWAP(Position2.w, Position3.w); }

			t1x = t2x = Position1.w; y = Position1.h;   // Starting points
			dx1 = (int)(Position2.w - Position1.w); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
			else signx1 = 1;
			dy1 = (int)(Position2.h - Position1.h);

			dx2 = (int)(Position3.w - Position1.w); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
			else signx2 = 1;
			dy2 = (int)(Position3.h - Position1.h);

			if (dy1 > dx1) {   // swap values
				SWAP(dx1, dy1);
				changed1 = true;
			}
			if (dy2 > dx2) {   // swap values
				SWAP(dy2, dx2);
				changed2 = true;
			}

			e2 = (int)(dx2 >> 1);
			// Flat top, just process the second half
			if (Position1.h == Position2.h) goto next;
			e1 = (int)(dx1 >> 1);

			for (int i = 0; i < dx1;) {
				t1xp = 0; t2xp = 0;
				if (t1x < t2x) { minx = t1x; maxx = t2x; }
				else { minx = t2x; maxx = t1x; }
				// process first Line until y value is about to change
				while (i < dx1) {
					i++;
					e1 += dy1;
					while (e1 >= dx1) {
						e1 -= dx1;
						if (changed1) t1xp = signx1;//t1x += signx1;
						else          goto next1;
					}
					if (changed1) break;
					else t1x += signx1;
				}
				// Move Line
			next1:
				// process second Line until y value is about to change
				while (1) {
					e2 += dy2;
					while (e2 >= dx2) {
						e2 -= dx2;
						if (changed2) t2xp = signx2;//t2x += signx2;
						else          goto next2;
					}
					if (changed2)     break;
					else              t2x += signx2;
				}
			next2:
				if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
				if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
				drawLine(minx, maxx, y);    // Draw Line from min to max points found on the y
											 // Now increase y
				if (!changed1) t1x += signx1;
				t1x += t1xp;
				if (!changed2) t2x += signx2;
				t2x += t2xp;
				y += 1;
				if (y == Position2.h) break;

			}
		next:
			// Second half
			dx1 = (int)(Position3.w - Position2.w); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
			else signx1 = 1;
			dy1 = (int)(Position3.h - Position2.h);
			t1x = Position2.w;

			if (dy1 > dx1) {   // swap values
				SWAP(dy1, dx1);
				changed1 = true;
			}
			else changed1 = false;

			e1 = (int)(dx1 >> 1);

			for (int i = 0; i <= dx1; i++) {
				t1xp = 0; t2xp = 0;
				if (t1x < t2x) { minx = t1x; maxx = t2x; }
				else { minx = t2x; maxx = t1x; }
				// process first Line until y value is about to change
				while (i < dx1) {
					e1 += dy1;
					while (e1 >= dx1) {
						e1 -= dx1;
						if (changed1) { t1xp = signx1; break; }//t1x += signx1;
						else          goto next3;
					}
					if (changed1) break;
					else   	   	  t1x += signx1;
					if (i < dx1) i++;
				}
			next3:
				// process second Line until y value is about to change
				while (t2x != Position3.w) {
					e2 += dy2;
					while (e2 >= dx2) {
						e2 -= dx2;
						if (changed2) t2xp = signx2;
						else          goto next4;
					}
					if (changed2)     break;
					else              t2x += signx2;
				}
			next4:

				if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
				if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
				drawLine(minx, maxx, y);
				if (!changed1) t1x += signx1;
				t1x += t1xp;
				if (!changed2) t2x += signx2;
				t2x += t2xp;
				y += 1;
				if (y > Position3.h) return;
			}
		}
		void Draw_Circle(COORDS Position, int radius, short color)
		{
			int x = 0;
			int y = radius;
			int p = 3 - 2 * radius;
			if (!radius) return;

			while (y >= x) // only formulate 1/8 of Circle
			{
				Draw({ Position.w - x, Position.h - y }, color);//upper left left
				Draw({ Position.w - y, Position.h - x }, color);//upper upper left
				Draw({ Position.w + y, Position.h - x }, color);//upper upper right
				Draw({ Position.w + x, Position.h - y }, color);//upper right right
				Draw({ Position.w - x, Position.h + y }, color);//lower left left
				Draw({ Position.w - y, Position.h + x }, color);//lower lower left
				Draw({ Position.w + y, Position.h + x }, color);//lower lower right
				Draw({ Position.w + x, Position.h + y }, color);//lower right right
				if (p < 0) p += 4 * x++ + 6;
				else p += 4 * (x++ - y--) + 10;
			}
		}
		void Draw_Full_Circle(COORDS Position, int radius, short color)
		{
			int x = 0;
			int y = radius;
			int p = 3 - 2 * radius;
			if (!radius) return;

			auto drawLine = [&](int sx, int ex, int ny)
			{
				for (int i = sx; i <= ex; i++)
					Draw({ i, ny }, color);
			};

			while (y >= x)
			{
				// Modified to draw scan-Lines instead of edges
				drawLine(Position.w - x, Position.w + x, Position.h - y);
				drawLine(Position.w - y, Position.w + y, Position.h - x);
				drawLine(Position.w - x, Position.w + x, Position.h + y);
				drawLine(Position.w - y, Position.w + y, Position.h + x);
				if (p < 0) p += 4 * x++ + 6;
				else p += 4 * (x++ - y--) + 10;
			}
		}

		STRIPE(COORDS size, short color, bool transparent) : size(size), bg_Color(color), bg_transparent(transparent)
		{
			for (int i = 0; i < (size.w + 1) * (size.h + 1); i++)
			{
				data[i] = bg_Color;
			}
			data[(size.w + 1) * (size.h + 1)] = '\0';
		};
	};

	SNWIN* ACTIVEWIN;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	bool SetCurrentWindow(SNWIN& win);

	void Draw(COORDS Position, short color);
	void Draw_Stripe(COORDS Position, STRIPE stripe);
	void Mirror_Stripe(string Dir, STRIPE stripe1, STRIPE& Destination);
	void Draw_Line(COORDS Position1, COORDS Position2, short color);

	void Draw_Triangle(COORDS Position1, COORDS Position2, COORDS Position3, short color);
	void Draw_Full_Triangle(COORDS Position1, COORDS Position2, COORDS Position3, short color);
	void Draw_Circle(COORDS Position, int radius, short color);
	void Draw_Full_Circle(COORDS Position, int radius, short color);

	void SwapBuffer();
	void ClearScreen();
	void ClearScreen(short bgcolor);
};

bool Sonic::SetCurrentWindow(SNWIN& win)
{
	ACTIVEWIN = &win;
	SetConsoleActiveScreenBuffer((*ACTIVEWIN).hConsole);
	return true;
}

Sonic::SNWIN::SNWIN(COORDS WinSize, LPCWSTR name)
{
	m_ScreenWidth = WinSize.w;
	m_ScreenHeight = WinSize.h;
	name = name;
	Open = true;
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 2;
	cfi.dwFontSize.Y = 2;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = 500;

	wcscpy_s(cfi.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(hConsole, true, &cfi);
	SetConsoleTitle(name);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	COORD coord = { (short)m_ScreenWidth, (short)m_ScreenHeight };
	SetConsoleScreenBufferSize(hConsole, coord);

	Window = { 0, 0, (short)(m_ScreenWidth - 1), (short)(m_ScreenHeight - 1) };
	SetConsoleWindowInfo(hConsole, TRUE, &Window);

	CONSOLE_SCREEN_BUFFER_INFOEX font;
	font.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(hConsole, &font);

	font.ColorTable[0] = RGB(1, 1, 1);
	font.ColorTable[1] = RGB(255, 255, 255);
	font.ColorTable[2] = RGB(127, 127, 127);
	font.ColorTable[3] = RGB(255, 0, 0);
	font.ColorTable[4] = RGB(0, 255, 0);
	font.ColorTable[5] = RGB(0, 0, 255);
	font.ColorTable[6] = RGB(255, 255, 0);
	font.ColorTable[7] = RGB(190, 0, 190);
	font.ColorTable[8] = RGB(10, 140, 10);
	font.ColorTable[9] = RGB(200, 200, 200);
	font.ColorTable[10] = RGB(0, 210, 210);
	font.ColorTable[11] = RGB(255, 160, 0);
	font.ColorTable[12] = RGB(0, 200, 0);
	font.ColorTable[13] = RGB(0, 0, 100);
	font.ColorTable[14] = RGB(127, 0, 0);
	font.ColorTable[15] = RGB(230, 90, 0);

	SetConsoleScreenBufferInfoEx(hConsole, &font);

	SetConsoleMode(hConsole, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	SetConsoleMode(hConsole, ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void Sonic::ClearScreen()
{
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
	float fElapsedTime = elapsedTime.count();
	for (int h = 0; h < (*ACTIVEWIN).m_ScreenHeight; h++)
	{
		for (int w = 0; w < (*ACTIVEWIN).m_ScreenWidth; w++)
		{
			Draw({ w, h }, m_Colors::SN_BLACK);
		}
	}
}

void Sonic::ClearScreen(short color)
{
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
	float fElapsedTime = elapsedTime.count();
	for (int h = 0; h < (*ACTIVEWIN).m_ScreenHeight; h++)
	{
		for (int w = 0; w < (*ACTIVEWIN).m_ScreenWidth; w++)
		{
			Draw({ w, h }, color);
		}
	}
}

void Sonic::Draw(COORDS position, short color)
{
	if (position.h > -1 && position.h < (*ACTIVEWIN).m_ScreenHeight && position.w > -1 && position.w < (*ACTIVEWIN).m_ScreenWidth)
	{
		(*ACTIVEWIN).buffer[position.h * (*ACTIVEWIN).m_ScreenWidth + position.w] = color;
	}
}

void Sonic::Draw_Stripe(COORDS Position, STRIPE stripe)
{
	if (stripe.bg_transparent == false)
	{
		for (int h = 0; h < stripe.size.h + 1; h++)
		{
			for (int w = 0; w < stripe.size.w; w++)
			{
				Draw({ Position.w + w, Position.h + h }, stripe.data[h * stripe.size.w + w]);
			}
		}
	}
	else
	{
		for (int h = 0; h < stripe.size.h + 1; h++)
		{
			for (int w = 0; w < stripe.size.w; w++)
			{
				if (stripe.data[h * stripe.size.w + w] == stripe.bg_Color)
				{
				}
				else
					Draw({ Position.w + w, Position.h + h }, stripe.data[h * stripe.size.w + w]);
			}
		}
	}
}

void Sonic::Mirror_Stripe(string Dir, STRIPE stripe1, STRIPE& Destination)
{
	Destination.bg_Color = stripe1.bg_Color;
	if (stripe1.size.w == Destination.size.w)
	{
		if (stripe1.size.h == Destination.size.h)
		{
			if (Dir == "left-right")
			{
				for (int h = 0; h < stripe1.size.h + 1; h++)
				{
					for (int w = 0; w < stripe1.size.w; w++)
					{
						Destination.data[h * stripe1.size.w + w] = stripe1.data[(h + 1) * stripe1.size.w - w];
					}
				}
			};
			if (Dir == "up-down")
			{
				for (int h = 0; h < stripe1.size.h + 1; h++)
				{
					for (int w = 0; w < stripe1.size.w; w++)
					{
						Destination.data[h * stripe1.size.w + w] = stripe1.data[(stripe1.size.h - h) * stripe1.size.w + w];
					}
				}
			}
		}
	}
}

void Sonic::Draw_Line(COORDS position1, COORDS position2, short color)
{
	int x, y, dx, dy, dw1, dh1, px, py, xe, ye, i;
	dx = position2.w - position1.w; dy = position2.h - position1.h;
	dw1 = abs(dx); dh1 = abs(dy);
	px = 2 * dh1 - dw1;	py = 2 * dw1 - dh1;
	if (dh1 <= dw1)
	{
		if (dx >= 0)
		{
			x = position1.w; y = position1.h; xe = position2.w;
		}
		else
		{
			x = position2.w; y = position2.h; xe = position1.w;
		}

		Draw({ x, y }, color);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dh1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dh1 - dw1);
			}
			Draw({ x, y }, color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = position1.w; y = position1.h; ye = position2.h;
		}
		else
		{
			x = position2.w; y = position2.h; ye = position1.h;
		}

		Draw({ x, y }, color);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dw1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dw1 - dh1);
			}
			Draw({ x, y }, color);
		}
	}
};

void Sonic::Draw_Triangle(COORDS Position1, COORDS Position2, COORDS Position3, short color)
{
	Draw_Line(Position1, Position2, color);
	Draw_Line(Position2, Position3, color);
	Draw_Line(Position3, Position1, color);
}

void Sonic::Draw_Full_Triangle(COORDS Position1, COORDS Position2, COORDS Position3, short color)
{
	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
	auto drawLine = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw({ i, ny }, color); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (Position1.h > Position2.h) { SWAP(Position1.h, Position2.h); SWAP(Position1.w, Position2.w); }
	if (Position1.h > Position3.h) { SWAP(Position1.h, Position3.h); SWAP(Position1.w, Position3.w); }
	if (Position2.h > Position3.h) { SWAP(Position2.h, Position3.h); SWAP(Position2.w, Position3.w); }

	t1x = t2x = Position1.w; y = Position1.h;   // Starting points
	dx1 = (int)(Position2.w - Position1.w); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(Position2.h - Position1.h);

	dx2 = (int)(Position3.w - Position1.w); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(Position3.h - Position1.h);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (Position1.h == Position2.h) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first Line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move Line
	next1:
		// process second Line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawLine(minx, maxx, y);    // Draw Line from min to max points found on the y
									 // Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == Position2.h) break;

	}
next:
	// Second half
	dx1 = (int)(Position3.w - Position2.w); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(Position3.h - Position2.h);
	t1x = Position2.w;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first Line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second Line until y value is about to change
		while (t2x != Position3.w) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawLine(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > Position3.h) return;
	}
}

void Sonic::Draw_Circle(COORDS Position, int radius, short color)
{
	int x = 0;
	int y = radius;
	int p = 3 - 2 * radius;
	if (!radius) return;

	while (y >= x) // only formulate 1/8 of Circle
	{
		Draw({ Position.w - x, Position.h - y }, color);//upper left left
		Draw({ Position.w - y, Position.h - x }, color);//upper upper left
		Draw({ Position.w + y, Position.h - x }, color);//upper upper right
		Draw({ Position.w + x, Position.h - y }, color);//upper right right
		Draw({ Position.w - x, Position.h + y }, color);//lower left left
		Draw({ Position.w - y, Position.h + x }, color);//lower lower left
		Draw({ Position.w + y, Position.h + x }, color);//lower lower right
		Draw({ Position.w + x, Position.h + y }, color);//lower right right
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void Sonic::Draw_Full_Circle(COORDS Position, int radius, short color)
{
	int x = 0;
	int y = radius;
	int p = 3 - 2 * radius;
	if (!radius) return;

	auto drawLine = [&](int sx, int ex, int ny)
	{
		for (int i = sx; i <= ex; i++)
			Draw({ i, ny }, color);
	};

	while (y >= x)
	{
		// Modified to draw scan-Lines instead of edges
		drawLine(Position.w - x, Position.w + x, Position.h - y);
		drawLine(Position.w - y, Position.w + y, Position.h - x);
		drawLine(Position.w - x, Position.w + x, Position.h + y);
		drawLine(Position.w - y, Position.w + y, Position.h + x);
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void Sonic::SwapBuffer()
{
	DWORD written = 0;
	WriteConsoleOutputAttribute((*ACTIVEWIN).hConsole, (*ACTIVEWIN).buffer, (*ACTIVEWIN).m_ScreenWidth * (*ACTIVEWIN).m_ScreenHeight, { 0, 0 }, &written);
}
#endif // !SONIC_MAIN.H