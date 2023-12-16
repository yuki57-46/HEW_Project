#include "col.h"

//�I�u�W�F�N�g���m�̏Փ˔���
bool CAABB::col(const DirectX::XMFLOAT3 &minA, const DirectX::XMFLOAT3 &maxA, const DirectX::XMFLOAT3 &minB, const DirectX::XMFLOAT3 &maxB) {
	return (minA.x <= maxB.x) && (maxA.x >= minB.x)&&
		(minA.y <= maxB.y && maxA.y >= minB.y) &&
		(minA.z <= maxB.z && maxA.z >= minB.z);
}