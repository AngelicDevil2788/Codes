#pragma once

#ifndef SONIC.H
#define SONIC_H

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <wchar.h>
#include <fstream>
#include <vector>


using namespace std;
struct SNwindow
{
	bool is_open = false;
	int width = 170;
	int height = 44;
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

SNwindow CurrentWindow;


bool MakeContextCurrent(SNwindow window)
{
	window.is_open = true;
	CurrentWindow = window;
	SetConsoleActiveScreenBuffer(CurrentWindow.hBufferConsole);
	SetConsoleTitle(CurrentWindow.name);
	return true;
};

void snDraw(SNwindow window, wchar_t c, int w, int h)
{
	window.buffer[(h - 1) * window.width + (w - 1)] = c;
};

#define snDraw snDraw

void snPrint(SNwindow window, LPCWSTR s, int w, int h)
{
	wsprintf(&window.buffer[(h - 1) * window.width + (w - 1)], s);
};

#define snPrint snPrint

void snRender()
{
	for (int i = 0; i < CurrentWindow.width * CurrentWindow.height; i++) CurrentWindow.buffer[i] = L' ';
}

#define snRender snRender

void snFill(SNwindow window, wchar_t c)
{
	for (int i = 0; i < window.width * window.height; i++)
	{
		window.buffer[i] = c;
	}
}

#define snFill snFill

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

#define snSetBorders snSetBorders

void snSet_FPS(int frames)
{
	Sleep(1000 / frames);
}

void snSwapBuffers(SNwindow window)
{
	CurrentWindow = window;
	WriteConsoleOutputCharacter(CurrentWindow.hBufferConsole, CurrentWindow.buffer, CurrentWindow.width * CurrentWindow.height, { 0,0 }, &CurrentWindow.dwBufferBytesWritten);
};

#define snSwapBuffers snSwapBuffers

void snSwapWindow(SNwindow window1, SNwindow window2)
{
	SNwindow window;
	window = window1;
	window1 = window2;
	window2 = window;
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