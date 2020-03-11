#include "Tile.h"

using namespace payload;

void Tile::OnCreate()
{
    
}

void Tile::OnDelete()
{

}

orxBOOL Tile::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

orxBOOL Tile::OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload)
{
    if (!orxString_Compare(_rstPayload.zParamName, "Cartesian"))
    {
        _rstPayload.fValue = m_bCartesian;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "TopRadius"))
    {
        _rstPayload.fValue = m_topRadius;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "BottomRadius"))
    {
        _rstPayload.fValue = m_bottomRadius;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "LeftEdgeTopPoint"))
    {
        _rstPayload.vValue = m_leftEdgeTopPoint;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "LeftEdgeBottomPoint"))
    {
        _rstPayload.vValue = m_leftEdgeBottomPoint;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "RightEdgeTopPoint"))
    {
        _rstPayload.vValue = m_rightEdgeTopPoint;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "RightEdgeBottomPoint"))
    {
        _rstPayload.vValue = m_rightEdgeBottomPoint;
    }

    return orxTRUE;
}

void Tile::Update(const orxCLOCK_INFO &_rstInfo)
{
    
}
