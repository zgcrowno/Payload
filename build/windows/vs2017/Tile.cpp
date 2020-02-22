#include "Tile.h"

using namespace payload;

void Tile::OnCreate()
{
    m_bezierInterval = GetFloat("BezierInterval", GetModelName());
    orxVECTOR rgb = GetVector("FillColor", GetModelName());
    float fillAlpha = GetFloat("FillAlpha", GetModelName());
    orxCOLOR fillColor = { rgb.fR, rgb.fG, rgb.fB, fillAlpha };
    m_fillColor = orxColor_ToRGBA(&fillColor);
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
    /*for (int i = 0; i < m_corners.size() - 1; i++)
    {
        orxVECTOR pos = GetPosition();
        orxVECTOR endpoint = { pos.fX + 100, pos.fY };
        orxDisplay_DrawLine(&pos, &endpoint, m_col);
    }*/

    orxVECTOR pos = WorldToScreenSpace(GetPosition());
    orxVECTOR endpoint = { GetScreenWidth(), GetScreenHeight() };
    orxDisplay_DrawLine(&pos, &endpoint, m_fillColor);


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
