#include "Player.h"
#include "Input.h"
#include "Sprite.h"
#include "Texture.h"

Player::Player()
	: m_pos(0.0f, 0.4f, 0.0f)
	, m_pCamera(nullptr)
	, m_pShadowTex(nullptr)
	, m_pMarkerTex(nullptr)
{
	// モデルの読み込み
	m_pModel = new Model;
//	if (!m_pModel->Load("Assets/Model/spot/spot.fbx", 0.6f))	
//	if (!m_pModel->Load("Assets/Model/myroom.mb", 1.0f, Model::XFlip))
	if (!m_pModel->Load("Assets/Model/Golem/Golem.FBX", 0.01f))	// ファイル指定、倍率指定、反転指定
	{
		MessageBox(NULL, "Model", "Player Error", MB_OK);	// エラーメッセージの表示
	}

	// 影のテクスチャ読み込み
	m_pShadowTex = new Texture();
	if (FAILED(m_pShadowTex->Create("Assets/Texture/Shadow.png")))
	{
		MessageBox(NULL, "Shadow", "Error", MB_OK);
	}

	// マーカーのテクスチャ読み込み
	m_pMarkerTex = new Texture();
	if (FAILED(m_pMarkerTex->Create("Assets/Texture/Marker.png")))
	{
		MessageBox(NULL, "Marker", "Error", MB_OK);
	}

	// 頂点シェーダーの読み込み
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pModel->SetVertexShader(m_pVS);	// モデルへ読み込んだ頂点シェーダーを設定

	m_pEffect = new Effect();
	m_pMoveEmitter = new MoveEmitter();
	m_pEffect->AddEmitter(m_pMoveEmitter);
}

Player::~Player()
{
	delete m_pMoveEmitter;
	delete m_pEffect;

	if (m_pVS)
	{
		delete m_pVS;
		m_pVS = nullptr;
	}

	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}

void Player::Update()
{
	bool isMove = false;
	DirectX::XMFLOAT3 camPos	= m_pCamera->GetPos();
	DirectX::XMFLOAT3 camLook	= m_pCamera->GetLook();
	DirectX::XMVECTOR vCamPos	= DirectX::XMLoadFloat3(&camPos);
	DirectX::XMVECTOR vCamLook	= DirectX::XMLoadFloat3(&camLook);

	DirectX::XMVECTOR vFront;	// カメラの正面方向のベクトル
	// カメラの位置と注視点から正面方向のベクトルを計算
	vFront = DirectX::XMVectorSubtract(vCamLook, vCamPos);
	// カメラの正面方向のベクトルを正規化
	vFront = DirectX::XMVector3Normalize(vFront);

	// ベクトルを90°回転させる行列を計算
	DirectX::XMMATRIX matRotSide = 
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90.0f));
	// 正面方向のベクトルを回転行列を適用させたベクトル
	DirectX::XMVECTOR vSide	
		= DirectX::XMVector3TransformCoord(vFront, matRotSide);
	// 0で初期化
	DirectX::XMVECTOR vMove	
		= DirectX::XMVectorZero();

	// vMoveにvFrontを加算して正面へ移動
	if (IsKeyPress('W')) { isMove = true; vMove = DirectX::XMVectorAdd(vMove, vFront); };
	// vMoveからvFrontを減算して後ろへ移動
	if (IsKeyPress('S')) { isMove = true; vMove = DirectX::XMVectorSubtract(vMove, vFront); };
	// vMoveにvSideを加算して右へ移動
	if (IsKeyPress('D')) { isMove = true; vMove = DirectX::XMVectorAdd(vMove, vSide); };
	// vMoveからvSideを減算して左へ移動
	if (IsKeyPress('A')) { isMove = true; vMove = DirectX::XMVectorSubtract(vMove, vSide); };

	vMove = DirectX::XMVectorMultiply(vMove, DirectX::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f));
	vMove = DirectX::XMVector3Normalize(vMove);		// 移動ベクトルの正規化
	vMove = DirectX::XMVectorScale(vMove, 0.1f);	// 移動ベクトルの移動速度を変更（定数倍）

	DirectX::XMFLOAT3 move;	// 計算した移動ベクトルの格納先
	// XMVECTOR型からXMFLOAT3型に変換し、moveに格納
	DirectX::XMStoreFloat3(&move, vMove);

	// 座標の更新
	m_pos.x += move.x;
	m_pos.y += move.y;
	m_pos.z += move.z;

	// エフェクトの発生
	m_pEffect->Update();
	m_pEffect->SetPos(m_pos);
	if (!m_pEffect->IsPlay() && isMove)	// エフェクトが再生されていなかったら
	{
		m_pEffect->Play();
	}
}

void Player::Draw()
{
	if (!m_pCamera) { return; }

	//DirectX::XMFLOAT4X4 inv; // 逆行列(inverse)の格納先
	//inv = m_pCamera->GetViewMatrix();
	//// カメラの行列はGPUに渡す際に転置されているため、逆行列の計算のために一度元に戻す
	//DirectX::XMMATRIX matInv = ;// invに格納されたカメラ行列を変換
	//matInv = DirectX::XMMatrixTranspose(matInv);// matInvを転置する
	//// 移動成分は逆行列で打ち消す必要がないので、0を設定して移動を無視する
	//DirectX::XMStoreFloat4x4(&inv, matInv);
	//inv._41 = 0.0f; inv._42 = 0.0f; inv._43 = 0.0f;
	//matInv = DirectX::XMLoadFloat4x4(&inv);
	//matInv = // 逆行列の計算
	//DirectX::XMMATRIX world = ;// ワールド行列とビルボードの行列を掛け合わせたもの(複数行)
	//DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	//Sprite::SetWorld(mat[0]);
	//Sprite::SetTexture(m_pMarkerTex);
	//Sprite::Draw();

	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)
		));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	m_pVS->WriteBuffer(0, mat);
	m_pModel->Draw();

//	DirectX::XMMATRIX mat1 = DirectX::XMMatrixTranslation(m_pos.x, 0.0f, m_pos.z);
	DirectX::XMMATRIX mat1 = DirectX::XMMatrixTranslation(m_pos.x, 0.0f, m_pos.z);
	mat1 = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90.0f)) * mat1;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mat1));
	Sprite::SetWorld(world);	// ワールド行列の設定
	Sprite::SetView(m_pCamera->GetViewMatrix());	// ビュー行列の設定
	Sprite::SetProjection(m_pCamera->GetProjectionMatrix());	// プロジェクション行列の設定
	Sprite::SetSize(DirectX::XMFLOAT2(0.5f, 0.5f));	// サイズを設定
	Sprite::SetTexture(m_pShadowTex);	// テクスチャを設定

	Sprite::Draw();

	// エフェクトの描画
	m_pEffect->Draw(
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix());
}

void Player::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

DirectX::XMFLOAT3 Player::GetPos()
{
	return m_pos;
}