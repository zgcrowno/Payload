#include "RaycastData.h"

using namespace payload;

RaycastData::RaycastData(orxOBJECT *_hitObject, orxVECTOR _hitPosition, orxVECTOR _hitNormal)
{
    m_hitObject = _hitObject;
    m_hitPosition = _hitPosition;
    m_hitNormal = _hitNormal;
}

RaycastData::~RaycastData()
{

}
