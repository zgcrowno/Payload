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
    // FOREGROUND DATA
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
    // BACKGROUND DATA
    else if (!orxString_Compare(_rstPayload.zParamName, "TopRadiusBG"))
    {
        _rstPayload.fValue = m_topRadiusBG;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "BottomRadiusBG"))
    {
        _rstPayload.fValue = m_bottomRadiusBG;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "LeftEdgeTopPointBG"))
    {
        _rstPayload.vValue = m_leftEdgeTopPointBG;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "LeftEdgeBottomPointBG"))
    {
        _rstPayload.vValue = m_leftEdgeBottomPointBG;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "RightEdgeTopPointBG"))
    {
        _rstPayload.vValue = m_rightEdgeTopPointBG;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "RightEdgeBottomPointBG"))
    {
        _rstPayload.vValue = m_rightEdgeBottomPointBG;
    }

    return orxTRUE;
}

void Tile::Update(const orxCLOCK_INFO &_rstInfo)
{
    
}

void Tile::SetVisualScale()
{
    orxVECTOR leftEdgeMidpoint = {
        (m_leftEdgeTopPoint.fX + m_leftEdgeBottomPoint.fX) / 2.0f,
        (m_leftEdgeTopPoint.fY + m_leftEdgeBottomPoint.fY) / 2.0f };
    orxVECTOR rightEdgeMidpoint = {
        (m_rightEdgeTopPoint.fX + m_rightEdgeBottomPoint.fX) / 2.0f,
        (m_rightEdgeTopPoint.fY + m_rightEdgeBottomPoint.fY) / 2.0f };
    m_visualScale = {
        fabs(rightEdgeMidpoint.fX - leftEdgeMidpoint.fX),
        fabs(m_rightEdgeBottomPoint.fY - m_rightEdgeTopPoint.fY) };
}
