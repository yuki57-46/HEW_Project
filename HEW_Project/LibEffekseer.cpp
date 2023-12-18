#include "LibEffekseer.h"
#include <uchar.h>

Effekseer::ManagerRef LibEffekseer::m_efkManager = nullptr;
EffekseerRendererDX11::RendererRef LibEffekseer::m_efkRenderer = nullptr;
EffekseerSound::SoundRef LibEffekseer::m_efkSound = nullptr;
Effekseer::Vector3D LibEffekseer::m_viewPosition;
Effekseer::Matrix44 LibEffekseer::m_viewMatrix;
Effekseer::Matrix44 LibEffekseer::m_projMatrix;

/*
* @brief ����������
* @parma[in] pDevice ���\�[�X�f�o�C�X
* @param[in] pContext �`��f�o�C�X
* @param[in] pAudio �T�E���h�f�o�C�X
*/
void LibEffekseer::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IXAudio2* pAudio)
{
	// �G�t�F�N�g�}�l�[�W���[�쐬
	m_efkManager = Effekseer::Manager::Create(8000);

	// �`��f�o�C�X�쐬
	auto graphicsDevice = EffekseerRendererDX11::CreateGraphicsDevice(pDevice, pContext);
	m_efkRenderer = EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	// �`�惂�W���[���̐ݒ�
	m_efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());
	m_efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	m_efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());
	m_efkManager->SetMaterialLoader(m_efkRenderer->CreateMaterialLoader());
	m_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// �T�E���h���W���[���ݒ�
	if (pAudio)
	{
		m_efkSound = EffekseerSound::Sound::Create(pAudio, 16, 16);
		m_efkManager->SetSoundPlayer(m_efkSound->CreateSoundPlayer());
		m_efkManager->SetSoundLoader(m_efkSound->CreateSoundLoader());
	}
}

/*
* @brief �I������
*/
void LibEffekseer::Uninit()
{
	m_efkSound.Reset();
	m_efkRenderer.Reset();
	m_efkManager.Reset();
}

/*
* @brief �J�����ʒu�̐ݒ�
* @param[in] pos �J�����ʒu
*/
void LibEffekseer::SetViewPosition(DirectX::XMFLOAT3 pos)
{
	m_viewPosition.X = pos.x;
	m_viewPosition.Y = pos.y;
	m_viewPosition.Z = pos.z;
}

/*
* @brief �J�����s��ݒ�
* @param[in] view �]�u�Ȃ��r���[�s��
* @param[in] proj �]�u�Ȃ��v���W�F�N�V�����s��
*/
void LibEffekseer::SetCameraMatrix(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	memcpy_s(m_viewMatrix.Values, sizeof(m_viewMatrix.Values), view.m, sizeof(view.m));
	memcpy_s(m_projMatrix.Values, sizeof(m_projMatrix.Values), proj.m, sizeof(proj.m));
}

/*
* @brief ���ׂẴG�t�F�N�g�̕`��
*/
void LibEffekseer::Draw()
{
	// ���C���[�p�����[�^�̐ݒ�
	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = m_viewPosition;
	m_efkManager->SetLayerParameter(0, layerParameter);

	// �}�l�[�W���[�̍X�V
	Effekseer::Manager::UpdateParameter updateParameter;
	m_efkManager->Update(updateParameter);

	// �����_���[�̍X�V
	m_efkRenderer->SetTime(1.0f / 60.0f);
	m_efkRenderer->SetProjectionMatrix(m_projMatrix);
	m_efkRenderer->SetCameraMatrix(m_viewMatrix);

	// �`�揈��
	m_efkRenderer->BeginRendering();
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = m_efkRenderer->GetCameraProjectionMatrix();
	m_efkManager->Draw(drawParameter);
	m_efkRenderer->EndRendering();
}

/*
* @brief Effekseer�}�l�[�W���[�̎擾
* @return �}�l�[�W���[�ւ̎Q��
*/
Effekseer::ManagerRef LibEffekseer::GetManager()
{
	return m_efkManager;
}

/*
* @brief �G�t�F�N�g�t�@�C���̓ǂݍ���
* @param[in] fileName �ǂݍ��ރt�@�C����
* @return �G�t�F�N�g�f�[�^
*/
Effekseer::EffectRef LibEffekseer::Create(const char* fileName)
{
	static char16_t u16Path[MAX_PATH] = u"";
	char16_t* ptr = u16Path;
	while(*fileName != '\0')
	{
		mbrtoc16(ptr, fileName, 1, nullptr);
		++ptr;
		++fileName;
	}
	*ptr = '\0';
	return Effekseer::Effect::Create(m_efkManager, u16Path);
}