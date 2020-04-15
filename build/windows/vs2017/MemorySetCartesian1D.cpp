#include "MemorySetCartesian1D.h"

using namespace payload;

void MemorySetCartesian1D::OnCreate()
{

}

void MemorySetCartesian1D::OnDelete()
{

}

orxBOOL MemorySetCartesian1D::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void MemorySetCartesian1D::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void MemorySetCartesian1D::Undo()
{

}

void MemorySetCartesian1D::Reconfigure(std::vector<std::vector<Tile*>> &_tileRows)
{
    // The MemorySet's position.
    orxVECTOR pos = GetPosition();
    // Swap the MemorySet's Tiles, as appropriate.
    for (int i = m_lowerBound; i <= m_upperBound; i++)
    {
        // Fetch the tiles at the appropriate indeces.
        Tile *tile1 = _tileRows.at(i).at(m_leftBound);
        Tile *tile2 = _tileRows.at(i).at(m_rightBound);
        // Set the Tiles' m_priorMemSetTheta value.
        tile1->m_priorMemSetTheta = CartesianToPolar(tile1->GetPosition(), pos).fY;
        tile2->m_priorMemSetTheta = CartesianToPolar(tile2->GetPosition(), pos).fY;
        // Swap the Tiles.
        tile1->m_col = m_rightBound;
        tile2->m_col = m_leftBound;
        _tileRows.at(i).at(m_leftBound) = tile2;
        _tileRows.at(i).at(m_rightBound) = tile1;
    }
}

void MemorySetCartesian1D::SetUp(
    const int &_row,
    const int &_col,
    const int &_tileSetSquare,
    const float &_tileSetWidth,
    const float &_tileSetHeight,
    const float &_normalizedBorderSize,
    const float &_normalizedTileSize,
    const orxVECTOR &_tileSetPos)
{
    // Determine the world size of border and tile.
    float borderWidth = _tileSetWidth * _normalizedBorderSize;
    float borderHeight = _tileSetHeight * _normalizedBorderSize;
    float tileWidth = _tileSetWidth * _normalizedTileSize;
    float tileHeight = _tileSetHeight * _normalizedTileSize;
    // Set scale relative to TileSet.
    float scaleX = borderWidth + tileWidth;
    float scaleY = borderHeight + tileHeight;
    SetScale({ scaleX, scaleY });
    // Set position relative to TileSet.
    float posX = _tileSetPos.fX - (_tileSetWidth / 2.0f) + (1.5f * borderWidth + tileWidth) + _col * (borderWidth + tileWidth);
    float posY = _tileSetPos.fY - (_tileSetHeight / 2.0f) + (borderHeight + 0.5f * tileHeight) + _row * (borderHeight + tileHeight);
    SetPosition({ posX, posY });
    // Set MemorySet's bounds.
    m_leftBound = _col;
    m_rightBound = _col + 1;
    m_lowerBound = 0;
    m_upperBound = _tileSetSquare - 1;
    // Set MemorySet's row.
    m_row = _row;
}
