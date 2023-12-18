#include "GameObject.h"

GameObject::GameObject()
	
{
}

GameObject::~GameObject()
{
}

bool GameObject::IsCollidingWith(GameObject& other)
{
	//Collision“à‚Ì“–‚½‚è”»’è‚ÌŠÖ”‚ğ•Ô‚·
	return m_aabb.IsAABBIntersecting(minBound, maxBound, other.minBound, other.maxBound);
}

bool GameObject::HIsCollidingWith(GameObject& other)
{
	//Collision“à‚Ì“–‚½‚è”»’è‚ÌŠÖ”‚ğ•Ô‚·
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




