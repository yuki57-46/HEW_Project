#include "CameraBase.h"


// 度からラジアンへの変換
//#define DEG2RAD( deg ) ( ( deg ) * DirectX::XM_PI / 180.0f )
CameraBase::CameraBase()
    : m_pos(0.0f, 0.0f, -3.0f)
    , m_look(0.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_fovy(DirectX::XMConvertToRadians(60.0f))
    , m_aspect(16.0f / 9.0f)
    , m_near(0.3f)
    , m_far(100.0f)
{
}

void CameraBase::Update()
{
}

DirectX::XMFLOAT4X4 CameraBase::GetViewMatrix()
{
    DirectX::XMFLOAT4X4 mat;
    DirectX::XMMATRIX view;
    
    view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_pos),     // カメラの位置
        DirectX::XMLoadFloat3(&m_look), DirectX::XMLoadFloat3(&m_up)); // カメラの注視点, カメラの上方向
    view = DirectX::XMMatrixTranspose(view); 
    DirectX::XMStoreFloat4x4(&mat, view); // 
    

    return mat;
}

DirectX::XMFLOAT4X4 CameraBase::GetProjectionMatrix()
{
    DirectX::XMFLOAT4X4 mat;
    DirectX::XMMATRIX proj;

    proj = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_near, m_far);
    proj = DirectX::XMMatrixTranspose(proj);
    DirectX::XMStoreFloat4x4(&mat, proj);

    return mat;
}
