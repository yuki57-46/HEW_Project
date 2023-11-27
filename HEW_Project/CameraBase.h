#pragma once
#include <DirectXMath.h>

class CameraBase
{
public:
	CameraBase();
	virtual ~CameraBase();
	virtual void Update() = 0;

	DirectX::XMFLOAT4X4 GetViewMatrix();	//�]�u�ς݃r���[�s��̎擾
	DirectX::XMFLOAT4X4 GetProjectionMatrix();	//�]�u�ς݃v���W�F�N�V�����s��̎擾

protected:
	DirectX::XMFLOAT3 m_pos, m_look, m_up;	//�r���[�s��̐ݒ�ɕK�v�ȕϐ�
	float m_fovy, m_aspect, m_near, m_far;	//�v���W�F�N�V�����s��̐ݒ�ɕK�v�ȕϐ�
};
