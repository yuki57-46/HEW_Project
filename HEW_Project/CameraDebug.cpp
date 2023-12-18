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


	////←,↑,↓,→,Shift,Ctrlでカメラの注視点を動かす
	//if (IsKeyPress(VK_UP))
	//{
	//	m_look.z += Speed;
	//}
	//if (IsKeyPress(VK_DOWN))
	//{
	//	m_look.z -= Speed;
	//}
	//if (IsKeyPress(VK_RIGHT))
	//{
	//	m_look.x += Speed;
	//}
	//if (IsKeyPress(VK_LEFT))
	//{
	//	m_look.x -= Speed;
	//}
	//if (IsKeyPress(VK_SHIFT))
	//{
	//	m_look.y += Speed;
	//}
	//if (IsKeyPress(VK_CONTROL))
	//{
	//	m_look.y -= Speed;
	//}

	////W,A,S,D,Q,Eでカメラの視点を動かす
	////回転
	//if (IsKeyPress('W'))
	//{
	//	m_radY -= Speed;
	//}
	//if (IsKeyPress('S'))
	//{
	//	m_radY += Speed;
	//}
	//if (IsKeyPress('A'))
	//{
	//	m_radXZ += Speed;
	//}
	//if (IsKeyPress('D'))
	//{
	//	m_radXZ -= Speed;
	//}
	////距離
	//if (IsKeyPress('Q'))
	//{
	//	m_radius += Speed;
	//}
	//if (IsKeyPress('E'))
	//{
	//	m_radius -= Speed;
	//}

	m_pos.x = cos(m_radY) * sin(m_radXZ) * m_radius + m_look.x;
	m_pos.y = sin(m_radY) * m_radius + m_look.y;
	m_pos.z = cos(m_radY) * cos(m_radXZ) * m_radius + m_look.z;
}
