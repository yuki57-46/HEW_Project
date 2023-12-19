#pragma once
//#ifndef ___BACK_SHADOW_H___
//#define ___BACK_SHADOW_H___

#include "Shader.h"
#include "CameraShadow.h"

#include "Player.h"
#include "ObjectManager.h"
#include "ShadowP.h"

class BackShadow
{
public:
	BackShadow();
	~BackShadow();

	void Update();
	void Draw(ObjectMng* Obj);
	void SetShadowCamera(CameraBase* pCamera);
	bool ShadowCollision(int sumAlpha, int cntAlpha, int noAlpha);
	bool ShadowUnderCollision(BYTE underAlpha);
	bool ShadowEdgeCollision(int h, UINT width);

private:
	// �J����
	CameraBase* m_pCamera;	// �����_�[�̃J����

	// �����_�[
	Texture* m_pRTVTexture;	// �����_�[�e�N�X�`���[
	RenderTarget* m_pRTV_BS;// �����_�[�^�[�Q�b�g
	DepthStencil* m_pDSV_BS;// �[�x�o�b�t�@

	//�e�̓����蔻��p
	DirectX::XMFLOAT3 m_SPpos;	// �e�v���C���[�̈ʒuXYZ
	int m_indexX;				// �e�v���C���[�̔z��̃C���f�b�N�XX
	int m_indexY;				// �e�v���C���[�̔z��̃C���f�b�N�XY
	int m_castPosX;				// �e�v���C���[�̈ʒu�̕ϐ�X��float��int��cast����p
	int m_castPosY;				// �e�v���C���[�̈ʒu�̕ϐ�Y��float��int��cast����p
	float m_SPposX;				// �e�v���C���[�̈ʒu��X�̒l
	float m_SPposY;				// �e�v���C���[�̈ʒu��Y�̒l
	BYTE m_alpha;				// �����_�[�E�B���h�E�̃��l
	BYTE m_underAlpha;			// �����̃��l
	BYTE m_Player_a;			// �v���C���[�̈ʒu�̃��l
	bool m_collisionFlag;		// �����蔻����Ƃ�����true�ɂȂ�
	bool m_upFlag;				// �Ζʂ��ǂ���
	bool m_underFlag;			// ���ꂪ���邩�ǂ���
	int m_sumAlpha;				// ���l�̍��v
	int m_alphaData;			// �X�L�����͈͂Ƀ��l�̂���ꏊ�̐�
	int m_noAlphaData;			// �X�L�����͈͂Ƀ��l�̂Ȃ��ꏊ�̐�
	bool m_LRcheck;				// �i�s�����m�F

	//�e�݂̂ɕ`��������
	ShadowP* m_pShadowPlayer;	// �e�v���C���[

};

//#endif // !___BACK_SHADOW_H___

