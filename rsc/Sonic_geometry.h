#pragma once


#ifndef SONIC_GEOMETRY.H
#define SONIC_GEOMETRY_H

#include "Sonic.h"

namespace Sonic
{
	namespace Geometry
	{

		struct rectangle
		{
			int PosWidth = 0;
			int PosHeight = 0;
			int length = 227;
			int width = 70;
			bool transparent = false;
			wchar_t* Background = new wchar_t[length - 1 * width - 1];
		};

		struct shader
		{
			int width = 0;
			int height = 0;
			wchar_t* texture = new wchar_t[width * height];
			wstring id = L"NONE";
			void Fill(wchar_t Brush)
			{
				for (int i = 0; i < width * height; i++)
				{
					texture[i] = Brush;
				}
			};
			void SetBorders(wchar_t Brush)
			{
				for (int h = 0; h < height; h++)
				{
					for (int w = 0; w < width; w++)
					{
						texture[h * width] = Brush;
						texture[h * width + width - 1] = Brush;
						texture[height * width - w] = Brush;
						texture[w] = Brush;
					}
				}
			};
		};

		void PutShadeOnShape(rectangle shape, shader shade);

		void DrawShape(window window, rectangle rectangle);
	}
}

void Sonic::Geometry::DrawShape(window window, rectangle rectangle)
{
	int Pos = (rectangle.PosHeight - 1) * window.buffer.width + (rectangle.PosWidth - 1);
	for (int h = 1; h < rectangle.width; h++)
	{
		for (int w = 0; w < rectangle.length; w++)
		{
			if (rectangle.transparent == false)
			{
				window.buffer.Pixel_[Pos + h * window.buffer.width + w] = true;
				window.buffer.Pixel_[Pos + w] = true;
			}
			window.buffer.screen[Pos + h * window.buffer.width + w] = rectangle.Background[h * rectangle.length + w];
			window.buffer.screen[Pos + w] = rectangle.Background[w];
		}
	}
};

void Sonic::Geometry::PutShadeOnShape(rectangle shape, shader shade)
{
	for (int i = 0; i < shape.width * shape.length; i++)
	{
		shape.Background[i] = shade.texture[i];
	}
}

#endif // !SONIC_GEOMETRY.H
