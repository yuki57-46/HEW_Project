#include "ShadowP.h"
#include "Input.h"
#include <chrono>


DirectX::XMFLOAT3 PMinBound = DirectX::XMFLOAT3(-0.15f, -0.1f, -0.3f);  //境界の最小値
DirectX::XMFLOAT3 PMaxBound = DirectX::XMFLOAT3(0.2f, 0.1f, 0.5f);     //最大値

std::chrono::steady_clock::time_point lastSoundPlayTimeSdPly;
const std::chrono::milliseconds soundIntervalSd = std::chrono::milliseconds(1000);//再生時間三秒の時



ShadowP::ShadowP()
	: m_pos(3.5f, 0.2f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 10.0f)
	, m_IsAlterDir(false)
	, m_LastDir(false)
	, m_Jump(false)
	, m_footing(false)
	, m_moveY(0.001f)
	, m_JumpY(0.5f)
	, m_rotationY(0.0f)
	,m_animeWalk(NULL)
	, m_pSVSESdPly(nullptr)//スピーカ
	, m_pSDSESdPly(nullptr)//サウンドデータ
{
	m_pModel = new Model;
	//if (!m_pModel->Load("Assets/Model/Golem//Golem.FBX"))
	//{
	//	MessageBox(NULL, "読み込んだファイル名", "Error", MB_OK);
	//}

	 //モデルの読み込み処理
	if (!m_pModel->Load("Assets/Model/Player/kage.fbx",0.5f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}
	/*if (!m_pModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}*/

	m_pVS = new VertexShader();
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	//m_pModel->SetVertexShader(m_pVS);
	m_animeWalk = m_pModel->AddAnimation("Assets/Animation/Walk.fbx");	//	ファイルパスを入れる

	
	m_pSDSESdPly = LoadSound("Assets/Sound/SE/Kageidouon_Oobayashi.wav");

	minBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.3f);
	maxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);

	SetBounds(minBound, maxBound);
}


ShadowP::~ShadowP()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pVS)
	{
		delete m_pVS;
		m_pVS = nullptr;
	}
}

void ShadowP::Update(float tick)
{//編集
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSoundPlayTimeSdPly);


	m_pModel->Step(tick);

	if (m_IsAlterDir == true || m_IsAlterDir == false)
	{
		m_pModel->Play(m_animeWalk, true);
		if (elapsedTime >= soundIntervalSd)
		{
			m_pSVSESdPly = PlaySound(m_pSDSESdPly);

			// 最後のサウンド再生時間を更新
			lastSoundPlayTimeSdPly = currentTime;
		}

	}

	m_oldPos = m_pos;
	if (m_IsAlterDir == true)
	{
		m_pos.x += 0.01;
		if (m_IsAlterDir == true || m_LastDir == false)
		{
			m_rotationY = 0.0f;
		}
	}
	else if (m_IsAlterDir == false)
	{
		m_pos.x -= 0.01;
		if (m_IsAlterDir == false || m_LastDir == true)
		{
			m_rotationY = 180.0f;
		}
	}

	//重力
	if (!m_footing)
	{
		m_pos.y -= m_moveY;
		if (m_moveY < 0.5f && m_moveY >= 0.0f)
		{
			m_moveY += 0.001;
		}
	}

	//if (m_Jump == true)
	//{
	//	m_moveY = 1.2f;
	//	m_pos.y += m_moveY;
	//	m_Jump = false;
	//}
	//m_moveYは常に下方向へ加速する
	//m_moveYの速度に応じてプレイヤーの座標を変える
	//ジャンプ


	SetBounds(PMinBound, PMaxBound);  //最小値と最大値をセット
	m_LastDir = m_IsAlterDir;
}

void ShadowP::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	//---ワールド行列の計算
	DirectX::XMMATRIX MT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	//DirectX::XMMATRIX MS = DirectX::XMMatrixScaling(1.0f, 2.5f, 1.0f);
	DirectX::XMMATRIX MR = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_rotationY)); // Y軸回転
	DirectX::XMMATRIX MS = DirectX::XMMatrixScaling(0.7f, 1.2f, 0.7f);
	DirectX::XMMATRIX world = MS * MR * MT; // 回転を適用
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = viewMatrix; // 与えられた viewMatrix を使う
	mat[2] = projectionMatrix; // 与えられた projectionMatrix を使う
	//----行列をシェーダーへ設定
	m_pVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
//	m_pModel->Draw();



	ShaderList::SetWVP(mat);	// 転置済みの変換行列

	m_pModel->Draw(nullptr, [this](int index)	// ラムダ式
	{
		const Model::Mesh* pMesh = m_pModel->GetMesh(index);
		const Model::Material* pMaterial = m_pModel->GetMaterial(pMesh->materialID);
		ShaderList::SetMaterial(*pMaterial);

		DirectX::XMFLOAT4X4 bones[200];
		for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
		{
			// この計算はゲームつくろー「スキンメッシュの仕組み」が参考になる
			DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
				pMesh->bones[i].invOffset *
				m_pModel->GetBone(pMesh->bones[i].index)
			));
		}
		ShaderList::SetBones(bones);
	});
	//m_pModel->DrawBone();

}

void ShadowP::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 ShadowP::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	//posに最小値、最大値を足して当たり判定をずらす
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

void ShadowP::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void ShadowP::ShadowPPosY()
{
	m_pos.y = m_oldPos.y;
}

void ShadowP::oldX()
{
	m_pos.x = m_oldPos.x;
}

float ShadowP::ShadowPPosX()
{
	return m_pos.x;
}

DirectX::XMFLOAT3 ShadowP::NowPos()
{
	return m_pos;
}


void ShadowP::ShadowPPos()
{
	m_pos = m_oldPos;
}

void ShadowP::ShadowPupY()
{
	m_pos.y += 0.05;
}

void ShadowP::Use()
{//編集
	if (m_IsAlterDir == false)
	{
		m_IsAlterDir = true;
	}
	else
	{
		m_IsAlterDir = false;
	}
}

void  ShadowP::NotUse()
{
	//if (m_IsAlterDir == true)
	//{
	//
	//}
	m_IsAlterDir = false;
}

void ShadowP::Jump()
{
	m_moveY = 0.10f;
	m_pos.y += m_moveY;
}

bool ShadowP::isUse()
{
	return m_IsAlterDir;
}

bool ShadowP::IsJump()
{
	return m_Jump;
}

float ShadowP::GetMove()
{
	return m_moveY;
}

void ShadowP::SetFooting(bool footing)
{
	m_footing = footing;
	if (m_footing == true)
	{
		m_moveY = 0.0f;
	}
}
