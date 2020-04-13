#include "MemorySetCartesian2D.h"
#include <iostream>

using namespace payload;

void MemorySetCartesian2D::OnCreate()
{

}

void MemorySetCartesian2D::OnDelete()
{

}

orxBOOL MemorySetCartesian2D::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void MemorySetCartesian2D::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void MemorySetCartesian2D::Reconfigure(std::vector<std::vector<Tile*>> &_tileRows)
{
    // The MemorySet's position.
    orxVECTOR pos = GetPosition();
    // Temp 2D Tile vector used to swap the tiles in _tileRows.
    std::vector<std::vector<Tile*>> tempTileRows;
    // Alter the MemorySet's associated Tiles' member data as appropriate.
    for (int i = m_lowerBound; i <= m_upperBound; i++)
    {
        // Set up the row.
        tempTileRows.push_back({});
        for (int j = m_leftBound; j <= m_rightBound; j++)
        {
            // Fetch the tile at the appropriate index.
            Tile *tile = _tileRows.at(i).at(j);
            // Set the Tile's m_priorMemSetTheta value.
            tile->m_priorMemSetTheta = CartesianToPolar(tile->GetPosition(), pos).fY;
            // Alter tile row and column.
            tile->m_row = m_lowerBound + (j - m_leftBound);
            tile->m_col = m_rightBound - (i - m_lowerBound);
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
            _tileRows.at(tile->m_row).at(tile->m_col) = tile;
        }
    }
}
