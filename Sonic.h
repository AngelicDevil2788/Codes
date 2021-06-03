#pragma once
/*
-----------------------------------------------------------
IMPORTANT
-----------------------------------------------------------
I only defined some of the characters from the character map that I think are the most useful, so if you want to use more go to character map application and 
look for the ones you need, good luck and please give me a feedback once you used this engine
*/
#ifndef SONIC.H
#define SONIC_H

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <wchar.h>
#include <fstream>
#include <vector>

using namespace std;

#define SN_UP L'î'
#define SN_DOWN L'V'
#define SN_RIGHT L'>'
#define SN_LEFT L'<'

#define SN_SHADE_NONE L' '
#define SN_SHADE_WHITE L'\x2593'
#define SN_SHADE_GREY L'\x2592'
#define SN_SHADE_BLACK L'\x2591'
#define SN_SHADE_FULL L'\x2588'

#define SN_SHADE_CORNER_UPRIGHT L'\x250C'
#define SN_SHADE_CORNER_DOWNRIGHT L'\x2518'
#define SN_SHADE_CORNER_UPLEFT L'\x2510'
#define SN_SHADE_CORNER_DOWNLEFT L'\x2514'

#define SN_SHADE_LINE_HORISONTAL L'\x2500'
#define SN_SHADE_LINE_VERTICAL L'\x2502'
#define SN_SHADE_LINE_VERTICAL_MIDDLE_RIGHT L'\x251C'
#define SN_SHADE_LINE_VERTICAL_MIDDLE_LEFT L'\x2524'
#define SN_SHADE_LINE_HORISONTAL_MIDDLE_UP L'\x2534'
#define SN_SHADE_LINE_HORISONTAL_MIDDLE_DOWN L'\x252C'
#define SN_SHADE_LINE_CROSS L'\x253C'

struct SNwindow
{
	bool is_open = false;
	int width = 227;
	int height = 58;
	HANDLE hBufferConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBufferBytesWritten = 0;
	LPCWSTR name = L" ";
	wchar_t* buffer = new wchar_t[width * height];
	void create(int Width, int Height,  LPCWSTR Name)
	{
		width = Width;
		height = Height;
		name = Name;
	};

	bool close()
	{
		is_open = false;
		return true;
	};
};

struct SNrectangle
{
	int PosWindth = 0;
	int PosHeight = 0;
	int length = 0;
	int width = 0;
	wchar_t Brush = L' ';
	wchar_t FillBrush = L' ';
};

SNwindow CurrentWindow;

bool snMakeContextCurrent(SNwindow window)
{
	window.is_open = true;
	CurrentWindow = window;
	SetConsoleActiveScreenBuffer(CurrentWindow.hBufferConsole);
	SetConsoleTitle(CurrentWindow.name);
	return true;
};

void snRender()
{
	for (int h = 0; h < CurrentWindow.height; h++)
	{
		for (int w = 0; w < CurrentWindow.width; w++)
		{
			CurrentWindow.buffer[h * CurrentWindow.width + w] = L' ';
		}
	}
}

void snDraw(SNwindow window, wchar_t c, int w, int h)
{
	window.buffer[(h - 1) * window.width + (w - 1)] = c;
};

#define snDraw snDraw

void snFill(SNwindow window, wchar_t c)
{
	for (int i = 0; i < window.width * window.height; i++)
	{
		window.buffer[i] = c;
	}
}

void snSetBorders(SNwindow Window, wchar_t c)
{
	for (int h = 0; h < Window.height; h++)
	{
		for (int w = 0; w < Window.width; w++)
		{
			Window.buffer[w] = c;
			Window.buffer[h * Window.width] = c;
			Window.buffer[h * Window.width - 1] = c;
			Window.buffer[(Window.height - 1) * Window.width + w] = c;
		};
	}
}

void snDrawShape(SNwindow window, SNrectangle rectangle)
{
	int Pos = (rectangle.PosHeight - 1) * window.width + (rectangle.PosWindth - 1);
	for (int h = 0; h < rectangle.width; h++)
	{
		for (int w = 0; w < rectangle.length; w++)
		{
			window.buffer[Pos + w] = rectangle.Brush;
			window.buffer[Pos + h * window.width] = rectangle.Brush;
			window.buffer[Pos + h * window.width + rectangle.length - 1] = rectangle.Brush;
			window.buffer[Pos + (rectangle.width - 1) * window.width + w] = rectangle.Brush;
			if (w < rectangle.length - 2 && h < rectangle.width - 2)
			{
				window.buffer[rectangle.PosHeight * window.width + rectangle.PosWindth + h * window.width + w] = rectangle.FillBrush;
			}
		}
	}
}

void snReplaceCharacters(SNwindow window, wchar_t character1, wchar_t character2)
{
	for (int p = 0; p < window.height * window.width; p++)
	{
		if (window.buffer[p] == character1)
		{
			window.buffer[p] = character2;
		}
	}
}

void snSet_FPS(int frames)
{
	Sleep(1000 / frames);
}

void snSwapBuffers()
{
	WriteConsoleOutputCharacter(CurrentWindow.hBufferConsole, CurrentWindow.buffer, CurrentWindow.width * CurrentWindow.height, { 0,0 }, &CurrentWindow.dwBufferBytesWritten);
};

void snSwapWindow(SNwindow window1, SNwindow window2)
{
	SNwindow window;
	window = window1;
	window1 = window2;
	window2 = window;
}

void snMakeBufferCurrent(SNwindow window)
{
	CurrentWindow.buffer = window.buffer;
}

void snTerminateIfKeyPressed(int key)
{
	if (GetAsyncKeyState(key))
	{
		CurrentWindow.close();
	}
}

void snTerminate()
{
		CurrentWindow.close();
}

struct Vertex2f
{
	float f1;
	float f2;
};
struct Vertex3f
{
	float f1;
	float f2;
	float f3;
};
struct Vertex2s
{
	LPCWSTR f1;
	LPCWSTR f2;
};
struct Vertex3s
{
	LPCWSTR f1;
	LPCWSTR f2;
	LPCWSTR f3;
};

#endif