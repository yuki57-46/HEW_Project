#include "MoveEmitter.h"

MoveEmitter::MoveEmitter()
	: Emitter("Assets/Texture/Smoke.png", {10, 0.1f, 2.0f})
{
}

MoveEmitter::~MoveEmitter()
{
}

void MoveEmitter::Spawn(Particle* particle, const DirectX::XMFLOAT3& rootPos)
{
	particle->pos.value = rootPos;
	particle->pos.add.y = 0.001f;
	particle->size.add.x = 0.001f;
	particle->size.add.y = 0.001f;
	particle->alpha.add.x = -0.1f;
}
