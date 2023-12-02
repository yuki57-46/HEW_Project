#ifndef __CAMERA_BASE_H__
#define __CAMERA_BASE_H__

#include <DirectXMath.h>

class CameraBase
{
public:
	CameraBase();
	virtual ~CameraBase();
	virtual void Update() = 0;
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetLook();

protected:
	DirectX::XMFLOAT3 m_pos, m_look, m_up;
	float m_fovy, m_aspect, m_near, m_far;

private:

};

#endif // !__CAMERA_BASE_H__

