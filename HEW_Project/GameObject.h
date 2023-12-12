#pragma once
#include <DirectXMath.h>
#include "Collision.h"
#include "HCollision.h"
#include "col.h"

class GameObject 
{
public:
	GameObject();
	virtual ~GameObject();
	bool IsCollidingWith(GameObject& other);
	bool HIsCollidingWith(GameObject& other);
	bool col(GameObject & other);

protected:
	
	// œßˆË‚Å‚«‚é—p(HBLOCK)
	DirectX::XMFLOAT3 minBound;
	DirectX::XMFLOAT3 maxBound;

	DirectX::XMFLOAT3 hminBound;
	DirectX::XMFLOAT3 hmaxBound;

	DirectX::XMFLOAT3 cminBound;
	DirectX::XMFLOAT3 cmaxBound;
	
	// œßˆË‚Å‚«‚È‚¢—p(DBLOCK)
	//DirectX::XMFLOAT3 minBoundNot;
	//DirectX::XMFLOAT3 maxBoundNot;

	//DirectX::XMFLOAT3 cminBoundNot;
	//DirectX::XMFLOAT3 cmaxBoundNot;

	//// œßˆË‚Å‚«‚È‚¢—p(MBLOCK)
	//DirectX::XMFLOAT3 minBoundAuto;
	//DirectX::XMFLOAT3 maxBoundAuto;

	//DirectX::XMFLOAT3 cminBoundAuto;
	//DirectX::XMFLOAT3 cmaxBoundAuto;

private:
	AABB m_aabb;
	HAABB m_haabb;
	CAABB m_caabb;
};
