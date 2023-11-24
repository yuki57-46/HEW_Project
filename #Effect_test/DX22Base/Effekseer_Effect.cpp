#include "Effekseer_Effect.h"

Effekseer_Effect::Effekseer_Effect()
	: m_pCamera(nullptr)
{
	effect = LibEffekseer::Create("Assets/Effect/Sword.efk");
}

Effekseer_Effect::~Effekseer_Effect()
{

}

void Effekseer_Effect::Update()
{
	LibEffekseer::GetManager()->Play(effect, 0.0f, 0.0f, 0.0f);
}

void Effekseer_Effect::Draw()
{
	//DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	//LibEffekseer::SetViewPosition(pos);

}


