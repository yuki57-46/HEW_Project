#include "CameraDebug.h"
#include <cmath>

CameraDebug::CameraDebug()
    : CameraBase()
    , m_radXZ(0.0f)
    , m_radY(0.0f)
    , m_radius(10.0f)
{
}

void CameraDebug::Update()
{
    const float Speed = 0.2f;
    // ←,↑,→,↓,Shift,Ctrlでカメラの注視点を動かす
    if(IsKeyPress(VK_UP)) { m_look.z += Speed; }
    if(IsKeyPress(VK_DOWN)) { m_look.z -= Speed; }
    if(IsKeyPress(VK_LEFT)) { m_look.x -= Speed; }
    if(IsKeyPress(VK_RIGHT)) { m_look.x += Speed; }
    if(IsKeyPress(VK_SHIFT)) { m_look.y += Speed; }
    if(IsKeyPress(VK_CONTROL)) { m_look.y -= Speed; }

    // W,A,S,D,Q,Eでカメラの位置を動かす
    // 回転
    if(IsKeyPress('W'))
    {
        // カメラの回り込み角度 += 回転スピード;
        m_radY += Speed;
    }
    if(IsKeyPress('S')) 
    {
        // カメラの回り込み角度 -= 回転スピード;
        m_radY -= Speed;
    }
    if(IsKeyPress('A')) 
    {
        // カメラの上下角度 += 回転スピード;
        m_radXZ += Speed;
    }
    if(IsKeyPress('D'))
    {
        // カメラの上下角度 -= 回転スピード;
        m_radXZ -= Speed;
    }

    // 距離
    if(IsKeyPress('Q')) 
    {
        // カメラと注視点の距離 += 移動スピード;
        m_radius -= Speed;

    }
    if(IsKeyPress('E'))
    {
        // カメラと注視点の距離 -= 移動スピード;
        m_radius += Speed;
    }
    
    // 角度と距離, 注視点からカメラの位置を計算
    m_pos.x = cos(m_radY) * sin(m_radXZ) * m_radius + m_look.x;
    m_pos.y = sin(m_radY) * m_radius + m_look.y;
    m_pos.z = cos(m_radY) * cos(m_radXZ) * m_radius + m_look.z;
}
