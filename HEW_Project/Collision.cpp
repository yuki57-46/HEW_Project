#include "Collision.h"

// プレイヤーとボックスの当たり判定
bool AABB::IsAABBIntersecting(const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA, const DirectX::XMFLOAT3& minB, const DirectX::XMFLOAT3& maxB) {
	return (minA.x <= maxB.x && maxA.x >= minB.x) &&
		(minA.y <= maxB.y && maxA.y >= minB.y) &&
		(minA.z <= maxB.z && maxA.z >= minB.z);
}

// プレイヤーとボックスの当たり判定(憑依できないブロック専用)
bool AABB::IsNotAABBIntersecting(const DirectX::XMFLOAT3& minA_1, const DirectX::XMFLOAT3& maxA_1, const DirectX::XMFLOAT3& minB_1, const DirectX::XMFLOAT3& maxB_1) {
	return (minA_1.x <= maxB_1.x && maxA_1.x >= minB_1.x) &&
		(minA_1.y <= maxB_1.y && maxA_1.y >= minB_1.y) &&
		(minA_1.z <= maxB_1.z && maxA_1.z >= minB_1.z);
}