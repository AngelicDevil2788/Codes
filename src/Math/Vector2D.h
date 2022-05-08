#pragma once

class Vector2D
{
public:
	Vector2D() : m_x(0), m_y(0)
	{};
	Vector2D(float x, float y) : m_x(x), m_y(y)
	{};
	Vector2D(const Vector2D& source) : m_x(source.m_x), m_y(source.m_y)
	{};

	~Vector2D()
	{};
public:
	float m_x, m_y;
};