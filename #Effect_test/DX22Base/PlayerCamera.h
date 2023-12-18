#pragma once

#include "CameraBase.h"
#include "Player.h"

class PlayerCamera : public CameraBase
{
public:
    PlayerCamera(Player* pPlayer);
    ~PlayerCamera() {}

    void Update();

    DirectX::XMFLOAT3 GetPos();
    DirectX::XMFLOAT3 GetLook();

private:
    Player* m_pPlayer;
    // カメラ系のパラメータ
    float m_radXZ;
    float m_radY;
    float m_radius;
};

