#pragma once
#include "sonic_main.h"

namespace Sonic
{
	namespace _3d_
	{
		struct vec3d
		{
			float w, h, l;
		};

		struct triangle
		{
			vec3d Pos1;
			vec3d Pos2;
			vec3d Pos3;
		};

		struct mesh
		{
			vector<triangle> Coords;
		};
		struct Matrix
		{
		private:
			int nMapWidth = 674;
			int nMapHeight = 346;
			int nMapLength = 1000;
		public:
			Matrix(SNWIN win) : nMapWidth(win.m_ScreenWidth), nMapHeight(win.m_ScreenHeight)
			{};
		};
		Matrix* ACTIVEMATRIX;
		void Init(Matrix &mat)
		{
			ACTIVEMATRIX = &mat;
		}
		void Draw_Mesh(mesh mesh, short color)
		{
			vector<triangle> mesh2 = mesh.Coords;
			float fRatio = (*ACTIVEWIN).m_ScreenWidth / (*ACTIVEWIN).m_ScreenHeight;
			for (auto tri : mesh2)
			{
				Draw_Triangle({ (*ACTIVEWIN).m_ScreenWidth / 2 + (int)(tri.Pos1.w * (tri.Pos1.l * fRatio)), (*ACTIVEWIN).m_ScreenHeight / 2 + (int)(tri.Pos1.h * (tri.Pos1.l * fRatio)) }, { (*ACTIVEWIN).m_ScreenWidth / 2 + (int)(tri.Pos2.w * (tri.Pos2.l * fRatio)), (*ACTIVEWIN).m_ScreenHeight / 2 + (int)(tri.Pos2.h * (tri.Pos2.l * fRatio)) }, { (*ACTIVEWIN).m_ScreenWidth / 2 + (int)(tri.Pos3.w * (tri.Pos3.l * fRatio)), (*ACTIVEWIN).m_ScreenHeight / 2 + (int)(tri.Pos3.h * (tri.Pos3.l * fRatio)) }, color);
			}
		}
	}
};