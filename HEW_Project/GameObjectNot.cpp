// �߈˂ł��Ȃ��u���b�N�̓����蔻����쐬����v���O����
#include "GameObjectNot.h"

GameObjectNot::GameObjectNot()

{
}

GameObjectNot::~GameObjectNot()
{
}

bool GameObjectNot::IsNotCollidingWith(GameObjectNot& other)
{
	//Collision���̓����蔻��̊֐���Ԃ�
	return m_aabb.IsAABBIntersecting(minBound, maxBound, other.minBound, other.maxBound);
}

bool GameObjectNot::HIsNotCollidingWith(GameObjectNot& other)
{
	//Collision���̓����蔻��̊֐���Ԃ�
	return m_haabb.HIsAABBIntersecting(hminBound, hmaxBound, other.hminBound, other.hmaxBound);
}

bool GameObjectNot::colNot(GameObjectNot& other)
{
	return m_caabb.col(hminBound, hmaxBound, other.hminBound, other.hmaxBound);
}