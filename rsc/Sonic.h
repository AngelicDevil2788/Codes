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

#include "Sonic_defined_symbols.h"

namespace Sonic
{
	struct window
	{
		bool is_open = false;
		HANDLE hBufferConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		DWORD dwBufferBytesWritten = 0;
		LPCWSTR name = L" ";
		struct Buffer
		{
			int width = 227;
			int height = 70;
			wchar_t* screen = new wchar_t[width * height];
			int* Pixel_ = new int[width * height];
		};
		Buffer buffer;
		void create(int Width, int Height, LPCWSTR Name)
		{
			buffer.width = Width;
			buffer.height = Height;
			name = Name;
			for (int i = 0; i < buffer.width * buffer.height; i++)
			{
				buffer.Pixel_[i] = 0;
			}
		};

		bool close()
		{
			is_open = false;
			return true;
		};
	};

	window CurrentWindow;

	bool MakeContextCurrent(window window);

	void Render();

	void Draw(window window, wchar_t c, int w, int h);

	void Fill(window window, wchar_t c);

	void SetBorders(window Window, wchar_t c);

	void Print(window window, wstring phrase, int width, int height);

	void PrintNumber(window window, int number, int width, int height);

	void ReplaceCharacters(window window, wchar_t character1, wchar_t character2);

	void Set_FPS(int frames);

	void SwapBuffers();

	void SwapWindow(window window1, window window2);

	void MakeBufferCurrent(window window);

	void TerminateIfKeyPressed(int key);

	void Terminate();

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
}

bool Sonic::MakeContextCurrent(window window)
{
	window.is_open = true;
	Sonic::CurrentWindow = window;
	SetConsoleActiveScreenBuffer(Sonic::CurrentWindow.hBufferConsole);
	SetConsoleTitle(CurrentWindow.name);
	return true;
};

void Sonic::Render()
{
	for (int h = 0; h < CurrentWindow.buffer.height; h++)
	{
		for (int w = 0; w < CurrentWindow.buffer.width; w++)
		{
			CurrentWindow.buffer.screen[h * CurrentWindow.buffer.width + w] = L' ';
		}
	}
};

void Sonic::Draw(window window, wchar_t c, int w, int h)
{
	window.buffer.screen[(h - 1) * window.buffer.width + (w - 1)] = c;
};

void Sonic::Fill(window window, wchar_t c)
{
	for (int i = 0; i < window.buffer.height; i++)
	{
		window.buffer.screen[i] = c;
	}
};

void Sonic::SetBorders(window Window, wchar_t c)
{
	for (int h = 0; h < Window.buffer.height; h++)
	{
		for (int w = 0; w < Window.buffer.width; w++)
		{
			Window.buffer.screen[w] = c;
			Window.buffer.screen[h * Window.buffer.width] = c;
			Window.buffer.screen[(Window.buffer.height - 1) * Window.buffer.width + w] = c;
			Window.buffer.screen[h * Window.buffer.width - 1] = c;
		};
	}
};

void Sonic::Print(window window, wstring phrase, int width, int height)
{
	for (int i = 0; i < phrase.size(); i++)
	{
		window.buffer.screen[(height - 1) * window.buffer.width + (width - 1) + i] = phrase.at(i);
	}
};

void Sonic::PrintNumber(window window, int number, int width, int height)
{
	wsprintf(&window.buffer.screen[(height - 1) * window.buffer.width + (width - 1)], L"%d", number);
	if (number < 10)
		Draw(window, L' ', width + 1, height);
	else if (number > 9 && number < 100)
		Draw(window, L' ', width + 2, height);
	else if (number > 99 && number < 1000)
		Draw(window, L' ', width + 3, height);
	else if (number > 999 && number < 1000)
		Draw(window, L' ', width + 4, height);
	else if (number > 9999 && number < 10000)
		Draw(window, L' ', width + 5, height);
	else if (number > 99999 && number < 100000)
		Draw(window, L' ', width + 6, height);
};

void Sonic::ReplaceCharacters(window window, wchar_t character1, wchar_t character2)
{
	for (int p = 0; p < window.buffer.height * window.buffer.width; p++)
	{
		if (window.buffer.screen[p] == character1)
		{
			window.buffer.screen[p] = character2;
		}
	}
};

void Sonic::Set_FPS(int frames)
{
	Sleep(1000 / frames);
}

void Sonic::SwapBuffers()
{
	WriteConsoleOutputCharacter(CurrentWindow.hBufferConsole, CurrentWindow.buffer.screen, CurrentWindow.buffer.width * CurrentWindow.buffer.height, { 0,0 }, &CurrentWindow.dwBufferBytesWritten);
};

void Sonic::SwapWindow(window window1, window window2)
{
	window window;
	window = window1;
	window1 = window2;
	window2 = window;
};

void Sonic::MakeBufferCurrent(window window)
{
	CurrentWindow.buffer = window.buffer;
}

void Sonic::TerminateIfKeyPressed(int key)
{
	if (GetAsyncKeyState(key))
	{
		CurrentWindow.close();
	}
}

void Sonic::Terminate()
{
	CurrentWindow.close();
};
#endif