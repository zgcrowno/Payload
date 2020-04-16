#include "MemorySetPolar2D.h"

using namespace payload;

void MemorySetPolar2D::OnCreate()
{

}

void MemorySetPolar2D::OnDelete()
{

}

orxBOOL MemorySetPolar2D::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void MemorySetPolar2D::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void MemorySetPolar2D::SetTiles()
{
    for (int i = m_lowerBound; i <= m_upperBound; i++)
    {
        for (int j = m_leftBound; j <= m_rightBound; j++)
        {
            if (i == m_lowerBound || i == m_upperBound || j == m_leftBound || j == m_rightBound)
            {
                m_tiles.push_back(m_tileSetTileRows->at(i).at(j));
            }
        }
    }
}

void MemorySetPolar2D::Reconfigure()
{
    MemorySet::Reconfigure();

    // The MemorySet's position.
    orxVECTOR pos = GetPosition();
    // Temp 2D Tile vector used to swap the tiles in _tileRows.
    std::vector<std::vector<Tile*>> tempTileRows;
    // Alter the MemorySet's associated Tiles' member data as appropriate.
    Tile *tile = nullptr;
    for (int i = m_lowerBound; i <= m_upperBound; i++)
    {
        // Set up the row.
        tempTileRows.push_back({});
        for (int j = m_leftBound; j <= m_rightBound; j++)
        {
            if (i == m_lowerBound || i == m_upperBound || j == m_leftBound || j == m_rightBound)
            {
                // Fetch the tile at the appropriate index.
                tile = m_tileSetTileRows->at(i).at(j);
                // Set the Tile's m_priorMemSetTheta value.
                tile->m_priorMemSetTheta = CartesianToPolar(tile->GetPosition(), pos).fY;
                // Alter tile row and column.
                tile->m_row = m_upperBound - (i - m_lowerBound);
                tile->m_col = m_rightBound - (j - m_leftBound);
            }
            // Add tile to temp vector.
            tempTileRows.at(i - m_lowerBound).push_back(tile);
        }
    }
    // Loop through the temp tiles to modify _tileRows appropriately.
    for (int i = 0; i < tempTileRows.size(); i++)
    {
        for (int j = 0; j < tempTileRows.size(); j++)
        {
            // Fetch the tile at the appropriate index.
            Tile *tile = tempTileRows.at(i).at(j);

            if (tile != nullptr)
            {
                m_tileSetTileRows->at(tile->m_row).at(tile->m_col) = tile;
            }
        }
    }
}

void MemorySetPolar2D::SetUp(
    const int &_row,
    const int &_tileSetHalfSquare,
    const float &_tileSetRadius,
    std::vector<std::vector<Tile*>> &_tileRows)
{
    // Set position relative to TileSet.
    SetParentSpacePosition({ 0.0f, 0.0f });
    // Set inner and outer radii.
    m_innerRadius = _tileSetRadius * ((float)_row / _tileSetHalfSquare);
    m_outerRadius = _tileSetRadius * ((float)(_row + 1) / _tileSetHalfSquare);
    // Set MemorySet's bounds.
    m_leftBound = (_tileSetHalfSquare - 1) - _row;
    m_rightBound = _tileSetHalfSquare + _row;
    m_lowerBound = (_tileSetHalfSquare - 1) - _row;
    m_upperBound = _tileSetHalfSquare + _row;
    // Set MemorySet's TileSet rows reference.
    m_tileSetTileRows = &_tileRows;
}
