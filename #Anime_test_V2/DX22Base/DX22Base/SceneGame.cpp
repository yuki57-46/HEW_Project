#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"
#include "CameraPlayer.h"
#include "Setting.h"

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
	m_pModel = new Model();
	// �����ŃA�j���[�V���������������̂̃��f�����w�肷��
	// ��{�I�Ƀt�@�C���p�X��mixamo�t�H���_�Ɏg�����f�������Ă���
	if(!m_pModel->Load(MODEL_PATH, MODEL_SCALE, MODEL_FLIP))	// �t�@�C���w��A�{���w��A���]�w��
	{
		MessageBox(NULL, "�A�j���[�V�������f��", "Error", MB_OK);	// �G���[���b�Z�[�W�̕\��
	}
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));	// 12/05 �ǉ�

	// �����̓A�j���[�V������������̂��p�X�Ɏw�肳����
	// ���f���ƃA�j���[�V�����ŕ�����Ă���ꍇ�A�p�X�̎w���"Assets/Model/Animation/�A�j���[�V�����p��fbx
	// �Ƃ������ɐ؂�ւ��Ă����ƕ�����₷���B�@���̍ہAAnimation�t�H���_�ɓ������f�������Ă���
	Model::AnimeNo anime = m_pModel->AddAnimation(ANIME_PATH);
	m_pModel->Play(anime, true);

	// ���_�V�F�[�_�[�̓ǂݍ���
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	// 12/05 �A�j���[�V�����̍ۂ͕s�v�Ȃ̂ō폜
	//m_pModel->SetVertexShader(m_pVS);	// ���f���֓ǂݍ��񂾒��_�V�F�[�_�[��ݒ�

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

	m_pModel->Step(tick);
	m_pPlayer->Update();
	m_pCamera->Update();
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

	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	// �s����V�F�[�_�[�֐ݒ�
	m_pVS->WriteBuffer(0, mat);		// �z��̐擪�A�h���X���w�肵�āA�܂Ƃ߂ĕϊ��s���n��

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);

	// Geometory�p�̕ϊ��s����v�Z
	// 12/05 0����ς���
	world = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *	// �g��k��
		// Geometry�̐ݒ��ς��Ă�����
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

	// �A�j���[�V�����̕`��
	ShaderList::SetWVP(mat);	// �]�u�ς݂̕ϊ��s��
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
//	m_pModel->DrawBone();

#endif // DEBUG
}
