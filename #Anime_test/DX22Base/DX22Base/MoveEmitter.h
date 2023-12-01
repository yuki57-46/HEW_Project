#ifndef __MOVE_EMITTER_H__
#define __MOVE_EMITTER_H__

#include "Effect.h"

class MoveEmitter : public Emitter
{
public:
	MoveEmitter();
	~MoveEmitter();

protected:
	void Spawn(Particle* particle, const DirectX::XMFLOAT3& rootPos);

private:

};



#endif // !__MOVE_EMITTER_H__

