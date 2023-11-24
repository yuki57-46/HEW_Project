#pragma once

#include <DirectXMath.h>
#include "Model.h"
#include "CameraBase.h"
#include "Texture.h"
#include "MoveEmitter.h"

class Player
{
public:
    Player();
    ~Player();

    void Update();
    void Draw();

    // カメラの設定
    void SetCamera(CameraBase* pCamera) { m_pCamera = pCamera; }
    // 現在位置の取得
    DirectX::XMFLOAT3 GetPos() { return m_pos; }

private:
    Model* m_pModel;
    VertexShader* m_pVS;
    CameraBase* m_pCamera; // カメラへの参照
    DirectX::XMFLOAT3 m_pos; // プレイヤーの位置

    Texture* m_pShadowTex;
    Texture* m_pMarker;

    Effect* m_pEffect;
    MoveEmitter* m_pMoveEmitter;

    PixelShader* m_pPS;
};

