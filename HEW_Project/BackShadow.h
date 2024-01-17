#pragma once
//#ifndef ___BACK_SHADOW_H___
//#define ___BACK_SHADOW_H___

#include "Shader.h"
#include "CameraShadow.h"

#include "Player.h"
#include "ObjectManager.h"
#include "ShadowP.h"
#include "Screen.h"

#include "Coin.h"
#include "Goal.h"

class BackShadow
{
public:
	BackShadow();
	~BackShadow();

	void Update(float tick);
	void Draw(ObjectCamera* m_pobjcamera, ObjectMng* Obj, Coin* Coin1, Coin* Coin2, Coin* Coin3, Goal* Goal);
	void SetShadowCamera(CameraBase* pCamera);
	bool ShadowCollision(int sumAlpha, int cntAlpha, int noAlpha);
	bool ShadowUnderCollision(BYTE underAlpha, BYTE underAlpha2);
	bool ShadowEdgeCollision(int h, UINT width);
	void CoinCollection(Coin* Coin1, Coin* Coin2, Coin* Coin3);//コインの所得処理
	void GoalCollision(Goal* Goal);//ゴール
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
	BYTE m_alpha;				// レンダーウィンドウのα値下半身用
	BYTE m_alpha2;				// レンダーウィンドウのα値上半身用
	BYTE m_underAlpha;			// 足元のα値
	BYTE m_underAlpha2;			// 足元のめり込み解消
	BYTE m_PleyerSenter;		// 影君のデス判定用
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

	//1コイン
	DirectX::XMFLOAT3 m_1Cpos;
	int m_cast1CposX;
	int m_cast1CposY;

	//2コイン
	DirectX::XMFLOAT3 m_2Cpos;
	int m_cast2CposX;
	int m_cast2CposY;

	//3コイン
	DirectX::XMFLOAT3 m_3Cpos;
	int m_cast3CposX;
	int m_cast3CposY;

	//コインのサイズ
	DirectX::XMFLOAT3 m_Csize;
	int m_castCsizeX;
	int m_castCsizeY;

	//ゴール
	DirectX::XMFLOAT3 m_Goalpos;
	int m_castGoalposX;
	int m_castGoalposY;

	//ゴールのサイズ
	DirectX::XMFLOAT3 m_Goalsize;
	int m_castGoalsizeX;
	int m_castGoalsizeY;

	//スクリーン
	Screen* m_pScreen;


	//sound
	IXAudio2SourceVoice* m_pSVSESdCoin; // サウンドソース
	XAUDIO2_BUFFER* m_pSDSESdCoin; // サウンドバッファ


	PixelShader* m_pPS[3];			
};

//#endif // !___BACK_SHADOW_H___

