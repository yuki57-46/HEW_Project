#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"
#include "CameraPlayer.h"

SceneGame::SceneGame()
	: m_pModel(nullptr)
	, m_pVS(nullptr)
	, m_pCamera(nullptr)
	, m_pPlayer(nullptr)
	, m_pUI(nullptr)
{	
	// �J�����̓ǂݍ���
//	m_pCamera = new CameraDebug;
	m_pPlayer = new Player();
	m_pCamera = new CameraPlayer(m_pPlayer);
	m_pPlayer->SetCamera(m_pCamera);
	m_pUI = new ItemUI();

	// ���f���̓ǂݍ���
	m_pModel = new Model;
//	if (!m_pModel->Load("Assets/Model/MushroomMon/MushroomMon.FBX", 0.01f))	
//	if (!m_pModel->Load("Assets/Model/myroom.mb", 1.0f, Model::XFlip))
	//if (!m_pModel->Load("Assets/Model/Golem/Golem.FBX", 0.01f))	// �t�@�C���w��A�{���w��A���]�w��
	//{
	//	MessageBox(NULL, "Golem.FBX", "Error", MB_OK);	// �G���[���b�Z�[�W�̕\��
	//}
	if (!m_pModel->Load("Assets/Model/mixamo/Running.fbx", 0.01f))	// �t�@�C���w��A�{���w��A���]�w��
	{
		MessageBox(NULL, "Golem.FBX", "Error", MB_OK);	// �G���[���b�Z�[�W�̕\��
	}
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));

	Model::AnimeNo anime = m_pModel->AddAnimation("Assets/Model/mixamo/Running.fbx");
	m_pModel->Play(anime, true);

	// ���_�V�F�[�_�[�̓ǂݍ���
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pModel->SetVertexShader(m_pVS);	// ���f���֓ǂݍ��񂾒��_�V�F�[�_�[��ݒ�


	// �����_�[�^�[�Q�b�g�A�[�x�o�b�t�@�̐ݒ�
	RenderTarget* pRTV = GetDefaultRTV();	// �f�t�H���g�Ŏg�p���Ă���RenderTargetView�̎擾
	DepthStencil* pDSV = GetDefaultDSV();	// �f�t�H���g�Ŏg�p���Ă���DepthStencilView�̎擾
	SetRenderTargets(1, &pRTV, pDSV);		// DSV��null����2D�\���ɂȂ�

}

SceneGame::~SceneGame()
{
	if (m_pUI)
	{
		delete m_pUI;
		m_pUI = nullptr;
	}

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

	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

}

void SceneGame::Update(float tick) 
{
//	_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�{�b�N�X�̉�]
//	static float rad = 0.0f;

	////rad���g���ĉ�]������
	//DirectX::XMMATRIX Rx  = DirectX::XMMatrixRotationX(rad);	//X���̉�]�s��
	//DirectX::XMMATRIX Ry  = DirectX::XMMatrixRotationY(rad);	//Y���̉�]�s��
	//DirectX::XMMATRIX Rz  = DirectX::XMMatrixRotationZ(rad);	//Z���̉�]�s��
	//DirectX::XMMATRIX mat = Rx * Ry * Rz;	//���ׂĂ̍s�����ɂ܂Ƃ߂�v�Z

	//mat = DirectX::XMMatrixTranspose(mat);	//�V�F�[�_�ɓn���O�Ɏ��s���鏈��
	//DirectX::XMFLOAT4X4 fMat;	//�s��̊i�[��

	//DirectX::XMStoreFloat4x4(&fMat, mat);	//mat��fMat�Ɋi�[���鏈��
	//Geometory::SetWorld(fMat);				//�{�b�N�X�ɕϊ��s���ݒ�
	//rad += tick;	//��]�p�̍X�V

//	_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	//�n�ʂƂ��ĕό`������

	//�O���b�h�������ɗ���悤�Ɉړ�
	DirectX::XMMATRIX T = 
		DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);

	//�n�ʂƂȂ�悤�ɁA�O�㍶�E�ɍL���A�㉺�ɋ�������
	DirectX::XMMATRIX S = 
		DirectX::XMMatrixScaling(10.0f, 0.1f, 10.0f);

	DirectX::XMMATRIX mat = S * T;	//���ׂĂ̍s�����ɂ܂Ƃ߂�v�Z

	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;				//�s��̊i�[��
	DirectX::XMStoreFloat4x4(&fMat, mat);	//mat��fMat�Ɋi�[���鏈��
	Geometory::SetWorld(fMat);

	m_pPlayer->Update();
	m_pCamera->Update();
	m_pModel->Step(tick);
}

void SceneGame::Draw()
{
	static float rad = 0.0f;

	DirectX::XMFLOAT4X4 mat[3];

	// ���[���h�s��̌v�Z
	DirectX::XMMATRIX world = 
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *		// �g��k��
		DirectX::XMMatrixRotationX(rad) *					// ��]X
		DirectX::XMMatrixRotationY(rad) *					// ��]Y
		DirectX::XMMatrixRotationZ(rad) *					// ��]Z
		DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);	// �ړ�

	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	// �r���[�s��̌v�Z
	//DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
	//	DirectX::XMVectorSet(10.0f, 10.0f, 10.0f, 0.0f),	// �J�����̈ʒu
	//	DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),		// �J���������ڂ��Ă���ʒu
	//	DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)		// �J�����̏����
	//);
	mat[1] = m_pCamera->GetViewMatrix();

	//view = DirectX::XMMatrixTranspose(view);
	//DirectX::XMStoreFloat4x4(&mat[1], view);

	// �v���W�F�N�V�����s��̌v�Z
	//DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(50.0f, 16.0f / 9.0f, 10.0f, 80.0f);
	//proj = DirectX::XMMatrixTranspose(proj);
	//DirectX::XMStoreFloat4x4(&mat[2], proj);
	mat[2] = m_pCamera->GetProjectionMatrix();

	// �s����V�F�[�_�[�֐ݒ�
	m_pVS->WriteBuffer(0, mat);		// �z��̐擪�A�h���X���w�肵�āA�܂Ƃ߂ĕϊ��s���n��

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);

	// �v���C���[�̕`��
//	m_pPlayer->Draw();

	// ���f���̕`��
	m_pModel->Draw();
	//if (m_pModel)
	//{
	//	m_pModel->Draw();
	//}

	// Geometory�p�̕ϊ��s����v�Z
	world = DirectX::XMMatrixScaling(15.0f, 0.0f, 15.0f) *	// �g��k��
		DirectX::XMMatrixRotationX(rad) *					// ��]X
		DirectX::XMMatrixRotationY(rad) *					// ��]Y
		DirectX::XMMatrixRotationZ(rad) *					// ��]Z
		DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);	// �ړ�

	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	// Geometory�p�̕ϊ��s���ݒ�
	Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	Geometory::DrawBox();	//�{�b�N�X��`��

	// �����_�[�^�[�Q�b�g�̍Ē�`��2D�\���ɐ؂�ւ���
	// �ϐ���3D�̕��ƈႤ�ϐ��Œ�`����
	RenderTarget* pRTV2D = GetDefaultRTV();
	DepthStencil* pDSV2D = GetDefaultDSV();
	SetRenderTargets(1, &pRTV2D, pDSV2D);		// 3D�\��
	SetRenderTargets(1, &pRTV2D, nullptr);		// 2D�\��

	// UI�̕`��
	m_pUI->Draw();

	// �A�j���[�V�����̕`��
	m_pModel->Draw(nullptr, [this](int index)	// �����_��
		{
			const Model::Mesh* pMesh = m_pModel->GetMesh(index);
			const Model::Material* pMaterial = m_pModel->GetMaterial(pMesh->materialID);
			ShaderList::SetMaterial(*pMaterial);

			DirectX::XMFLOAT4X4 bones[200];
			for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
			{
				// ���̌v�Z�̓Q�[������[�u�X�L�����b�V���̎d�g�݁v���Q�l�ɂȂ�
				DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
					pMesh->bones[i].invOffset *
					m_pModel->GetBone(pMesh->bones[i].index)
				));
			}
			ShaderList::SetBones(bones);
		});

#ifdef _DEBUG
	m_pModel->DrawBone();
#endif // DEBUG
}
