#include "GameObject.h"

GameObject::GameObject()
	
{
}

GameObject::~GameObject()
{
}

bool GameObject::IsCollidingWith(GameObject& other)
{
	//Collision���̓����蔻��̊֐���Ԃ�
	return m_aabb.IsAABBIntersecting(minBound, maxBound, other.minBound, other.maxBound);
}

bool GameObject::HIsCollidingWith(GameObject& other)
{
	//Collision���̓����蔻��̊֐���Ԃ�
	return m_haabb.HIsAABBIntersecting(hminBound, hmaxBound, other.hminBound, other.hmaxBound);
}

bool GameObject::col(GameObject & other)
{
	return m_caabb.col(hminBound, hmaxBound, other.hminBound, other.hmaxBound);
}




