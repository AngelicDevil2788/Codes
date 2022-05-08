#pragma region Game Engine Class Definition
#include "../Window/Window.h"
#include <iostream>

class GameEngine
{
public:
	GameEngine();

	bool release();
	bool init(Vector2D window_size, LPCWSTR name);

	bool running();

	~GameEngine();
public:
	WIN window;
	struct constant
	{
		Matrix4x4 m_world;
		Matrix4x4 m_view;
		Matrix4x4 m_projection;
	};
	constant cc;
	Vector3D CameraPosition;
	static GameEngine* get();
	void RenderCircle(Vector2D Position, int Radius, Pixel color);
	void RenderFullCircle(Vector2D Position, int Radius, Pixel color);
	void MultiplyMatrixVector(Vector3D& input, Vector3D& output, Matrix4x4& matrix);
	void RenderObject(Object);
private:
	friend class Pixel;
	friend class PixelBuffer;
	friend class Vector2D;
	friend class Vector2DBuffer;
	friend class Vector3D;
	friend class Vector3DBuffer;
	friend class Matrix4x4;
	friend class ObjectSample;
	friend class Object;
};

#pragma endregion

#pragma region Game Engine Function Definition

GameEngine::GameEngine()
{};


bool GameEngine::release()
{
	return true;
}

bool GameEngine::init(Vector2D window_size, LPCWSTR name)
{
	return window.create(window_size, name);
}

bool GameEngine::running()
{
	return window.is_open();
}

GameEngine::~GameEngine()
{};

GameEngine* GameEngine::get()
{
	static GameEngine engine;
	return &engine;
}

void GameEngine::RenderCircle(Vector2D Position, int Radius, Pixel color)
{
	int x = 0;
	int y = Radius;
	int p = 3 - 2 * Radius;
	if (!Radius) return;

	while (y >= x) // only formulate 1/8 of Circle
	{
		window.SetPixel(Vector2D(Position.m_x - x, Position.m_y - y ), color);//upper left left
		window.SetPixel(Vector2D(Position.m_x - x, Position.m_y - y ), color);//upper upper left
		window.SetPixel(Vector2D(Position.m_x + x, Position.m_y - y ), color);//upper upper right
		window.SetPixel(Vector2D(Position.m_x + x, Position.m_y - y ), color);//upper right right
		window.SetPixel(Vector2D(Position.m_x - x, Position.m_y + y ), color);//lower left left
		window.SetPixel(Vector2D(Position.m_x - x, Position.m_y + y ), color);//lower lower left
		window.SetPixel(Vector2D(Position.m_x + x, Position.m_y + y ), color);//lower lower right
		window.SetPixel(Vector2D(Position.m_x + x, Position.m_y + y ), color);//lower right right
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void GameEngine::RenderFullCircle(Vector2D Position, int Radius, Pixel color)
{
	int x = 0;
	int y = Radius;
	int p = 3 - 2 * Radius;
	if (!Radius) return;

	auto drawLine = [&](int sx, int ex, int ny)
	{
		for (int i = sx; i <= ex; i++)
			window.SetPixel(Vector2D( i, ny ), color);
	};

	while (y >= x)
	{

		// Modified to draw scan-Lines instead of edges
		drawLine(Position.m_x - x, Position.m_x + x, Position.m_y - y);
		drawLine(Position.m_x - y, Position.m_x + y, Position.m_y - x);
		drawLine(Position.m_x - x, Position.m_x + x, Position.m_y + y);
		drawLine(Position.m_x - y, Position.m_x + y, Position.m_y + x);
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void GameEngine::MultiplyMatrixVector(Vector3D& input, Vector3D& output, Matrix4x4& matrix)
{
	output.m_x = input.m_x * matrix.m_mat[0][0] + input.m_y * matrix.m_mat[1][0] + input.m_z * matrix.m_mat[2][0] + matrix.m_mat[3][0];
	output.m_y = input.m_x * matrix.m_mat[0][1] + input.m_y * matrix.m_mat[1][1] + input.m_z * matrix.m_mat[2][1] + matrix.m_mat[3][1];
	output.m_z = input.m_x * matrix.m_mat[0][2] + input.m_y * matrix.m_mat[1][2] + input.m_z * matrix.m_mat[2][2] + matrix.m_mat[3][2];
	int w = input.m_x * matrix.m_mat[0][3] + input.m_y * matrix.m_mat[1][3] + input.m_z * matrix.m_mat[2][3] + matrix.m_mat[3][3];

	if (w != 0.0f)
	{
		output.m_x /= w; output.m_y /= w; output.m_z /= w;
	}
}

void GameEngine::RenderObject(Object source)
{
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = (float)window.ScreenHeight() / (float)window.ScreenWidth();
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	Matrix4x4 matProj;

	matProj.m_mat[0][0] = fAspectRatio * fFovRad;
	matProj.m_mat[1][1] = fFovRad;
	matProj.m_mat[2][2] = fFar / (fFar - fNear);
	matProj.m_mat[3][2] = (-fFar * fNear) / (fFar - fNear);
	matProj.m_mat[2][3] = 1.0f;
	matProj.m_mat[3][3] = 0.0f;

	for (auto tri : source.m_mesh)
	{


		ObjectSample triProjected, triTranslated;

		// Offset into the screen
		triTranslated = tri;
		triTranslated.m_corner1.m_z = tri.m_corner1.m_z + 3.f;
		triTranslated.m_corner2.m_z = tri.m_corner2.m_z + 3.f;
		triTranslated.m_corner3.m_z = tri.m_corner3.m_z + 3.f;

		Vector3D normal, line1, line2;
		line1.m_x = triTranslated.m_corner2.m_x - triTranslated.m_corner1.m_x;
		line1.m_y = triTranslated.m_corner2.m_y - triTranslated.m_corner1.m_y;
		line1.m_z = triTranslated.m_corner2.m_z - triTranslated.m_corner1.m_z;

		line2.m_x = triTranslated.m_corner3.m_x - triTranslated.m_corner1.m_x;
		line2.m_y = triTranslated.m_corner3.m_y - triTranslated.m_corner1.m_y;
		line2.m_z = triTranslated.m_corner3.m_z - triTranslated.m_corner1.m_z;

		normal.m_x = line1.m_y * line2.m_z - line1.m_z * line2.m_y;
		normal.m_y = line1.m_z * line2.m_x - line1.m_x * line2.m_z;
		normal.m_z = line1.m_x * line2.m_y - line1.m_y * line2.m_x;

		float l = sqrtf(normal.m_x * normal.m_x + normal.m_y * normal.m_y + normal.m_z * normal.m_z);
		normal.m_x /= l; normal.m_y /= l; normal.m_z /= l;

		if (CameraPosition.m_x > triTranslated.m_corner1.m_x)
		{
			triTranslated.m_corner1.m_x -= CameraPosition.m_x;
		}
		else if (CameraPosition.m_x < triTranslated.m_corner1.m_x)
		{
			triTranslated.m_corner1.m_x += CameraPosition.m_x;
		}

		if (CameraPosition.m_x > triTranslated.m_corner2.m_x)
		{
			triTranslated.m_corner2.m_x -= CameraPosition.m_x;
		}
		else if (CameraPosition.m_x < triTranslated.m_corner2.m_x)
		{
			triTranslated.m_corner2.m_x += CameraPosition.m_x;
		}

		if (CameraPosition.m_x > triProjected.m_corner3.m_x)
		{
			triTranslated.m_corner3.m_x -= CameraPosition.m_x;
		}
		else if (CameraPosition.m_x < triProjected.m_corner3.m_x)
		{
			triTranslated.m_corner3.m_x += CameraPosition.m_x;
		}

		if (normal.m_x * (triTranslated.m_corner1.m_x - CameraPosition.m_x) +
			normal.m_y * (triTranslated.m_corner1.m_y - CameraPosition.m_y) +
			normal.m_z * (triTranslated.m_corner1.m_z - CameraPosition.m_z) < 0.0f)
		{
			// Project triangles from 3D --> 2D
			MultiplyMatrixVector(triTranslated.m_corner1, triProjected.m_corner1, matProj);
			MultiplyMatrixVector(triTranslated.m_corner2, triProjected.m_corner2, matProj);
			MultiplyMatrixVector(triTranslated.m_corner3, triProjected.m_corner3, matProj);

			// Scale into view
			triProjected.m_corner1.m_x += 1.0f; triProjected.m_corner1.m_y += 1.0f;
			triProjected.m_corner2.m_x += 1.0f; triProjected.m_corner2.m_y += 1.0f;
			triProjected.m_corner3.m_x += 1.0f; triProjected.m_corner3.m_y += 1.0f;
			triProjected.m_corner1.m_x *= 0.5f * (float)window.ScreenWidth();
			triProjected.m_corner1.m_y *= 0.5f * (float)window.ScreenHeight();
			triProjected.m_corner2.m_x *= 0.5f * (float)window.ScreenWidth();
			triProjected.m_corner2.m_y *= 0.5f * (float)window.ScreenHeight();
			triProjected.m_corner3.m_x *= 0.5f * (float)window.ScreenWidth();
			triProjected.m_corner3.m_y *= 0.5f * (float)window.ScreenHeight();




			// Rasterize triangle
			Vector2DBuffer triBuffer;
			triBuffer.m_buffer.push_back(Vector2D(triProjected.m_corner1.m_x, triProjected.m_corner1.m_y));
			triBuffer.m_buffer.push_back(Vector2D(triProjected.m_corner2.m_x, triProjected.m_corner2.m_y));
			triBuffer.m_buffer.push_back(Vector2D(triProjected.m_corner3.m_x, triProjected.m_corner3.m_y));

			window.RenderTriangle(triBuffer, tri.m_color);

		}
	}

}

#pragma endregion