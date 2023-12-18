#include "Texture.h"
#include "DirectXTex/TextureLoad.h"

/// <summary>
/// �e�N�X�`��
/// </summary>
Texture::Texture()
	: m_width(0), m_height(0)
	, m_pTex(nullptr)
	, m_pSRV(nullptr)
{
}
Texture::~Texture()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pTex);
}
HRESULT Texture::Create(const char* fileName)
{
	HRESULT hr = S_OK;

	// �����ϊ�
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, fileName, -1, wPath, MAX_PATH);  
	// �� ���C�h�����ɕϊ�:����(�����R�[�h, �t���O, �ϊ���������, ������, �ϊ��敶����, ������)
	//	 �����R�[�h: 0 = UTF-16 ?
	//	 �t���O: 0 = �f�t�H���g
	//	 �ϊ���������: fileName
	//	 ������: -1 = ������̏I�[�܂�
	//	 �ϊ��敶����: wPath
	//	 ������: MAX_PATH = �ő啶����(�p�X���ǂݍ��߂�ő啶����)

	// �t�@�C���ʓǂݍ���
	DirectX::TexMetadata mdata;	// ���^�f�[�^
	DirectX::ScratchImage image;	// �摜�f�[�^
	if (strstr(fileName, ".tga"))
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);	// TGA�t�@�C�����ǂݍ��܂ꂽ��hr��S_OK������
	else
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
		// �� WIC(Windows Imaging Component)���g�p���ĉ摜��ǂݍ���
		//	 ����(�t�@�C���p�X, �t���O, ���^�f�[�^, �摜�f�[�^)
		//	 �t�@�C���p�X: wPath
		//	 �t���O: WIC_FLAGS_NONE = �f�t�H���g
		//	 ���^�f�[�^: mdata
		//	 �摜�f�[�^: image
	if (FAILED(hr)) {
		return E_FAIL;	// �ǂݍ��ݎ��s
	}

	// �V�F�[�_���\�[�X����
	hr = CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), mdata, &m_pSRV);
	// �� �V�F�[�_���\�[�X�r���[���쐬
	//	 ����(�f�o�C�X, �摜�f�[�^, �摜��, ���^�f�[�^, �V�F�[�_���\�[�X�r���[)
	//	 �f�o�C�X: GetDevice()
	//	 �摜�f�[�^: image.GetImages() = �摜�f�[�^�̃|�C���^���擾
	//	 �摜��: image.GetImageCount() = �摜�f�[�^�̐����擾
	//	 ���^�f�[�^: mdata
	//	 �V�F�[�_���\�[�X�r���[: m_pSRV
	if (SUCCEEDED(hr))
	{
		m_width = (UINT)mdata.width;	// ���^�f�[�^���畝���擾
		m_height = (UINT)mdata.height;	// ���^�f�[�^���獂�����擾
	}
	return hr;
}
HRESULT Texture::Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData)
// �� ����(�t�H�[�}�b�g, ��, ����, �f�[�^) DXGI_FORMAT = �e�N�X�`���̃t�H�[�}�b�g, UINT = unsigned int,���ƍ���, �f�[�^
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height); 
	// �� �e�N�X�`���̐ݒ���쐬
	//	 ����(�t�H�[�}�b�g, ��, ����)
	return CreateResource(desc, pData);	// �e�N�X�`�����\�[�X���쐬
}

UINT Texture::GetWidth() const
{
	return m_width;
}
UINT Texture::GetHeight() const
{
	return m_height;
}
ID3D11ShaderResourceView* Texture::GetResource() const
{
	return m_pSRV;
}

D3D11_TEXTURE2D_DESC Texture::MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
// �� �e�N�X�`���̐ݒ���쐬 ����(�t�H�[�}�b�g, ��, ����)
{
	D3D11_TEXTURE2D_DESC desc = {};	// �e�N�X�`���̐ݒ�
	desc.Usage = D3D11_USAGE_DEFAULT;	// �g�p���@: D3D11_USAGE_DEFAULT = GPU���ǂݏ����\
	desc.Format = format;			// �t�H�[�}�b�g
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// �o�C���h�t���O: D3D11_BIND_SHADER_RESOURCE = �V�F�[�_���\�[�X�Ƃ��ăo�C���h
	desc.Width = width;			// ��
	desc.Height = height;		// ����
	desc.MipLevels = 1;		// �~�b�v�}�b�v���x��
	desc.ArraySize = 1;	// �z��T�C�Y
	desc.SampleDesc.Count = 1;	// �T���v����
	return desc;	// �e�N�X�`���̐ݒ��Ԃ�
}
HRESULT Texture::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
// �� �e�N�X�`�����\�[�X���쐬 ����(�e�N�X�`���̐ݒ�, �f�[�^)
{
	HRESULT hr = E_FAIL;	// �߂�l �����l: E_FAIL = ���s

	// �e�N�X�`���쐬
	D3D11_SUBRESOURCE_DATA data = {};	// �T�u���\�[�X�f�[�^
	data.pSysMem = pData;	// �V�X�e���������Ƀf�[�^��ݒ�
	data.SysMemPitch = desc.Width * 4;	// �s�b�`: �� * 4
	hr = GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);
	// �� 2D�e�N�X�`�����쐬
	//	 ����(�e�N�X�`���̐ݒ�, �����f�[�^, 2D�e�N�X�`��)
	//	 pData ? &data : pData�������data�̃|�C���^���Q��,����(=NULL)�̏ꍇ�͏����f�[�^�͖����A��ɍX�V�����?
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};	// �V�F�[�_���\�[�X�r���[�̐ݒ�
	switch (desc.Format)
	{
	default:						srvDesc.Format = desc.Format;			break;	// �t�H�[�}�b�g
	case DXGI_FORMAT_R32_TYPELESS: 	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;	break;
	// �� DXGI_FORMAT_R32_TYPELESS = 32�r�b�g�̌^�Ȃ��f�[�^, DXGI_FORMAT_R32_FLOAT = 32�r�b�g�̕��������_�f�[�^
	}
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	// �r���[�̎���: D3D11_SRV_DIMENSION_TEXTURE2D = 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;	// �~�b�v�}�b�v���x��
	// ����
	hr = GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);	// �V�F�[�_���\�[�X�r���[���쐬
	if (SUCCEEDED(hr))
	{
		m_width = desc.Width;
		m_height = desc.Height;
	}
	return hr;
}

/// <summary>
/// �����_�[�^�[�Q�b�g
/// </summary>
RenderTarget::RenderTarget()
	: m_pRTV(nullptr)
{
}
RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(m_pRTV);
}
void RenderTarget::Clear()
{
	static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Clear(color);
}
void RenderTarget::Clear(const float* color)
{
	GetContext()->ClearRenderTargetView(m_pRTV, color);	// �����_�[�^�[�Q�b�g�r���[���N���A
}
HRESULT RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);		// �e�N�X�`���̐ݒ���쐬
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;	// �o�C���h�t���O�Ƀ����_�[�^�[�Q�b�g��ǉ�
	return CreateResource(desc);	// �e�N�X�`�����\�[�X���쐬���A�߂�l��Ԃ�
}
HRESULT RenderTarget::CreateFromScreen()
{
	HRESULT hr;

	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = NULL;	// �o�b�N�o�b�t�@
	hr = GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pTex);	// �o�b�N�o�b�t�@���擾
	if (FAILED(hr)) { return hr; }

	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;	// �r���[�̎���: D3D11_RTV_DIMENSION_TEXTURE2D = 2D�e�N�X�`��
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// �� DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 32�r�b�g��RGBA�e8�r�b�g�̕����Ȃ������f�[�^
	rtvDesc.Texture2D.MipSlice = 0;	// �~�b�v�}�b�v���x��
	hr = GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
	// �� �����_�[�^�[�Q�b�g�r���[���쐬
	//	 ����(�e�N�X�`��, �����_�[�^�[�Q�b�g�r���[�̐ݒ�, �����_�[�^�[�Q�b�g�r���[)
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;	// �e�N�X�`���̐ݒ�
		m_pTex->GetDesc(&desc);	// �e�N�X�`���̐ݒ���擾
		m_width = desc.Width;	// ��
		m_height = desc.Height;	// ����
	}
	return hr;
}
ID3D11RenderTargetView* RenderTarget::GetView() const
{
	return m_pRTV;
}
HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// �e�N�X�`�����\�[�X�쐬
	HRESULT hr = Texture::CreateResource(desc, nullptr);	// �e�N�X�`�����\�[�X���쐬
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	rtvDesc.Format = desc.Format;	// �t�H�[�}�b�g
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// �� �r���[�̎���: D3D11_RTV_DIMENSION_TEXTURE2D = 2D�e�N�X�`��

	// ����
	return GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);	// �����_�[�^�[�Q�b�g�r���[���쐬
}

/// <summary>
/// �[�x�e�N�X�`��
/// </summary>
DepthStencil::DepthStencil()
	: m_pDSV(nullptr)
{
}
DepthStencil::~DepthStencil()
{
	SAFE_RELEASE(m_pDSV);
}
void DepthStencil::Clear()
{
	GetContext()->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
HRESULT DepthStencil::Create(UINT width, UINT height, bool useStencil)
{
	// https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#compositing
	// ��msdn�̍����̍��ڂ��Q��
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);
	// �� �e�N�X�`���̐ݒ���쐬
	//	 ����(�t�H�[�}�b�g, ��, ����)
	//	 �t�H�[�}�b�g: useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS
	//	 �� useStencil���^�Ȃ�DXGI_FORMAT_R24G8_TYPELESS���A�U�Ȃ�DXGI_FORMAT_R32_TYPELESS��Ԃ��B
	//   DXGI_FORMAT_R24G8_TYPELESS = 24�r�b�g�̐[�x��8�r�b�g�̃X�e���V���̏������t�H�[�}�b�g
	//   DXGI_FORMAT_R32_TYPELESS = 32�r�b�g�̐[�x�������t�H�[�}�b�g
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;	// �o�C���h�t���O�ɐ[�x�X�e���V����ǉ�
	return CreateResource(desc);
}
ID3D11DepthStencilView* DepthStencil::GetView() const
{
	return m_pDSV;
}
HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// �X�e���V���g�p����
	bool useStencil = (desc.Format == DXGI_FORMAT_R24G8_TYPELESS);
	// �� �t�H�[�}�b�g��DXGI_FORMAT_R24G8_TYPELESS�Ȃ�useStencil�͐^�A�����łȂ���΋U

	// ���\�[�X����
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;	// �o�C���h�t���O�ɐ[�x�X�e���V����ǉ�
	HRESULT hr = Texture::CreateResource(desc, nullptr);	// �e�N�X�`�����\�[�X���쐬
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};	// �[�x�X�e���V���r���[�̐ݒ�
	dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	// �� �t�H�[�}�b�g: useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT
	//   DXGI_FORMAT_D24_UNORM_S8_UINT = 24�r�b�g�̐[�x��8�r�b�g�̃X�e���V���̏������t�H�[�}�b�g
	//   DXGI_FORMAT_D32_FLOAT = 32�r�b�g�̕��������_�̐[�x�o�b�t�@�����t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	// �r���[�̎���: D3D11_DSV_DIMENSION_TEXTURE2D = 2D�e�N�X�`��

	// ����
	return GetDevice()->CreateDepthStencilView(m_pTex, &dsvDesc, &m_pDSV);	// �[�x�X�e���V���r���[���쐬���A�߂�l��Ԃ�
}