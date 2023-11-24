#include "PlayerCamera.h"
#include "Input.h"

PlayerCamera::PlayerCamera(Player *pPlayer)
    : m_pPlayer(pPlayer)
    , m_radXZ(0.0f)
    , m_radY(0.0f)
{
}

void PlayerCamera::Update()
{
    m_look = m_pPlayer->GetPos(); // 注視点をプレイヤーの位置とする

    // カーソルキーで, カメラを回転させる処理
    const float Speed = 0.01f;
    if(IsKeyPress(VK_UP)) { m_radY += Speed; }
    if(IsKeyPress(VK_DOWN)) { m_radY -= Speed; }
    if(IsKeyPress(VK_LEFT)) { m_radXZ += Speed; }
    if(IsKeyPress(VK_RIGHT))  { m_radXZ -= Speed; }

    const float Radius = 5.0f;
    m_pos.x = cosf(m_radY) * sinf(m_radXZ) * Radius + m_look.x;
    m_pos.y = sinf(m_radY) * Radius + m_look.y;
    m_pos.z = cosf(m_radY) * cosf(m_radXZ) * Radius + m_look.z;

}

