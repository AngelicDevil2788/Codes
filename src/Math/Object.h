#pragma once
#include "ObjectSample.h"
#include <vector>
#include <iostream>
class Object
{
public:
	Object()
	{};
	Object(std::vector<ObjectSample> mesh) : m_mesh(mesh)
	{};
	Object(const Object& source) : m_mesh(source.m_mesh)
	{};
	~Object()
	{};
public:
	std::vector<ObjectSample> m_mesh;
};