#pragma once


#ifndef SONIC_GEOMETRY.H
#define SONIC_GEOMETRY_H

#include "Sonic.h"

namespace Sonic
{
	namespace Geometry
	{
		struct COORDS
		{
			int w = 0;
			int h = 0;
		};

		void Draw(window window, short character, short color, COORDS Pos);
		void DrawLine(window window, short character, short color, COORDS Pos1, COORDS Pos2);

		struct triangle
		{
			short filling = SN_PIXEL_TYPE::PIXEL_SOLID;
			short color = SN_COLOR::FULL_BLACK;
			bool full = false;
			COORDS C1;
			COORDS C2;
			COORDS C3;
			void draw(window window)
			{
				if (!full)
				{
					DrawLine(window, filling, color, { C1.w, C1.h }, { C2.w, C2.h });
					DrawLine(window, filling, color, { C2.w, C2.h }, { C3.w, C3.h });
					DrawLine(window, filling, color, { C3.w, C3.h }, { C1.w, C1.h });
				}
				else
				{
					auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
					auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(window, filling, color, { i, ny }); };

					int t1x, t2x, y, minx, maxx, t1xp, t2xp;
					bool changed1 = false;
					bool changed2 = false;
					int signx1, signx2, dx1, dy1, dx2, dy2;
					int e1, e2;
					// Sort vertices
					if (C1.h > C2.h) { SWAP(C1.h, C2.h); SWAP(C1.w, C2.w); }
					if (C1.h > C3.h) { SWAP(C1.h, C3.h); SWAP(C1.w, C3.w); }
					if (C2.h > C3.h) { SWAP(C2.h, C3.h); SWAP(C2.w, C3.w); }

					t1x = t2x = C1.w; y = C1.h;   // Starting points
					dx1 = (int)(C2.w - C1.w); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
					else signx1 = 1;
					dy1 = (int)(C2.h - C1.h);

					dx2 = (int)(C3.w - C1.w); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
					else signx2 = 1;
					dy2 = (int)(C3.h - C1.h);

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
					if (C1.h == C2.h) goto next;
					e1 = (int)(dx1 >> 1);

					for (int i = 0; i < dx1;) {
						t1xp = 0; t2xp = 0;
						if (t1x < t2x) { minx = t1x; maxx = t2x; }
						else { minx = t2x; maxx = t1x; }
						// process first line until y value is about to change
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
						// Move line
					next1:
						// process second line until y value is about to change
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
						drawline(minx, maxx, y);    // Draw line from min to max points found on the y
													 // Now increase y
						if (!changed1) t1x += signx1;
						t1x += t1xp;
						if (!changed2) t2x += signx2;
						t2x += t2xp;
						y += 1;
						if (y == C2.h) break;

					}
				next:
					// Second half
					dx1 = (int)(C3.w - C2.w); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
					else signx1 = 1;
					dy1 = (int)(C3.h - C2.h);
					t1x = C2.w;

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
						// process first line until y value is about to change
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
						// process second line until y value is about to change
						while (t2x != C3.w) {
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
						drawline(minx, maxx, y);
						if (!changed1) t1x += signx1;
						t1x += t1xp;
						if (!changed2) t2x += signx2;
						t2x += t2xp;
						y += 1;
						if (y > C3.h) return;
					}
				}
			}
		};

		struct circle
		{
			short filling = SN_PIXEL_TYPE::PIXEL_SOLID;
			short color = SN_PIXEL_TYPE::PIXEL_SOLID;
			bool full = false;
			COORDS C;
			int r = 0;
			void draw(window window)
			{
				if (!full)
				{
					int x = 0;
					int y = r;
					int p = 3 - 2 * r;
					if (!r) return;

					while (y >= x) // only formulate 1/8 of circle
					{
						Draw(window, filling, color, { C.w - x, C.h - y });//upper left left
						Draw(window, filling, color, { C.w - y, C.h - x });//upper upper left
						Draw(window, filling, color, { C.w + y, C.h - x });//upper upper right
						Draw(window, filling, color, { C.w + x, C.h - y });//upper right right
						Draw(window, filling, color, { C.w - x, C.h + y });//lower left left
						Draw(window, filling, color, { C.w - y, C.h + x });//lower lower left
						Draw(window, filling, color, { C.w + y, C.h + x });//lower lower right
						Draw(window, filling, color, { C.w + x, C.h + y });//lower right right
						if (p < 0) p += 4 * x++ + 6;
						else p += 4 * (x++ - y--) + 10;
					}
				}
				else
				{
					// Taken from wikipedia
					int x = 0;
					int y = r;
					int p = 3 - 2 * r;
					if (!r) return;

					auto drawline = [&](int sx, int ex, int ny)
					{
						for (int i = sx; i <= ex; i++)
							Draw(window, filling, color, { i, ny });
					};

					while (y >= x)
					{
						// Modified to draw scan-lines instead of edges
						drawline(C.w - x, C.w + x, C.h - y);
						drawline(C.w - y, C.w + y, C.h - x);
						drawline(C.w - x, C.w + x, C.h + y);
						drawline(C.w - y, C.w + y, C.h + x);
						if (p < 0) p += 4 * x++ + 6;
						else p += 4 * (x++ - y--) + 10;
					}
				}
			}
		};

		struct rectangle
		{
			short filling = SN_PIXEL_TYPE::PIXEL_SOLID;
			short color = SN_COLOR::FULL_BLACK;
			bool full = false;
			COORDS C1;
			COORDS C2;
			void draw(window window)
			{
				DrawLine(window, filling, color, { C1.w, C2.h }, C1);
				DrawLine(window, filling, color, { C2.w, C1.h }, C1);
				DrawLine(window, filling, color, { C1.w, C2.h }, C2);
				DrawLine(window, filling, color, { C2.w, C1.h }, C2);
				if (full)
				{
					int w = 0;
					int h = 0;
					if (C2.w > C1.w)
					{
						w = C2.w - C1.w;
						if (C2.h > C1.h)
						{
							h = C2.h - C1.h;
						}
						else if (C2.h < C1.h)
						{
							h = C1.h < C2.h;
						}
					}
					else if (C2.w < C1.w)
					{
						w = C1.w - C2.w;
						if (C2.h > C1.h)
						{
							h = C2.h - C1.h;
						}
						else if (C2.h < C1.h)
						{
							h = C1.h < C2.h;
						}
					}
					for (int H = 0; H < h; H++)
					{
							DrawLine(window, filling, color, { C1.w, C1.h + H }, { C2.w, C2.h + H });
					}
				}
			}
		};
	}
}

void Sonic::Geometry::Draw(window window, short character, short color, COORDS Pos)
{
	window.buffer.screen[Pos.h * window.buffer.width + Pos.w].Char.UnicodeChar = character;
	window.buffer.screen[Pos.h * window.buffer.width + Pos.w].Attributes = color;
};



void Sonic::Geometry::DrawLine(window window, short character, short color, COORDS Pos1, COORDS Pos2)
{
	int x, y, dx, dy, dw1, dh1, px, py, xe, ye, i;
	dx = Pos2.w - Pos1.w; dy = Pos2.h - Pos1.h;
	dw1 = abs(dx); dh1 = abs(dy);
	px = 2 * dh1 - dw1;	py = 2 * dw1 - dh1;
	if (dh1 <= dw1)
	{
		if (dx >= 0)
		{
			x = Pos1.w; y = Pos1.h; xe = Pos2.w;
		}
		else
		{
			x = Pos2.w; y = Pos2.h; xe = Pos1.w;
		}

		Draw(window, character, color, { x, y });

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
			Draw(window, character, color, { x, y });
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = Pos1.w; y = Pos1.h; ye = Pos2.h;
		}
		else
		{
			x = Pos2.w; y = Pos2.h; ye = Pos1.h;
		}

		Draw(window, character, color, { x, y });

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
			Draw(window, character, color, { x, y });
		}
	}
}

#endif // !SONIC_GEOMETRY.H