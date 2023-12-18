struct VS_IN	// CPU����n����Ă��钸�_�f�[�^
{
	float3 pos		: POSITION;
	float3 normal	: NORMAL0;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
};

struct VS_OUT	// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ɓn���f�[�^
{
	float4 pos		: SV_POSITION;
	float3 normal	: NORMAL0;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
};

cbuffer WVP : register(b0)	// CPU����n����Ă���萔�f�[�^
{
	float4x4 world;	// ���[���h�ϊ��s��
	float4x4 view;	// �r���[�ϊ��s��
	float4x4 proj;	// �v���W�F�N�V�����ϊ��s��
};

VS_OUT main(VS_IN vin) 
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);	// ���[�J�����W���烏�[���h���W�֕ϊ�
	vout.pos = mul(vout.pos, view);		// ���[���h���W����r���[���W�֕ϊ�
	vout.pos = mul(vout.pos, proj);		// �r���[���W����v���W�F�N�V�������W�֕ϊ�

	// ���̑��̒l�n��
	vout.normal = mul(vin.normal,(float3x3)world);	// ���̕ӂ̉����3�N����
	vout.uv		= vin.uv;
	vout.color	= vin.color;
	return vout;
}

