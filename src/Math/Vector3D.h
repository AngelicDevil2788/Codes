#pragma once

class Vector3D
{
public:
	Vector3D() : m_x(0), m_y(0), m_z(0)
	{};
	Vector3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
	{};
	Vector3D(const Vector3D& source) : m_x(source.m_x), m_y(source.m_y), m_z(source.m_z)
	{};

	~Vector3D()
	{};
public:
	float m_x, m_y, m_z;
};