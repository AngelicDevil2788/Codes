#include "Functions.h"

Sonic Sn;

int main()
{
	Sn.buffer.Create(170, 44);
	while (true)
	{
		Sn.player.Create(L"Hello");
		Sn.buffer.Type(1, 1, Sn.player.name);
		Sn.buffer.Display();
	}
}