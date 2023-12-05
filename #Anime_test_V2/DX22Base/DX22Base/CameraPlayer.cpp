#include "CameraPlayer.h"
#include "Input.h"

CameraPlayer::CameraPlayer(Player* pPlayer)
	: m_pPlayer(pPlayer)
	, m_radXZ(0.0f)
	, m_radY(0.0f)
{

}

CameraPlayer::~CameraPlayer()
{

}

void CameraPlayer::Update()
{
	m_look = m_pPlayer->GetPos();

	// ‰ñ“]
	const float Speed = 0.1f;

	if (IsKeyPress(VK_UP))		{ m_radY += Speed; }
	if (IsKeyPress(VK_DOWN))	{ m_radY -= Speed; }
	if (IsKeyPress(VK_LEFT))	{ m_radXZ += Speed; }
	if (IsKeyPress(VK_RIGHT))	{ m_radXZ -= Speed; }

	const float Radius = 5.0f;

	m_pos.x = cosf(m_radY) * sinf(m_radXZ) * Radius + m_look.x;
	m_pos.y = sinf(m_radY) * Radius + m_look.y;
	m_pos.z = cosf(m_radY) * cosf(m_radXZ) * Radius + m_look.z;

}