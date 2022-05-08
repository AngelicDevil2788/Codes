#pragma once
#include <memory>
#include "Vector3D.h"

class Matrix4x4
{
public:
	Matrix4x4() : m_mat()
	{
	};
	Matrix4x4(const Matrix4x4& source) : m_mat()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_mat[i][j] = source.m_mat[i][j];
			}
		}
	};

	void setIdentity()
	{
		memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTranslation(const Vector3D& reference)
	{
		setIdentity();
		m_mat[3][0] = reference.m_x;
		m_mat[3][1] = reference.m_y;
		m_mat[3][2] = reference.m_z;
	}

	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2 / width;
		m_mat[1][1] = 2 / height;
		m_mat[2][2] = 1 - (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

	~Matrix4x4()
	{};
public:
	float m_mat[4][4];
};