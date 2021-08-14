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
		HANDLE hBufferConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		LPCWSTR name = L" ";
		struct Buffer
		{
			int width = SN_MAX_WIDTH;
			int height = SN_MAX_HEIGHT;
			CHAR_INFO* screen = new CHAR_INFO[width * height];
			int* Pixel_ = new int[width * height];
		};
		Buffer buffer;
		SMALL_RECT dwBufferBytesWritten = { 0, 0, (short)buffer.width, (short)buffer.height };
		void create(int Width, int Height, int fontw, int fonth, LPCWSTR Name)
		{
			buffer.width = Width;
			buffer.height = Height;
			name = Name;
			for (int i = 0; i < buffer.width * buffer.height; i++)
			{
				buffer.Pixel_[i] = 0;
			}
			CONSOLE_FONT_INFOEX cfi;
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = fontw;
			cfi.dwFontSize.Y = fonth;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;

			wcscpy_s(cfi.FaceName, L"Lucida Console");
			SetCurrentConsoleFontEx(hBufferConsole, false, &cfi);
		};

		bool close()
		{
			is_open = false;
			return true;
		};
	};

	window CurrentWindow;

	bool MakeContextCurrent(window window);

	void Render(short color);

	void Fill(window window, wchar_t character, short color);

	void SetBorders(window Window, wchar_t character, short color);

	void Print(window window, wstring phrase, short color, int width, int height);

	void PrintNumber(window window, int number, short color, int width, int height);

	void ReplaceCharacters(window window, wchar_t character1, wchar_t character2);

	void Set_FPS(int frames);

	void SwapBuffers();

	void SwapWindow(window window1, window window2);

	void MakeBufferCurrent(window window);

	void TerminateIfKeyPressed(int key);

	void Terminate();

	wstring MakeStringWide(string phrase);

	int GetPos(int width, int height);

	struct Vertewidth2f
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
	struct Vertewidth2s
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

void Sonic::Render( short color)
{
	for (int h = 0; h < CurrentWindow.buffer.height; h++)
	{
		for (int w = 0; w < CurrentWindow.buffer.width; w++)
		{
			CurrentWindow.buffer.screen[h * CurrentWindow.buffer.width + w].Char.UnicodeChar = ' ';
			CurrentWindow.buffer.screen[h * CurrentWindow.buffer.width + w].Attributes = color;
		}
	}
};

void Sonic::Fill(window window, wchar_t character, short color)
{
	for (int i = 0; i < window.buffer.height; i++)
	{
		window.buffer.screen[i].Char.UnicodeChar = character;
		window.buffer.screen[i].Attributes = color;
	}
};

void Sonic::SetBorders(window Window, wchar_t character, short color)
{
	for (int h = 0; h < Window.buffer.height; h++)
	{
		for (int w = 0; w < Window.buffer.width; w++)
		{
			Window.buffer.screen[w].Char.UnicodeChar = character;
			Window.buffer.screen[w].Attributes = color;
			Window.buffer.screen[h * Window.buffer.width].Char.UnicodeChar = character;
			Window.buffer.screen[h * Window.buffer.width].Attributes = color;
			Window.buffer.screen[(Window.buffer.height - 1) * Window.buffer.width + w].Char.UnicodeChar = character;
			Window.buffer.screen[(Window.buffer.height - 1) * Window.buffer.width + w].Attributes = color;
			Window.buffer.screen[h * Window.buffer.width - 1].Char.UnicodeChar = character;
			Window.buffer.screen[h * Window.buffer.width - 1].Attributes = color;
		};
	}
};

void Sonic::Print(window window, wstring phrase, short color, int width, int height)
{
	for (int i = 0; i < phrase.size(); i++)
	{
		window.buffer.screen[(height - 1) * window.buffer.width + (width - 1) + i].Char.UnicodeChar = phrase.at(i);
		window.buffer.screen[(height - 1) * window.buffer.width + (width - 1) + i].Attributes = color;
	}
};

void Sonic::PrintNumber(window window, int number, short color, int width, int height)
{
	wchar_t Number[1000];
	wsprintf(&Number[0], L"%d", number);

	wstring No;
	for (int i = 0; i < 1000; i++)
	{
		No.push_back(L' ');
		if (Number[i] == L'\0')
		{
			No.pop_back();
			break;
		}
		No.at(i) = Number[i];
	}
	Print(window, No, color, width, height);
}

void Sonic::ReplaceCharacters(window window, wchar_t character1, wchar_t character2)
{
	for (int p = 0; p < window.buffer.height * window.buffer.width; p++)
	{
		if (window.buffer.screen[p].Char.UnicodeChar == character1)
		{
			window.buffer.screen[p].Char.UnicodeChar = character2;
		}
	}
};

void Sonic::Set_FPS(int frames)
{
	Sleep(1000 / frames);
}

void Sonic::SwapBuffers()
{
	WriteConsoleOutputW(CurrentWindow.hBufferConsole, CurrentWindow.buffer.screen, { (short)CurrentWindow.buffer.width, (short)CurrentWindow.buffer.height }, { 0, 0 }, &CurrentWindow.dwBufferBytesWritten);
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
	CurrentWindow.name = window.name;
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

wstring Sonic::MakeStringWide(string phrase)
{
	wstring Phrase;

	for (int i = 0; i < phrase.size(); i++)
	{
		Phrase += L" ";
		Phrase.at(i) = (wchar_t)phrase.at(i);
	}
	return Phrase;
}

int Sonic::GetPos(int width, int height)
{
	return height * CurrentWindow.buffer.width + width;
}

#endif