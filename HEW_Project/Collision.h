#pragma once
#include <DirectXMath.h>
class AABB
{
public:
	bool IsAABBIntersecting(const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA, const DirectX::XMFLOAT3& minB, const DirectX::XMFLOAT3& maxB);
	
	bool IsNotAABBIntersecting(const DirectX::XMFLOAT3& minA_1, const DirectX::XMFLOAT3& maxA_1, const DirectX::XMFLOAT3& minB_1, const DirectX::XMFLOAT3& maxB_1);


private:
};
