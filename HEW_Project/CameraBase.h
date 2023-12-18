//#pragma once
#ifndef ___CAMERA_BASE_H___
#define ___CAMERA_BASE_H___

#include<DirectXMath.h>

class CameraBase
{
public:
	CameraBase();
	virtual ~CameraBase();

	virtual void Update() = 0;

	DirectX::XMFLOAT4X4 GetViewMatrix();		//�]�u�ς݃r���[�s��̎擾
	DirectX::XMFLOAT4X4 GetProjectionMatrix();	//�]�u�ς݃v���W�F�N�V�����s��̎擾

	DirectX::XMFLOAT4X4 GetShadowViewMatrix();			//�e�̓]�u�ς݃r���[�s��̎擾
	DirectX::XMFLOAT4X4 GetShadowProjectionMatrix();	//�e�̓]�u�ς݃v���W�F�N�V�����s��̎擾

	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetLook();

protected:
	DirectX::XMFLOAT3 m_pos, m_look, m_up;	//�r���[�s��̐ݒ�ɕK�v�ȕϐ�
	DirectX::XMFLOAT3 m_Spos, m_Slook, m_Sup;	//�r���[�s��̐ݒ�ɕK�v�ȕϐ�
	float m_fovy, m_aspect, m_near, m_far;	//�v���W�F�N�V�����s��̐ݒ�ɕK�v�ȕϐ�

};


#endif // !___CAMERA_BASE_H___