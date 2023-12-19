#include "CameraBase.h"

CameraBase::CameraBase()
	: m_pos(0.0f, 3.0f, -6.0f)
	, m_look(0.0f, 0.0f, 0.0f)
	, m_up(0.0f, 1.0f, 0.0f)
	, m_Spos(0.0f, 1.0f, -10.0f)
	, m_Slook(0.0f, 0.5f, 0.0f)
	, m_Sup(0.0f, 1.0f, 0.0f)
	, m_fovy(DirectX::XMConvertToRadians(60.0f))
	, m_aspect(16.0f / 9.0f)
	, m_near(0.3f)
	, m_far(100.0f)
{
}

CameraBase::~CameraBase()
{
}




DirectX::XMFLOAT4X4 CameraBase::GetViewMatrix()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX view;

	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&m_pos),	//�J�����̈ʒu
		DirectX::XMLoadFloat3(&m_look),	//�J�����̒����_
		DirectX::XMLoadFloat3(&m_up));	//�J�����̏����

	view = DirectX::XMMatrixTranspose(view);
	DirectX::XMStoreFloat4x4(&mat, view);

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

DirectX::XMFLOAT4X4 CameraBase::GetShadowViewMatrix()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX view;

	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&m_Spos),		//�J�����̈ʒu
		DirectX::XMLoadFloat3(&m_Slook),	//�J�����̒����_
		DirectX::XMLoadFloat3(&m_Sup));		//�J�����̏����

	view = DirectX::XMMatrixTranspose(view);
	DirectX::XMStoreFloat4x4(&mat, view);

	return mat;
}

DirectX::XMFLOAT4X4 CameraBase::GetShadowProjectionMatrix()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX proj;

	proj = DirectX::XMMatrixOrthographicOffCenterLH
	(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(proj));

	return mat;
}

DirectX::XMFLOAT3 CameraBase::GetPos()
{
	return m_pos;
}

DirectX::XMFLOAT3 CameraBase::GetLook()
{
	return m_look;
}

