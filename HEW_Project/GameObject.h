#pragma once
#include <DirectXMath.h>
#include "Collision.h"
#include"HCollision.h"
#include"col.h"

class GameObject 
{
public:
	GameObject();
	virtual ~GameObject();
	// úﬂàÀÇ≈Ç´ÇÈÉuÉçÉbÉN
	bool IsCollidingWith(GameObject& other);
	bool HIsCollidingWith(GameObject& other);
	bool col(GameObject & other);

	

protected:
	
	DirectX::XMFLOAT3 minBound;
	DirectX::XMFLOAT3 maxBound;

	DirectX::XMFLOAT3 hminBound;
	DirectX::XMFLOAT3 hmaxBound;


	DirectX::XMFLOAT3 cminBound;
	DirectX::XMFLOAT3 cmaxBound;
	
private:
	AABB m_aabb;
	HAABB m_haabb;
	CAABB m_caabb;
};
