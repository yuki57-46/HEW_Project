#pragma once

#include "CameraBase.h"
#include "Input.h"

class CameraDebug : public CameraBase
{
public:
    CameraDebug();
    ~CameraDebug() {}

    void Update();

private:
    float m_radXZ;
    float m_radY;
    float m_radius;
};

