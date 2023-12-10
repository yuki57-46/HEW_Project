#include "MoveEmitter.h"

MoveEmitter::MoveEmitter()
	: Emitter("Assets/Texture/Smoke.png", {3, 0.1, 2.0f})
{

}

MoveEmitter::~MoveEmitter()
{

}

void MoveEmitter::Spawn(Particle* particle, const DirectX::XMFLOAT3& rootPos)
{
	particle->pos.value = rootPos;
	particle->pos.add.y = 0.001f;
	particle->size.value.x = 0.8f;
	particle->size.value.y = 0.8f;
	particle->size.add.x = 0.001f;
	particle->size.add.y = 0.001f;
	particle->alpha.add.x = -0.001f;
	particle->life = 0.3f;

//	particle->pos.accele.y = 1.1f;
}