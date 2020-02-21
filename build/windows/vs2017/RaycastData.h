#pragma once

#include "Payload.h"

namespace payload
{
    class RaycastData
    {
    public:
        orxVECTOR m_hitPosition;
        orxVECTOR m_hitNormal;
        orxOBJECT *m_hitObject;

        RaycastData(orxOBJECT *_hitObject, orxVECTOR _hitPosition, orxVECTOR _hitNormal);
        ~RaycastData();
    };
}
