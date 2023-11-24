#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"
#include "PlayerCamera.h"


SceneGame::SceneGame()
	: m_pModel(nullptr)
	, m_pVS(nullptr)
	, m_pCamera(nullptr)
	, m_pPlayer(nullptr)
	, m_pUI(nullptr)
	, m_pPS(nullptr)
	, m_pEff(nullptr)
{
	// �����_�[�^�[�Q�b�g�A�[�x�o�b�t�@�̐ݒ�
	RenderTarget* pRTV = GetDefaultRTV(); // �f�t�H���g�Ŏg�p���Ă���RenderTargetView�̎擾
	DepthStencil* pDSV = GetDefaultDSV(); // �f�t�H���g�Ŏg�p���Ă���DepthStencilView�̎擾
	SetRenderTargets(1, &pRTV, pDSV);  // DSV��null����2D�\���ɂȂ�
	
	m_pPlayer = new Player();
	
	m_pCamera = new PlayerCamera(m_pPlayer);

	m_pUI = new ItemUI();
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("Assets/Shader/PS_Gray_test.cso")))
	{
		MessageBox(nullptr, "PS_Model.cso", "Error", MB_OK);
	}

	m_pEff = new Effekseer_Effect();

	//m_pModel = new Model;
	//if (!m_pModel->Load("Assets/Model/Squid_LOD0.fbx", 0.025f, Model::XFlip)) // �A�Z�b�g�̃p�X, �{��, ���]
	//{
	//	MessageBox(NULL, "Model", "Error", MB_OK);	//
	//}
	//m_pModel->SetVertexShader(m_pVS);	// ���f���֓ǂݍ��񂾒��_�V�F�[�_�[��ݒ�
	m_pPlayer->SetCamera(m_pCamera);
	//m_pEff->SetCamera(m_pCamera);
}

SceneGame::~SceneGame()
{
	if (m_pEff)
	{
		delete m_pEff;
		m_pEff = nullptr;
	}

	if (m_pUI)
	{
		delete m_pUI;
		m_pUI = nullptr;
	}
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pVS)
	{
		delete m_pVS;
		m_pVS = nullptr;
	}
	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

void SceneGame::Update(float tick)
{
	//static float rad = 0.0f;
	//DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(rad); // rad���g���ĉ�]������
	//DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(rad);	// rad���g���ĉ�]������
	//DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(rad);	// rad���g���ĉ�]������
	//DirectX::XMMATRIX mat = Rx * Ry * Rz;
	//mat = DirectX::XMMatrixTranspose(mat);
	//DirectX::XMFLOAT4X4 fMat; // �s��̊i�[��
	//DirectX::XMStoreFloat4x4(&fMat, mat);
	//Geometory::SetWorld(fMat); // �{�b�N�X�ɕϊ��s���ݒ�
	//rad += tick; // ��]�p�̍X�V

	m_pPlayer->Update();

	m_pCamera->Update();

	
	m_pEff->Update();


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
	static float rad = 0.0f;

	DirectX::XMFLOAT4X4 mat[3];

	// --- ���[���h�s��̌v�Z
	//DirectX::XMMATRIX world =[���[���h�s��̐ݒ� �� �ړ��A��]�A�g�k���l�������1�s�ł͎��܂�Ȃ�;
	DirectX::XMMATRIX world =
							DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
							DirectX::XMMatrixRotationX(rad) * DirectX::XMMatrixRotationY(rad) * DirectX::XMMatrixRotationZ(rad) *
							DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	// --- �r���[�s��̌v�Z
	//DirectX::XMMATRIX view = [�r���[�s��̐ݒ�]

	//DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
	//								DirectX::XMVectorSet(-3.0f, 4.0f, -3.0f, 0.0f),	// �J�����̈ʒu
	//								DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),	// �����_�̈ʒu
	//								DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)	// �A�b�v�x�N�g��
	//							);
	//view = DirectX::XMMatrixTranspose(view);
	//// [XMMATRIX�^����XMFLOAT4X4�^(mat[1])�֕ϊ����Ċi�[
	//DirectX::XMStoreFloat4x4(&mat[1], view);

	// --- �v���W�F�N�V�����s��̌v�Z
	// DirectX::XMMATRIX proj; �v���W�F�N�V�����s��
	//DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(5.0f, 16.0 / 9.0f, 10.0f, 500.0f);
	//proj = DirectX::XMMatrixTranspose(proj);
	//// [XMMATRIX�^����XMFLOAT4X4�^(mat[2])�֕ϊ����Ċi�[
	//DirectX::XMStoreFloat4x4(&mat[2], proj);
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();



	// --- �s����V�F�[�_�[�֐ݒ�
	m_pVS->WriteBuffer(0, mat);

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);
	
	
	m_pPlayer->Draw();
	// ���f���\��
	if (m_pModel)
	{
		m_pModel->Draw();
	}

	m_pEff->Draw();
	
	// Geometory�p�̕ϊ��s��̌v�Z
	//[���[���h�s��̐ݒ�]
	world = DirectX::XMMatrixScaling(10.0f, 0.1f, 10.0f) *
			DirectX::XMMatrixRotationX(rad) * DirectX::XMMatrixRotationY(rad) * DirectX::XMMatrixRotationZ(rad) *
			DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);
	//[�]�u�s��ɕϊ�
	world = DirectX::XMMatrixTranspose(world);
	//[XMMATRIX�^(world)����XMFLOAT4X4(mat[0])�֕ϊ����Ċi�[
	DirectX::XMStoreFloat4x4(&mat[0], world);
	// Geometory�p�̕ϊ��s��̐ݒ�
	Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	Geometory::DrawBox();

	SetRenderTargets(1, &pRTV, nullptr);
	m_pUI->Draw();

	
}
