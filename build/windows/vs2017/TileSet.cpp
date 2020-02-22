#include "TileSet.h"
#include <iostream>

using namespace payload;

void TileSet::OnCreate()
{
    m_numRows = GetU32("NumRows", GetModelName());
    m_numColumns = GetU32("NumColumns", GetModelName());
    m_width = GetFloat("Width", GetModelName());
    m_height = GetFloat("Height", GetModelName());

    int halfNumColumns = m_numColumns / 2;
    int halfNumRows = m_numRows / 2;
    orxVECTOR pos = GetPosition();
    for (int i = 0; i < m_numRows; i++)
    {
        m_tileRows.push_back({});
        for (int j = 0; j < m_numColumns; j++)
        {
            Tile *tile = ScrollCast<Tile*>(CreateObject("O-Tile"));
            tile->SetParent(this);
            tile->SetParentSpacePosition({ (j - halfNumColumns) / (float)m_numColumns + (0.5f / m_numColumns), (i - halfNumRows) / (float)m_numRows + (0.5f / m_numRows) });
            /*orxVECTOR firstTileCorner = { (j - halfNumColumns) / halfNumColumns, (i - halfNumRows) / halfNumRows };
            orxVECTOR secondTileCorner = { (j - halfNumColumns) / halfNumColumns + (1 / m_numColumns), (i - halfNumRows) / halfNumRows };
            orxVECTOR thirdTileCorner = { (j - halfNumColumns) / halfNumColumns, (i - halfNumRows) / halfNumRows + (1 / m_numRows) };
            orxVECTOR fourthTileCorner = { (j - halfNumColumns) / halfNumColumns + (1 / m_numColumns), (i - halfNumRows) / halfNumRows + (1 / m_numRows) };
            tile->m_corners.push_back(firstTileCorner);
            tile->m_corners.push_back(secondTileCorner);
            tile->m_corners.push_back(thirdTileCorner);
            tile->m_corners.push_back(fourthTileCorner);*/
            m_tileRows.at(i).push_back(tile);
        }
    }
}

void TileSet::OnDelete()
{

}

orxBOOL TileSet::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void TileSet::Update(const orxCLOCK_INFO &_rstInfo)
{
    orxVECTOR rgb = GetVector("FillColor", "O-Tile");
    float fillAlpha = GetFloat("FillAlpha", "O-Tile");
    orxCOLOR fillColor = { rgb.fR, rgb.fG, rgb.fB, fillAlpha };
    orxRGBA fillColorRGBA = orxColor_ToRGBA(&fillColor);

    orxVECTOR pos = GetPosition();
    orxVECTOR scaledSize = GetScaledSize();
    orxVECTOR startPoint = WorldToScreenSpace({ pos.fX - scaledSize.fX / 2, pos.fY - scaledSize.fY / 2 });
    orxVECTOR endPoint = WorldToScreenSpace({ pos.fX + scaledSize.fX / 2, pos.fY - scaledSize.fY / 2 });
    orxDisplay_DrawLine(&startPoint, &endPoint, fillColorRGBA);
}
