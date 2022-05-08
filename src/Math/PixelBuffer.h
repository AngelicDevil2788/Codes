#pragma region PixelBuffer Class Definition
#include "Pixel.h"

class PixelBuffer
{
public:
	PixelBuffer();
	PixelBuffer(int size);

	~PixelBuffer();
public:
	int m_size;
	Pixel* m_buffer = new Pixel[m_size];
};

#pragma endregion

#pragma region PixelBuffer Function Definition

PixelBuffer::PixelBuffer() : m_size(1)
{
	for (int i = 0; i < m_size; i++)
	{
		m_buffer[i] = { 0, 0, 0 };
	}
};

PixelBuffer::PixelBuffer(int size) : m_size(size)
{
	for (int i = 0; i < m_size; i++)
	{
		m_buffer[i] = { 0, 0, 0 };
	}
}

PixelBuffer::~PixelBuffer()
{};

#pragma endregion