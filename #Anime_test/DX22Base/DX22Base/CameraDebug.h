#ifndef __CAMERA_DEBUG_H__
#define __CAMERA_DEBUG_H__

#include "Input.h"
#include "CameraBase.h"

class CameraDebug : public CameraBase
{
public:
	CameraDebug();
	~CameraDebug();

	void Update();

private:
	float	m_radXZ;
	float	m_radY;
	float	m_radius;
};


#endif // !__CAMERA_DEBUG_H__

