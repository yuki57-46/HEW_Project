#include "Lever.h"

Lever::Lever()
{

}

Lever::~Lever()
{

}

void Lever::Update()
{
	int LeverNum = 2;
	bool isUse = true;

	while (isUse)
	{
		switch (LeverNum)
		{
		// レバーを押して釣り床を上に上げる処理
		case 1:
			// オブジェクトのY軸にプラスを加える


			break;
		// レバーを押して釣り床を下に下げる処理
		case 2:
			// オブジェクトのY軸にマイナスを加える


			break;

		default:
			break;
		}
	}
}

void Lever::Draw()
{

}