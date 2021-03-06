#pragma once
#ifndef SONIC_DS.H
#define SONIC_DS_H


#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <wchar.h>
#include <chrono>
#include <atomic>

using namespace std;

#define SN_MAX_WIDTH 679
#define SN_MAX_HEIGHT 400

enum SN_COLOR {
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,

	FULL_BLACK = 0x0000 + 0x0000,
	FULL_DARK_BLUE = 0x0001 + 0x0010,
	FULL_DARK_GREEN = 0x0002 + 0x0020,
	FULL_DARK_CYAN = 0x0003 + 0x0030,
	FULL_DARK_RED = 0x0004 + 0x0040,
	FULL_DARK_MAGENTA = 0x0005 + 0x0050,
	FULL_DARK_YELLOW = 0x0006 + 0x0060,
	FULL_DARK_GREY = 0x0008 + 0x0080,
	FULL_GREY = 0x0007 + 0x0080,
	FULL_BLUE = 0x0009 + 0x0090,
	FULL_GREEN = 0x000A + 0x00A0,
	FULL_CYAN = 0x000B + 0x00B0,
	FULL_RED = 0x000C + 0x00C0,
	FULL_MAGENTA = 0x000D + 0x00D0,
	FULL_YELLOW = 0x000E + 0x00E0,
	FULL_WHITE = 0x000F + 0x00F0,
};

enum SN_PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591
};

#endif // !SONIC_DS_H
