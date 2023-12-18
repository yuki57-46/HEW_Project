#include "CameraDebug.h"
#include "Input.h"


//InputManager imanagerC = InputManager();

CameraDebug::CameraDebug()
	: m_radXZ(0.0f)
	, m_radY(0.0f)
	, m_radius(5.0f)
{

}

CameraDebug::~CameraDebug()
{

}

void CameraDebug::Update()
{
	float Speed = 0.1f;

	////©,ª,«,¨,Shift,CtrlÅJÌ_ð®©·
	if (IsKeyPress('H'))
	{
		m_look.z += Speed;
	}
	if (IsKeyPress('N'))
	{
		m_look.z -= Speed;
	}
	if (IsKeyPress('M'))
	{
		m_look.x += Speed;
	}
	if (IsKeyPress('B'))
	{
		m_look.x -= Speed;
	}
	if (IsKeyPress(VK_SHIFT))
	{
		m_look.y += Speed;
	}
	if (IsKeyPress(VK_CONTROL))
	{
		m_look.y -= Speed;
	}

	//W,A,S,D,Q,EÅJÌ_ð®©·
	//ñ]
	if (IsKeyPress('W'))
	{
		m_radY -= Speed;
	}
	if (IsKeyPress('S'))
	{
		m_radY += Speed;
	}
	if (IsKeyPress('A'))
	{
		m_radXZ += Speed;
	}
	if (IsKeyPress('D'))
	{
		m_radXZ -= Speed;
	}
	//£
	if (IsKeyPress('Q'))
	{
		m_radius += Speed;
	}
	//if (IsKeyPress('E'))
	//{
	//	m_radius -= Speed;
	//}

	m_pos.x = cos(m_radY) * sin(m_radXZ) * m_radius + m_look.x;
	m_pos.y = sin(m_radY) * m_radius + m_look.y;
	m_pos.z = cos(m_radY) * cos(m_radXZ) * m_radius + m_look.z;
}

