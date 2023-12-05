#include <CameraDebug.h>

CameraDebug::CameraDebug()
{

}

CameraDebug::~CameraDebug()
{

}

void CameraDebug::Update()
{
	const float Speed = 0.1f;
	// íçéãì_
	if (IsKeyPress(VK_UP))		{ m_look.z += Speed; }
	if (IsKeyPress(VK_DOWN))	{ m_look.z -= Speed; }
	if (IsKeyPress(VK_RIGHT))	{ m_look.x += Speed; }
	if (IsKeyPress(VK_LEFT))	{ m_look.x -= Speed; }
	if (IsKeyPress(VK_SHIFT))	{ m_look.y += Speed; }
	if (IsKeyPress(VK_CONTROL)) { m_look.y -= Speed; }

	// à íu
	//if (IsKeyPress('W'))	{ m_pos.z += Speed; }
	//if (IsKeyPress('S'))	{ m_pos.z -= Speed; }
	//if (IsKeyPress('D'))	{ m_pos.x += Speed; }
	//if (IsKeyPress('A'))	{ m_pos.x -= Speed; }
	//if (IsKeyPress('E'))	{ m_pos.y += Speed; }
	//if (IsKeyPress('Q'))	{ m_pos.y -= Speed; }

	const float Radius = 5.0f;
	// âÒì]
	if (IsKeyPress('W')) { m_radY  += Speed; }
	if (IsKeyPress('S')) { m_radY  -= Speed; }
	if (IsKeyPress('A')) { m_radXZ += Speed; }
	if (IsKeyPress('D')) { m_radXZ -= Speed; }

	// ãóó£
	if (IsKeyPress('Q')) { m_radius -= Speed; }
	if (IsKeyPress('E')) { m_radius += Speed; }

	m_pos.x = cos(m_radY) * sin(m_radXZ) * m_radius + m_look.x;
	m_pos.y = sin(m_radY) * m_radius + m_look.y;
	m_pos.z = cos(m_radY) * sin(m_radXZ) * m_radius + m_look.z;
}