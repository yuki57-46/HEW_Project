#ifndef __CAMERA_PLAYER_H__
#define __CAMERA_PLAYER_H__

#include "CameraBase.h"
#include "Player.h"

class CameraPlayer : public CameraBase
{
public:
	CameraPlayer(Player* pPlayer);
	~CameraPlayer();

	void Update();

private:
	Player*	m_pPlayer;
	float	m_radXZ;
	float	m_radY;

};


#endif // !__CAMERA_PLAYER_H__

