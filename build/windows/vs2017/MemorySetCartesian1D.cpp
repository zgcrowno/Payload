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
