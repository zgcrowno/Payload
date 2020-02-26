#include "Tile.h"

using namespace payload;

void Tile::OnCreate()
{
    orxVECTOR rgb = GetVector("FillColor", GetModelName());
    float fillAlpha = GetFloat("FillAlpha", GetModelName());
    float borderAlpha = GetFloat("BorderAlpha", GetModelName());
    orxCOLOR fillColor = { rgb.fR, rgb.fG, rgb.fB, fillAlpha };
    rgb = GetVector("BorderColor", GetModelName());
    orxCOLOR borderColor = { rgb.fR, rgb.fG, rgb.fB, borderAlpha };
    m_fillColor = orxColor_ToRGBA(&fillColor);
    m_borderColor = orxColor_ToRGBA(&borderColor);
    for (ScrollObject *child = GetOwnedChild(); child != nullptr; child = child->GetOwnedSibling())
    {
        m_edges.push_back(ScrollCast<TileEdge*>(child));
    }
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

void Tile::Update(const orxCLOCK_INFO &_rstInfo)
{
    Drawn::Update(_rstInfo);
}

void Tile::Draw()
{
    // TODO: Consolidate these so I'm not looping through all of the TileEdges twice.
    // Draw Tile
    std::vector<orxVECTOR> polygonVertices;
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
    orxDisplay_DrawPolygon(polygonVertices.data(), polygonVertices.size(), m_fillColor, true);
    // Draw Border
    for (TileEdge *tileEdge : m_edges)
    {
        for (int i = 0; i < tileEdge->m_vertices.size() - 1; i++)
        {
            orxVECTOR startingPoint = WorldToScreenSpace(tileEdge->m_vertices.at(i)->GetPosition());
            orxVECTOR endPoint = WorldToScreenSpace(tileEdge->m_vertices.at(i + 1)->GetPosition());
            orxDisplay_DrawLine(&startingPoint, &endPoint, m_borderColor);
        }
    }

    /*orxVECTOR drawPoint1 = orxVECTOR_0;
    orxVECTOR drawPoint2 = orxVECTOR_0;

    orxVECTOR drawPoint = orxVECTOR_0;

    orxVECTOR p1 = { 500, 500, 0 };
    orxVECTOR cp1 = { 550, 200, 0 };
    orxVECTOR cp2 = { 600, 200, 0 };
    orxVECTOR p2 = { 650, 600, 0 };

    for (float step = 0; step + m_bezierInterval <= 1; step += m_bezierInterval)
    {
        drawPoint1 = *orxVector_Bezier(&drawPoint,
            &p1,
            &cp1,
            &cp2,
            &p2,
            step);
        drawPoint2 = *orxVector_Bezier(&drawPoint,
            &p1,
            &cp1,
            &cp2,
            &p2,
            step + m_bezierInterval);

        orxDisplay_DrawLine(&drawPoint1, &drawPoint2, m_col);
    }*/
}
