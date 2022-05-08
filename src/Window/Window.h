#pragma region Window Class Definition
#include <windows.h>
#include <memory>
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <utility>
#include <algorithm>
#include <condition_variable>
#include <map>
#include <fstream>
#include "../Math/PixelBuffer.h"
#include "../Math/Vector2DBuffer.h"
#include "../Math/Vector3DBuffer.h"
#include "../Math/Matrix4x4.h"
#include "../Math/Object.h"

#define DEFAULT_WIN_WIDTH GetSystemMetrics(SM_CXSCREEN) + 15
#define DEFAULT_WIN_HEIGHT GetSystemMetrics(SM_CYSCREEN) - 30

struct WIN
{
private:
	int m_nScreenWidth = DEFAULT_WIN_WIDTH;
	int m_nScreenHeight = DEFAULT_WIN_HEIGHT;

	COLORREF* m_pixel_map = new COLORREF[(m_nScreenWidth - 16) * (m_nScreenHeight - 39)];
	HWND m_hWin;
	HDC hdc;
	HDC hdcMem;
	HINSTANCE m_hInstance = GetModuleHandle(nullptr);
	LPCWSTR name = L"";
	Pixel bg_Color = { 0, 0, 0 };
	bool open = false;
	HBITMAP bitMap = CreateBitmap(m_nScreenWidth - 16, m_nScreenHeight - 39, 1, 8 * 4, m_pixel_map);
	std::chrono::system_clock::time_point tp1;
	std::chrono::system_clock::time_point tp2;

	bool ProcessMessages();
public:

	int ScreenWidth();
	int ScreenHeight();

	bool create(Vector2D Size, LPCWSTR name);

	bool is_open();
	void close();
	bool UpdateWindow();
	float PrepareWindow();
	void SetBackgroundColor(Pixel color);
	void SetPixel(Vector2D position, Pixel color);
	void RenderLine(Vector2DBuffer positions, Pixel color);
	void RenderTriangle(Vector2DBuffer positions, Pixel color);
	void RenderFullTriangle(Vector2DBuffer positions, Pixel color);
	Pixel Get_Color(Vector2D Position)
	{
		return { (int)GetRValue(m_pixel_map[(int)Position.m_y * m_nScreenWidth + (int)Position.m_x]), (int)GetGValue(m_pixel_map[(int)Position.m_y * m_nScreenWidth + (int)Position.m_x]), (int)GetBValue(m_pixel_map[(int)Position.m_y * m_nScreenWidth + (int)Position.m_x]) };
	};

};

#pragma endregion

#pragma region Window Function Definition

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool WIN::ProcessMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return true;
}

int WIN::ScreenWidth()
{
	return m_nScreenWidth - 16;
};

int WIN::ScreenHeight()
{
	return m_nScreenHeight - 39;
};

bool WIN::create(Vector2D Size, LPCWSTR name)
{
	this->m_nScreenWidth = Size.m_x;
	this->m_nScreenHeight = Size.m_y;
	this->name = name;
	this->tp1 = std::chrono::system_clock::now();
	this->tp2 = std::chrono::system_clock::now();
	open = true;
	LPCWSTR className = L"OBSIDIAN";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = className;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WndProc;

	if (!RegisterClass(&wndClass))
		return false;

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = m_nScreenHeight;
	rect.right = m_nScreenWidth;


	AdjustWindowRect(&rect, style, false);
	m_hWin = CreateWindowW((LPCWSTR)className, (LPCWSTR)name, style, rect.left, rect.top + 30, m_nScreenWidth, m_nScreenHeight, NULL, NULL, m_hInstance, NULL);
	ShowWindow(m_hWin, SW_SHOW);
	hdc = GetDC(m_hWin);
	hdcMem = CreateCompatibleDC(hdc);

	for (int i = 0; i < (m_nScreenWidth - 16) * (m_nScreenHeight - 39); i++)
	{
		m_pixel_map[i] = RGB(bg_Color.m_b, bg_Color.m_g, bg_Color.m_r);
	}
	return true;
};

bool WIN::is_open()
{
	return open;
};

void WIN::close()
{
	open = false;
};

bool WIN::UpdateWindow()
{
	BitBlt(hdc, 0, 0, (m_nScreenWidth - 16), (m_nScreenHeight - 39), hdcMem, 0, 0, SRCCOPY);
	SetWindowText(m_hWin, name);
	ShowWindow(m_hWin, SW_SHOW);
	return true;
}

float WIN::PrepareWindow()
{
	bitMap = CreateBitmap((m_nScreenWidth - 16), (m_nScreenHeight - 39), 1, 8 * 4, (void*)m_pixel_map);
	SelectObject(hdcMem, bitMap);
	for (int h = 0; h < m_nScreenHeight - 39; h++)
	{
		for (int w = 0; w < m_nScreenWidth - 16; w++)
		{
			SetPixel(Vector2D((float)w, (float)h), bg_Color);
		}
	}
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
	float fElapsedTime = elapsedTime.count();
	if (!ProcessMessages())
		open = false;
	return fElapsedTime * 100;
};

void WIN::SetBackgroundColor(Pixel color)
{
	bg_Color = color;
}

void WIN::SetPixel(Vector2D position, Pixel color)
{
	if (position.m_x < m_nScreenWidth - 16 && position.m_x > 0 && position.m_y < m_nScreenHeight - 39 && position.m_y > 0)
	{
		m_pixel_map[(int)position.m_y * (m_nScreenWidth - 16) + (int)position.m_x] = RGB(color.m_b, color.m_g, color.m_r);
	}
}

void WIN::RenderLine(Vector2DBuffer positions, Pixel color)
{
	Vector2D position1 = positions.m_buffer[0];
	Vector2D position2 = positions.m_buffer[1];
	int x, y, dx, dy, dw1, dh1, px, py, xe, ye, i;
	dx = position2.m_x - position1.m_x; dy = position2.m_y - position1.m_y;
	dw1 = abs(dx); dh1 = abs(dy);
	px = 2 * dh1 - dw1;	py = 2 * dw1 - dh1;
	if (dh1 <= dw1)
	{
		if (dx >= 0)
		{
			x = position1.m_x; y = position1.m_y; xe = position2.m_x;
		}
		else
		{
			x = position2.m_x; y = position2.m_y; xe = position1.m_x;
		}

		SetPixel(Vector2D((float)x, (float)y), color);

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
			SetPixel(Vector2D((float)x, (float)y), color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = position1.m_x; y = position1.m_y; ye = position2.m_y;
		}
		else
		{
			x = position2.m_x; y = position2.m_y; ye = position1.m_y;
		}

		SetPixel(Vector2D((float)x, (float)y), color);

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
			SetPixel(Vector2D((float)x, (float)y), color);
		}
	};
};

void WIN::RenderTriangle(Vector2DBuffer positions, Pixel color)
{
	Vector2DBuffer line1buffer;
	Vector2D* line1 = new Vector2D[2];
	line1[0] = positions.m_buffer[0];
	line1[1] = positions.m_buffer[1];
	line1buffer.m_buffer.push_back(line1[0]);
	line1buffer.m_buffer.push_back(line1[1]);

	Vector2DBuffer line2buffer;
	Vector2D* line2 = new Vector2D[2];
	line2[0] = positions.m_buffer[1];
	line2[1] = positions.m_buffer[2];
	line2buffer.m_buffer.push_back(line2[0]);
	line2buffer.m_buffer.push_back(line2[1]);

	Vector2DBuffer line3buffer;
	Vector2D* line3 = new Vector2D[2];
	line3[0] = positions.m_buffer[2];
	line3[1] = positions.m_buffer[0];
	line3buffer.m_buffer.push_back(line3[0]);
	line3buffer.m_buffer.push_back(line3[1]);

	RenderLine(line1buffer, color);
	RenderLine(line2buffer, color);
	RenderLine(line3buffer, color);
}

void WIN::RenderFullTriangle(Vector2DBuffer positions, Pixel color)
{
	Vector2D Position1 = positions.m_buffer[0];
	Vector2D Position2 = positions.m_buffer[1];
	Vector2D Position3 = positions.m_buffer[2];
	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
	auto SetPixelLine = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) SetPixel(Vector2D((float)i, (float)ny), color); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (Position1.m_y > Position2.m_y) { SWAP((int&)Position1.m_y, (int&)Position2.m_y); SWAP((int&)Position1.m_x, (int&)Position2.m_x); }
	if (Position1.m_y > Position3.m_y) { SWAP((int&)Position1.m_y, (int&)Position3.m_y); SWAP((int&)Position1.m_x, (int&)Position3.m_x); }
	if (Position2.m_y > Position3.m_y) { SWAP((int&)Position2.m_y, (int&)Position3.m_y); SWAP((int&)Position2.m_x, (int&)Position3.m_x); }

	t1x = t2x = Position1.m_x; y = Position1.m_y;   // Starting points
	dx1 = (int)(Position2.m_x - Position1.m_x); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(Position2.m_y - Position1.m_y);

	dx2 = (int)(Position3.m_x - Position1.m_x); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(Position3.m_y - Position1.m_y);

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
	if (Position1.m_y == Position2.m_y) goto next;
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
		SetPixelLine(minx, maxx, y);    // SetPixel Line from min to max points found on the y
									 // Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == Position2.m_y) break;

	}
next:
	// Second half
	dx1 = (int)(Position3.m_x - Position2.m_x); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(Position3.m_y - Position2.m_y);
	t1x = Position2.m_x;

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
		while (t2x != Position3.m_x) {
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
		SetPixelLine(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > Position3.m_y) return;
	}
}

#pragma endregion