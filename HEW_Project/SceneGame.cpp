#include "SceneGame.h"
#include "Geometory.h"
#include <DirectXMath.h>

SceneGame::SceneGame()
	:m_pSound(nullptr)
,m_pSourceVoice(nullptr)
, m_pVS(nullptr)
, m_pCamera{ nullptr, nullptr,nullptr }
, m_pobjcamera(nullptr)
, m_pRTV(nullptr)
, m_pDSV(nullptr)
{

	//RenderTarget* pRTV = GetDefaultRTV();  //�f�t�H���g�Ŏg�p���Ă���RenderTargetView�̎擾
	//DepthStencil* pDSV = GetDefaultDSV();  //�f�t�H���g�Ŏg�p���Ă���DepthStencilView�̎擾
	//SetRenderTargets(1, &pRTV, pDSV);      //DSV��null����2D�\���ɂȂ�
	//

	//�[�x�o�b�t�@�A�����_�[�^�[�Q�b�g�̐ݒ�
	m_pRTV = GetDefaultRTV();	//�f�t�H���g�Ŏg�p���Ă���Render Target View�̎擾
	m_pDSV = GetDefaultDSV();	//�f�t�H���g�Ŏg�p���Ă���Depth Stencil View�̎擾
	SetRenderTargets(1, &m_pRTV, m_pDSV);		//DSV��null���ƂQD�\���ɂȂ�

	//m_pPlayer = new Player();
	m_pVS = new VertexShader();

	m_pobjcamera = new ObjectCamera();

	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "ERROR", MB_OK);
	}

	m_pCamera[CAM_OBJ] = new CameraObject(m_pobjcamera);
	m_pCamera[CAM_DEBUG] = new CameraDebug();
	m_pCamera[CAM_SHADOW] = new CameraShadow();

	m_pBackShadow = new BackShadow;

	m_pObjectMng = new ObjectMng();
	//m_pDCamera = new CameraDebug();
	
	m_pBackShadow->SetShadowCamera(m_pCamera[CAM_SHADOW]);
	m_pSound = LoadSound("Assets/Sound/BGM/Ge-musi-nnA_Muto.wav"); // �T�E���h�t�@�C���̓ǂݍ���
	
	m_pobjcamera->SetCamera(m_pCamera[CAM_OBJ]);
	m_pBackShadow->SetShadowCamera(m_pCamera[CAM_SHADOW]);
	//m_pSourceVoice = PlaySound(m_pSound); // �T�E���h�̍Đ�
}

SceneGame::~SceneGame()
{
	
	/*if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}*/
	if (m_pRTV)
	{
		delete m_pRTV;
		m_pRTV = nullptr;
	}
	if (m_pDSV)
	{
		delete m_pDSV;
		m_pDSV = nullptr;
	}
	if (m_pCamera)
	{
		for (int i = 0; i < MAX_CAMERA; i++)
		{
			delete m_pCamera[i];
			m_pCamera[i] = nullptr;
		}
	}
	if (m_pObjectMng)
	{
		delete m_pObjectMng;
		m_pObjectMng = nullptr;
	}
	if (m_pBackShadow)
	{
		delete m_pBackShadow;
		m_pBackShadow = nullptr;
	}
	if (m_pBackShadow)
	{
		delete m_pBackShadow;
		m_pBackShadow = nullptr;
	}
	//m_pSourceVoice->Stop();
}

void SceneGame::Update(float tick)
{

	m_pobjcamera->SetCamera(m_pCamera[CAM_SHADOW]);
	m_pBackShadow->Update();

	//m_pObjectMng->SetPlayer(m_pPlayer);
	m_pobjcamera->SetCamera(m_pCamera[CAM_OBJ]);

	m_pCamera[CAM_OBJ]->Update();

	//�I�u�W�F�N�g
	m_pobjcamera->SetCamera(m_pCamera[CAM_DEBUG]);
	m_pObjectMng->Update(tick);
	//m_pObject2D->Update();

	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(10.0f, 0.1f, 10.0f);
	DirectX::XMMATRIX mat = S * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);
}

void SceneGame::Draw()
{

	m_pDSV->Clear();
	SetRenderTargets(1, &m_pRTV, nullptr);

	static float rad = 0.0f;
	DirectX::XMFLOAT4X4 mat[3];

	m_pobjcamera->SetCamera(m_pCamera[CAM_SHADOW]);
	m_pBackShadow->Draw(m_pObjectMng);


	//3D�\���ɕύX
	SetRenderTargets(1, &m_pRTV, m_pDSV);

	//���[���h�s��̌v�Z
	DirectX::XMMATRIX world =
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
		DirectX::XMMatrixRotationX(rad) * DirectX::XMMatrixRotationY(rad) * DirectX::XMMatrixRotationZ(rad) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	//�r���[�s��̌v�Z
	mat[1] = m_pCamera[CAM_OBJ]->GetViewMatrix();
	//�v���W�F�N�V�����s��̌v�Z
	mat[2] = m_pCamera[CAM_OBJ]->GetProjectionMatrix();

	//�s����V�F�[�_�[�֐ݒ�
	m_pVS->WriteBuffer(0, mat);

	//m_pPlayer->Draw(viewMatrix, projectionMatrix);

	//�v���C���[
	m_pobjcamera->SetCamera(m_pCamera[CAM_OBJ]);
	m_pobjcamera->Draw();

	//�I�u�W�F�N�g
	m_pObjectMng->Draw(m_pCamera[CAM_OBJ]->GetViewMatrix(), m_pCamera[CAM_OBJ]->GetProjectionMatrix());
	

	//Geometry�p�̕ύX�s����v�Z
	//���[���h�s��̍Čv�Z
	world =
		DirectX::XMMatrixScaling(10.0f, 0.1f, 10.0f) *
		DirectX::XMMatrixRotationX(rad) * DirectX::XMMatrixRotationY(rad) * DirectX::XMMatrixRotationZ(rad) *
		DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);
	//�]�u�s��ɕϊ�
	world = DirectX::XMMatrixTranspose(world);
	//XMMATRIX�^����XMFLOAT4X4�ɕϊ����Ċi�[
	DirectX::XMStoreFloat4x4(&mat[0], world);

	//Geometory�p�̕ϊ��s���ݒ�
	Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	//���f���\��
	//Geometory::DrawBox();

	//2D�\���ɕϊ�(�~�j�}�b�v��UI
	SetRenderTargets(1, &m_pRTV, nullptr);
}

