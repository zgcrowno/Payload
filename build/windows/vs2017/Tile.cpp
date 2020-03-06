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
    if (!orxString_Compare(_rstPayload.zParamName, "UnitDistanceFromOrigin"))
    {
        _rstPayload.fValue = m_unitDistanceFromOrigin;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "TopCenterAngle"))
    {
        _rstPayload.fValue = m_topCenterAngle;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "BottomCenterAngle"))
    {
        _rstPayload.fValue = m_bottomCenterAngle;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "LeftEdgeTopAngle"))
    {
        _rstPayload.fValue = m_leftEdgeTopAngle;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "LeftEdgeBottomAngle"))
    {
        _rstPayload.fValue = m_leftEdgeBottomAngle;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "RightEdgeTopAngle"))
    {
        _rstPayload.fValue = m_rightEdgeTopAngle;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "RightEdgeBottomAngle"))
    {
        _rstPayload.fValue = m_rightEdgeBottomAngle;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "TopCenterPoint"))
    {
        _rstPayload.vValue = m_topCenterPoint;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "BottomCenterPoint"))
    {
        _rstPayload.vValue = m_bottomCenterPoint;
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
    Drawn::Update(_rstInfo);
}

void Tile::Draw()
{
    // TODO: Consolidate these so I'm not looping through all of the TileEdges twice.
    // Draw Tile
    /*std::vector<orxVECTOR> polygonVertices;
    for (TileEdge *tileEdge : m_edges)
    {
        for (int i = 0; i < tileEdge->m_vertices.size() - 1; i++)
        {
            polygonVertices.push_back(WorldToScreenSpace(tileEdge->m_vertices.at(i)->GetPosition()));
            if (i == tileEdge->m_vertices.size() - 2)
            {
                polygonVertices.push_back(WorldToScreenSpace(tileEdge->m_vertices.at(i + 1)->GetPosition()));
            }
        }
    }
    orxDisplay_DrawPolygon(polygonVertices.data(), polygonVertices.size(), m_fillColor, true);*/
    // Draw Border
    /*for (TileEdge *tileEdge : m_edges)
    {
        for (int i = 0; i < tileEdge->m_vertices.size() - 1; i++)
        {
            orxVECTOR startingPoint = WorldToScreenSpace(tileEdge->m_vertices.at(i)->GetPosition());
            orxVECTOR endPoint = WorldToScreenSpace(tileEdge->m_vertices.at(i + 1)->GetPosition());
            orxDisplay_DrawLine(&startingPoint, &endPoint, m_borderColor);
        }
    }*/
}
