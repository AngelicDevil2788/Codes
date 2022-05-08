#pragma once
#include "Vector3D.h"
#include "Pixel.h"
class ObjectSample
{
public:
	ObjectSample() : m_corner1(Vector3D(0, 0, 0)), m_corner2(Vector3D(0, 0, 0)), m_corner3(Vector3D(0, 0, 0)), m_color(Pixel(0, 0, 0)), m_full(false)
	{};
	ObjectSample(Vector3D corner1, Vector3D corner2, Vector3D corner3, Pixel color, bool full) : m_corner1(corner1), m_corner2(corner2), m_corner3(corner3), m_color(color), m_full(full)
	{};
	ObjectSample(const ObjectSample& source) : m_corner1(source.m_corner1), m_corner2(source.m_corner2), m_corner3(source.m_corner3), m_color(source.m_color), m_full(source.m_full)
	{};
	~ObjectSample()
	{};
public:
	Vector3D m_corner1;
	Vector3D m_corner2;
	Vector3D m_corner3;
	Pixel m_color;
	bool m_full;
};