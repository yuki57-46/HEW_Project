#include "Lever.h"
#include "Lift_Obj.hpp"
#include "Input.h"
#include "Safe_Delete.hpp"

Lever::Lever()
	: m_isUse(false)
	, m_pModel(nullptr)
	, m_pVS(nullptr)
	, m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_rot(0.0f, 0.0f, 0.0f)
	, m_rotMat(DirectX::XMMatrixIdentity())
	, m_direction(0.0f, 0.0f, 1.0f)
	, m_rotSpeed(10.0f)
	, m_pLift_Obj(nullptr)
{
	m_isUse = true;

	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip))
	{
		MessageBox(nullptr, "モデルの読み込みに失敗しました", "エラー", MB_OK);
	}
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "頂点シェーダーの読み込みに失敗しました", "エラー", MB_OK);
	}
	m_pModel->SetVertexShader(m_pVS);



}

Lever::~Lever()
{
	SafeDelete(m_pLift_Obj);
	SafeDelete(m_pVS);
	SafeDelete(m_pModel);

}

void Lever::Update()
{
	// switch文でレバーの上下を判定を行う処理

	int LeverNum = 1;

	if (IsKeyTrigger(VK_SHIFT))
	{
		switch (LeverNum)
		{
		case 0:
			LeverNum = 1;
			break;
		case 1:
			LeverNum = 0;
			break;
		}
		
	}

	while (m_isUse)
	{
		switch (LeverNum)
		{
		// レバーを押して釣り床を上に上げる処理
		case 0:
			// オブジェクトのY軸にプラスを加える
			m_pLift_Obj->SetMoveFlg(true);

			break;
		// レバーを押して釣り床を下に下げる処理
		case 1:
			// オブジェクトのY軸にマイナスを加える
			m_pLift_Obj->SetMoveFlg(false);

			break;

		default:
			break;
		}
	}
}

void Lever::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	// レバーの描画処理
	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列の作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)
	);

	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = viewMatrix; // 引数の `viewMatrix` を利用
	mat[2] = projectionMatrix; // 引数の `projectionMatrix` を利用

	m_pVS->WriteBuffer(0, mat);
	m_pModel->Draw();

}

bool Lever::GetLeverFlag()
{
	return m_isUse;
}

void Lever::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool isFlag)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;

	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;

	m_isUse = isFlag;
}

void Lever::ModelChange()
{
	if (!m_pModel->Load("Assets/Model/test_model/test_block.fbx", 0.05f, Model::Flip::XFlip))
	{
		MessageBox(nullptr, "モデルの読み込みに失敗しました", "エラー", MB_OK);
	}
}

void Lever::ModelChange2()
{
	if (!m_pModel->Load("Assets/Model/Block/test_block_cube_tex_plus.fbx.fbx", 0.05f, Model::Flip::XFlip))
	{
		MessageBox(nullptr, "モデルの読み込みに失敗しました", "エラー", MB_OK);
	}
}
