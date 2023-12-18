#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "CameraBase.h"
#include "Model.h"
#include "Texture.h"
#include "MoveEmitter.h"

class Player
{
public:
	Player();
	~Player();

	void Update();
	void Draw();
	void SetCamera(CameraBase* pCamera);
	DirectX::XMFLOAT3 GetPos();

private:
	Model*				m_pModel;
	VertexShader*		m_pVS;
	CameraBase*			m_pCamera;		// �J�����̎Q��
	DirectX::XMFLOAT3	m_pos;			// �v���C���[�̈ʒu
	Texture*			m_pShadowTex;	// �e�̃e�N�X�`��
	Texture*			m_pMarkerTex;	// �}�[�J�[�̃e�N�X�`��
	Effect*				m_pEffect;		// �G�t�F�N�g
	MoveEmitter*		m_pMoveEmitter;	// �A�j���[�V����

};

#endif // !__PLAYER_H__

