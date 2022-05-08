#pragma once

class Pixel
{
public:
	Pixel() : m_r(0), m_g(0), m_b(0)
	{};
	Pixel(int r, int g, int b) : m_r(r), m_g(g), m_b(b)
	{}
	Pixel(const Pixel& source) : m_r(source.m_r), m_g(source.m_g), m_b(source.m_b)
	{};

	~Pixel()
	{};
public:
	int m_r, m_g, m_b;
};