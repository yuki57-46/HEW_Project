#include "Shader.h"
#include <d3dcompiler.h>
#include <stdio.h>

#pragma comment(lib, "d3dcompiler.lib") // ���C�u�����̃����N

//----------
// ��{�N���X
Shader::Shader(Kind kind)
	: m_kind(kind)
{
}
Shader::~Shader()
{
	std::vector<ID3D11Buffer*>::iterator it = m_pBuffers.begin();
	while (it != m_pBuffers.end())
	{
		SAFE_RELEASE((*it));
		++it;
	}
}
HRESULT Shader::Load(const char* pFileName)
{
	HRESULT hr = E_FAIL;

	// �t�@�C����ǂݍ���
	FILE* fp;
	fopen_s(&fp, pFileName, "rb");	// �o�C�i�����[�h�œǂݍ���
	if (!fp) { return hr; }

	// �t�@�C���̃T�C�Y�𒲂ׂ�
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);	// �t�@�C���̖����Ɉړ�
	fileSize = ftell(fp);	// �t�@�C���|�C���^�̈ʒu���擾

	// �������ɓǂݍ���
	fseek(fp, 0, SEEK_SET);	// �t�@�C���̐擪�Ɉړ�
	char* pData = new char[fileSize];	// �t�@�C���T�C�Y���̃��������m��
	fread(pData, fileSize, 1, fp);	// �t�@�C�����������ɓǂݍ���
	fclose(fp);

	// �V�F�[�_�[�쐬
	hr = Make(pData, fileSize);	// �V�F�[�_�[�쐬
	
	// �I������
	if (pData) { delete[] pData; }
	return hr;
}
HRESULT Shader::Compile(const char *pCode)
{
	static const char *pTargetList[] = 	// �V�F�[�_�[�^�[�Q�b�g
	{
		"vs_5_0",	// ���_�V�F�[�_�[
		"ps_5_0"	// �s�N�Z���V�F�[�_�[
	};

	HRESULT hr;
	ID3DBlob *pBlob;	// �R���p�C����̃f�[�^
	ID3DBlob *error;	// �G���[���
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;	// �R���p�C���t���O: �f�o�b�O��񂠂�A�œK���Ȃ�
	hr = D3DCompile(pCode, strlen(pCode), nullptr, nullptr, nullptr,	// �R���p�C��
		"main", pTargetList[m_kind], compileFlag, 0, &pBlob, &error);	
	// �� ����(�R�[�h, �R�[�h�T�C�Y, �t�@�C����, �}�N��, �C���N���[�h, �G���g���|�C���g, �^�[�Q�b�g, �R���p�C���t���O, �G�t�F�N�g�R���p�C���t���O, �R���p�C����̃f�[�^, �G���[���)
	//	 �R�[�h: pCode = �R���p�C������V�F�[�_�[�R�[�h(HLSL)
	//	 �R�[�h�T�C�Y: strlen(pCode) = �R�[�h�̃T�C�Y = ������̒������擾
	//	 �t�@�C����: nullptr = �t�@�C�����Ȃ�
	//	 �}�N��: nullptr = �}�N���Ȃ�
	//	 �C���N���[�h: nullptr = �C���N���[�h�Ȃ�
	//	 �G���g���|�C���g: "main" = �G���g���|�C���g�̊֐���
	//	 �^�[�Q�b�g: pTargetList[m_kind] = �V�F�[�_�[�^�[�Q�b�g = ���_�V�F�[�_�[ or �s�N�Z���V�F�[�_�[
	//	 �R���p�C���t���O: compileFlag = �R���p�C���t���O = �f�o�b�O��񂠂�A�œK���Ȃ�
	//	 �G�t�F�N�g�R���p�C���t���O: 0 = �f�t�H���g	
	//	 �R���p�C����̃f�[�^: &pBlob = �R���p�C����̃f�[�^���i�[����ϐ��̃A�h���X
	//	 �G���[���: &error = �G���[�����i�[����ϐ��̃A�h���X
	if (FAILED(hr)) { return hr; }

	// �V�F�[�_�쐬
	hr = Make(pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());	// �V�F�[�_�[�쐬 = Make�֐��ɃR���p�C����̃f�[�^��n��
	SAFE_RELEASE(pBlob);	// �R���p�C����̃f�[�^�����
	SAFE_RELEASE(error);	// �G���[�������
	return hr;
}

void Shader::WriteBuffer(UINT slot, void* pData)
{
	if(slot < m_pBuffers.size())	// �o�b�t�@�̐���菬�����ꍇ
		GetContext()->UpdateSubresource(m_pBuffers[slot], 0, nullptr, pData, 0, 0);	// �o�b�t�@�ɏ�������
		// �� ����(�o�b�t�@, �T�u���\�[�X, �̈�, �f�[�^, �f�[�^�T�C�Y, �f�[�^�̃I�t�Z�b�g)
		//	 �o�b�t�@: m_pBuffers[slot] = �o�b�t�@�̃|�C���^
		//	 �T�u���\�[�X: 0 = �f�t�H���g
		//	 �̈�: nullptr = �f�t�H���g
		//	 �f�[�^: pData = �������ރf�[�^
		//	 �f�[�^�T�C�Y: 0 = �f�t�H���g
		//	 �f�[�^�̃I�t�Z�b�g: 0 = �f�t�H���g
}
void Shader::SetTexture(UINT slot, Texture* tex)
{
	if (!tex || slot >= m_pTextures.size()) { return; }	// �e�N�X�`�����Ȃ� or �e�N�X�`���̐����傫���ꍇ
	ID3D11ShaderResourceView* pTex = tex->GetResource();	// �e�N�X�`���̃��\�[�X���擾
	m_pTextures[slot] = pTex;	// �e�N�X�`���̃��\�[�X���i�[
	switch (m_kind)	// �V�F�[�_�[�̎�ޕʂɏ���
	{
	case Vertex:	GetContext()->VSSetShaderResources(slot, 1, &pTex); break;	// ���_�V�F�[�_�[�̏ꍇ
	case Pixel:		GetContext()->PSSetShaderResources(slot, 1, &pTex); break;	// �s�N�Z���V�F�[�_�[�̏ꍇ
	}
}

HRESULT Shader::Make(void* pData, UINT size)	// �V�F�[�_�[�쐬
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();	// �f�o�C�X�擾

	// ��͗p�̃��t���N�V�����쐬
	ID3D11ShaderReflection* pReflection;	// �V�F�[�_�[���t���N�V����
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));	// �V�F�[�_�[���t���N�V�������쐬
	// �� ����(�f�[�^, �f�[�^�T�C�Y, �C���^�[�t�F�[�XID, �C���^�[�t�F�[�X)
	//	 �f�[�^: pData = �V�F�[�_�[�R�[�h
	//	 �f�[�^�T�C�Y: size = �V�F�[�_�[�R�[�h�̃T�C�Y
	//	 �C���^�[�t�F�[�XID: IID_PPV_ARGS(&pReflection) = �C���^�[�t�F�[�XID = �V�F�[�_�[���t���N�V������ID
	if (FAILED(hr)) { return hr; }

	// �萔�o�b�t�@�쐬
	D3D11_SHADER_DESC shaderDesc;	// �V�F�[�_�[�̐ݒ�
	pReflection->GetDesc(&shaderDesc);	// �V�F�[�_�[�̐ݒ���擾
	m_pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);	// �o�b�t�@�̐���ݒ�
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)	// �o�b�t�@�̐������J��Ԃ�
	{
		// �V�F�[�_�[�̒萔�o�b�t�@�̏����擾
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;	// �V�F�[�_�[�̒萔�o�b�t�@�̏��
		ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);	// �V�F�[�_�[�̒萔�o�b�t�@���擾
		cbuf->GetDesc(&shaderBufDesc);	// �V�F�[�_�[�̒萔�o�b�t�@�̏����擾

		// �쐬����o�b�t�@�̏��
		D3D11_BUFFER_DESC bufDesc = {};	// �o�b�t�@�̏��
		bufDesc.ByteWidth = shaderBufDesc.Size;	// �o�b�t�@�̃T�C�Y
		bufDesc.Usage = D3D11_USAGE_DEFAULT;	// �g�p���@: D3D11_USAGE_DEFAULT = GPU���ǂݏ����\
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �o�C���h�t���O: D3D11_BIND_CONSTANT_BUFFER = �萔�o�b�t�@�Ƃ��ăo�C���h

		// �o�b�t�@�̍쐬
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffers[i]);	// �o�b�t�@���쐬
		if (FAILED(hr)) { return hr; }	// ���s�����ꍇ�͏I��
	}
	// �e�N�X�`���̈�쐬
	m_pTextures.resize(shaderDesc.TextureNormalInstructions, nullptr);	// �e�N�X�`���̐���ݒ�

	return MakeShader(pData, size);	// �V�F�[�_�[�쐬
}

//----------
// ���_�V�F�[�_
VertexShader::VertexShader()
	: Shader(Shader::Vertex)
	, m_pVS(nullptr)
	, m_pInputLayout(nullptr)
{
}

VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVS);
}

void VertexShader::Bind(void)
{
	ID3D11DeviceContext* pContext =	GetContext();	// �f�o�C�X�R���e�L�X�g�擾
	pContext->VSSetShader(m_pVS, NULL, 0);	// ���_�V�F�[�_�[��ݒ�
	pContext->IASetInputLayout(m_pInputLayout);	// ���̓��C�A�E�g��ݒ�
	for (int i = 0; i < m_pBuffers.size(); ++i)	// �o�b�t�@�̐������J��Ԃ�
		pContext->VSSetConstantBuffers(i, 1, &m_pBuffers[i]);	// �o�b�t�@��ݒ�
	for (int i = 0; i < m_pTextures.size(); ++i)		// �e�N�X�`���̐������J��Ԃ�
		pContext->VSSetShaderResources(i, 1, &m_pTextures[i]);	// �e�N�X�`����ݒ�
}

HRESULT VertexShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();	// �f�o�C�X�擾
	
	// �V�F�[�_�[�쐬
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVS);	// ���_�V�F�[�_�[���쐬
	if(FAILED(hr)) { return hr; }

	/*
	�V�F�[�_�쐬���ɃV�F�[�_���t���N�V������ʂ��ăC���v�b�g���C�A�E�g���擾
	�Z�}���e�B�N�X�̔z�u�Ȃǂ��环�ʎq���쐬
	���ʎq���o�^�ρ��ė��p�A�Ȃ���ΐV�K�쐬
	https://blog.techlab-xe.net/dxc-shader-reflection/
	*/
	// �� �Q�l�T�C�g

	ID3D11ShaderReflection *pReflection;	// �V�F�[�_�[���t���N�V����
	D3D11_SHADER_DESC shaderDesc;	// �V�F�[�_�[�̐ݒ�
	D3D11_INPUT_ELEMENT_DESC* pInputDesc;	// ���̓��C�A�E�g�̐ݒ�
	D3D11_SIGNATURE_PARAMETER_DESC sigDesc;	// �V�O�l�`���̐ݒ�

	DXGI_FORMAT formats[][4] =	// �t�H�[�}�b�g
	{
		{
			DXGI_FORMAT_R32_UINT,			// DXGI_FORMAT_R32_UINT = 32�r�b�g�̕����Ȃ������f�[�^
			DXGI_FORMAT_R32G32_UINT,		// DXGI_FORMAT_R32G32_UINT = 32�r�b�g�̕����Ȃ������f�[�^(2����)
			DXGI_FORMAT_R32G32B32_UINT,		// DXGI_FORMAT_R32G32B32_UINT = 32�r�b�g�̕����Ȃ������f�[�^(3����)
			DXGI_FORMAT_R32G32B32A32_UINT,	// DXGI_FORMAT_R32G32B32A32_UINT = 32�r�b�g�̕����Ȃ������f�[�^(4����)
		}, {
			DXGI_FORMAT_R32_SINT,			// DXGI_FORMAT_R32_SINT = 32�r�b�g�̕������萮���f�[�^
			DXGI_FORMAT_R32G32_SINT,		// DXGI_FORMAT_R32G32_SINT = 32�r�b�g�̕������萮���f�[�^(2����)
			DXGI_FORMAT_R32G32B32_SINT,		// DXGI_FORMAT_R32G32B32_SINT = 32�r�b�g�̕������萮���f�[�^(3����)
			DXGI_FORMAT_R32G32B32A32_SINT,	// DXGI_FORMAT_R32G32B32A32_SINT = 32�r�b�g�̕������萮���f�[�^(4����)
		}, {
			DXGI_FORMAT_R32_FLOAT,			// DXGI_FORMAT_R32_FLOAT = 32�r�b�g�̕��������_�f�[�^
			DXGI_FORMAT_R32G32_FLOAT,		// DXGI_FORMAT_R32G32_FLOAT = 32�r�b�g�̕��������_�f�[�^(2����)
			DXGI_FORMAT_R32G32B32_FLOAT,	// DXGI_FORMAT_R32G32B32_FLOAT = 32�r�b�g�̕��������_�f�[�^(3����)
			DXGI_FORMAT_R32G32B32A32_FLOAT,	// DXGI_FORMAT_R32G32B32A32_FLOAT = 32�r�b�g�̕��������_�f�[�^(4����)
		},
	};

	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));	// �V�F�[�_�[���t���N�V�������쐬
	if (FAILED(hr)) { return hr; }

	pReflection->GetDesc(&shaderDesc);	// �V�F�[�_�[�̐ݒ���擾
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];	// ���̓��C�A�E�g�̐ݒ���쐬
	for(UINT i = 0; i < shaderDesc.InputParameters; ++ i)	// ���̓��C�A�E�g�̐ݒ�̐������J��Ԃ�
	{
		pReflection->GetInputParameterDesc(i, &sigDesc);	// �V�O�l�`���̐ݒ���擾
		pInputDesc[i].SemanticName = sigDesc.SemanticName;	// �Z�}���e�B�N�X�̖��O
		pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;	// �Z�}���e�B�N�X�̃C���f�b�N�X

		// http://marupeke296.com/TIPS_No17_Bit.html
		// �� �Q�l�T�C�g �r�b�g���Z�̂��ꂱ��
		BYTE elementCount = sigDesc.Mask;	// �}�X�N
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);	// �}�X�N�̌v�Z
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);	// �}�X�N�̌v�Z

		switch (sigDesc.ComponentType)	// �R���|�[�l���g�^�C�v
		{
			case D3D_REGISTER_COMPONENT_UINT32:	// D3D_REGISTER_COMPONENT_UINT32 = 32�r�b�g�̕����Ȃ������f�[�^
				pInputDesc[i].Format = formats[0][elementCount - 1];	// �t�H�[�}�b�g
				break;
			case D3D_REGISTER_COMPONENT_SINT32:	// D3D_REGISTER_COMPONENT_SINT32 = 32�r�b�g�̕������萮���f�[�^
				pInputDesc[i].Format = formats[1][elementCount - 1];	// �t�H�[�}�b�g
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:	// D3D_REGISTER_COMPONENT_FLOAT32 = 32�r�b�g�̕��������_�f�[�^
				pInputDesc[i].Format = formats[2][elementCount - 1];	// �t�H�[�}�b�g
				break;
		}
		pInputDesc[i].InputSlot = 0;	// ���̓X���b�g
		pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;	// �A���C�����g�o�C�g�I�t�Z�b�g
		pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// ���̓X���b�g�N���X: D3D11_INPUT_PER_VERTEX_DATA = ���_���ƂɃf�[�^�����
		pInputDesc[i].InstanceDataStepRate = 0;	// �C���X�^���X�f�[�^�X�e�b�v���[�g
	}

	hr = pDevice->CreateInputLayout(	// ���̓��C�A�E�g���쐬
		pInputDesc, shaderDesc.InputParameters,	// ���̓��C�A�E�g�̐ݒ�, ���̓��C�A�E�g�̐ݒ�̐�
		pData, size, &m_pInputLayout	// �V�F�[�_�[�R�[�h, �V�F�[�_�[�R�[�h�̃T�C�Y, ���̓��C�A�E�g
	);
	
	delete[] pInputDesc;	// ���̓��C�A�E�g�̐ݒ�����
	return hr;	// ���ʂ�Ԃ�
}

//----------
// �s�N�Z���V�F�[�_
PixelShader::PixelShader()
	: Shader(Shader::Pixel)
	, m_pPS(nullptr)
{
}
PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pPS);
}
void PixelShader::Bind(void)
{
	ID3D11DeviceContext* pContext = GetContext();	// �f�o�C�X�R���e�L�X�g�擾
	pContext->PSSetShader(m_pPS, nullptr, 0);	// �s�N�Z���V�F�[�_�[��ݒ�
	for (int i = 0; i < m_pBuffers.size(); ++i)	// �o�b�t�@�̐������J��Ԃ�
		pContext->PSSetConstantBuffers(i, 1, &m_pBuffers[i]);	// �o�b�t�@��ݒ�
	for (int i = 0; i < m_pTextures.size(); ++i)	// �e�N�X�`���̐������J��Ԃ�
		pContext->PSSetShaderResources(i, 1, &m_pTextures[i]);	// �e�N�X�`����ݒ�
}
HRESULT PixelShader::MakeShader(void* pData, UINT size)
{
	return GetDevice()->CreatePixelShader(pData, size, NULL, &m_pPS);	// �s�N�Z���V�F�[�_�[���쐬
}