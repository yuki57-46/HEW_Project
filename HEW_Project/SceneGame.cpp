#include "SceneGame.h"
#include "Geometory.h"
#include <DirectXMath.h>

SceneGame::SceneGame()
{
	RenderTarget* pRTV = GetDefaultRTV();  //�f�t�H���g�Ŏg�p���Ă���RenderTargetView�̎擾
	DepthStencil* pDSV = GetDefaultDSV();  //�f�t�H���g�Ŏg�p���Ă���DepthStencilView�̎擾
	SetRenderTargets(1, &pRTV, pDSV);      //DSV��null����2D�\���ɂȂ�
	

	//m_pPlayer = new Player();
	m_pObjectMng = new ObjectMng();
	m_pCamera = new CameraDebug();
	m_pObject2D = new Object2D();
}

SceneGame::~SceneGame()
{
	
	/*if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}*/
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	if (m_pObjectMng)
	{
		delete m_pObjectMng;
		m_pObjectMng = nullptr;
	}
	if (m_pObject2D)
	{
		delete m_pObject2D;
		m_pObject2D = nullptr;
	}
}

void SceneGame::Update(float tick)
{
;
	m_pObjectMng->Update();
	//m_pPlayer->Update();
	m_pCamera->Update();
	
	m_pObject2D->Update();
}

void SceneGame::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();
	DirectX::XMFLOAT4X4 viewMatrix = m_pCamera->GetViewMatrix();
	DirectX::XMFLOAT4X4 projectionMatrix = m_pCamera->GetProjectionMatrix();

	
	m_pObjectMng->Draw(viewMatrix, projectionMatrix);

	m_pObject2D->Draw(viewMatrix, projectionMatrix);
	//---Geometry�p�̕ϊ��s����v�Z
	DirectX::XMMATRIX MoT = DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);
	DirectX::XMMATRIX MoS = DirectX::XMMatrixScaling(5.0f, 0.5f, 5.0f);
	DirectX::XMMATRIX world = MoS * MoT;
	//world = [���[���h�s��̐ݒ�];
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	//----Geomatry�p�̕ϊ��s���ݒ�
	Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	//Geometory::DrawBox();
	Geometory::DrawTriangle();

}
