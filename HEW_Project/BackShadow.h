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

	void Update(float tick);
	void Draw(ObjectMng* Obj);
	void SetShadowCamera(CameraBase* pCamera);
	bool ShadowCollision(int sumAlpha, int cntAlpha, int noAlpha);
	bool ShadowUnderCollision(BYTE underAlpha);
	bool ShadowEdgeCollision(int h, UINT width);

private:
	// カメラ
	CameraBase* m_pCamera;	// レンダーのカメラ

	// レンダー
	Texture* m_pRTVTexture;	// レンダーテクスチャー
	RenderTarget* m_pRTV_BS;// レンダーターゲット
	DepthStencil* m_pDSV_BS;// 深度バッファ

	//影の当たり判定用
	DirectX::XMFLOAT3 m_SPpos;	// 影プレイヤーの位置XYZ
	int m_indexX;				// 影プレイヤーの配列のインデックスX
	int m_indexY;				// 影プレイヤーの配列のインデックスY
	int m_castPosX;				// 影プレイヤーの位置の変数Xのfloatをintにcastする用
	int m_castPosY;				// 影プレイヤーの位置の変数Yのfloatをintにcastする用
	float m_SPposX;				// 影プレイヤーの位置のXの値
	float m_SPposY;				// 影プレイヤーの位置のYの値
	BYTE m_alpha;				// レンダーウィンドウのα値
	BYTE m_underAlpha;			// 足元のα値
	BYTE m_Player_a;			// プレイヤーの位置のα値
	bool m_collisionFlag;		// 当たり判定をとったらtrueになる
	bool m_upFlag;				// 斜面かどうか
	bool m_underFlag;			// 足場があるかどうか
	int m_sumAlpha;				// α値の合計
	int m_alphaData;			// スキャン範囲にα値のある場所の数
	int m_noAlphaData;			// スキャン範囲にα値のない場所の数
	bool m_LRcheck;				// 進行方向確認

	bool m_draw;

	//影のみに描きたい物
	ShadowP* m_pShadowPlayer;	// 影プレイヤー

};

//#endif // !___BACK_SHADOW_H___

