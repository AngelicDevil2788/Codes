#pragma once
#include "Vector2D.h"
#include <iostream>
#include <vector>

class Vector2DBuffer
{
public:
	Vector2DBuffer()
	{};
	Vector2DBuffer(std::vector<Vector2D> source) : m_buffer(source)
	{};

	~Vector2DBuffer()
	{};
public:
	std::vector<Vector2D> m_buffer;
};