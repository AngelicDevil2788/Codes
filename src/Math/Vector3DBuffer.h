#pragma once
#include "Vector3D.h"
#include <vector>
#include <iostream>
class Vector3DBuffer
{
public:
	Vector3DBuffer()
	{};
	Vector3DBuffer(std::vector<Vector3D> source) : m_buffer(source)
	{};

	~Vector3DBuffer()
	{};
public:
	std::vector<Vector3D> m_buffer;
};