#include "GameObject.h"

GameObject::GameObject()
	
{
}

GameObject::~GameObject()
{
}

bool GameObject::IsCollidingWith(GameObject& other)
{
	//Collision内の当たり判定の関数を返す
	return m_aabb.IsAABBIntersecting(minBound, maxBound, other.minBound, other.maxBound);
}

bool GameObject::HIsCollidingWith(GameObject& other)
{
	//Collision内の当たり判定の関数を返す
	return m_haabb.HIsAABBIntersecting(hminBound, hmaxBound, other.hminBound, other.hmaxBound);
}

bool GameObject::col(GameObject & other)
{
	return m_caabb.col(cminBound, cmaxBound, other.cminBound, other.cmaxBound);
}

DirectX::XMFLOAT3 GameObject::GetCMinBounds()
{
	return cminBound;
}

DirectX::XMFLOAT3 GameObject::GetMaxBounds()
{
	return cmaxBound;
}




