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
	CameraBase*			m_pCamera;		// カメラの参照
	DirectX::XMFLOAT3	m_pos;			// プレイヤーの位置
	Texture*			m_pShadowTex;	// 影のテクスチャ
	Texture*			m_pMarkerTex;	// マーカーのテクスチャ
	Effect*				m_pEffect;		// エフェクト
	MoveEmitter*		m_pMoveEmitter;	// アニメーション

};

#endif // !__PLAYER_H__

