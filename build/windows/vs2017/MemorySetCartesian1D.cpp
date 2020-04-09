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

void MemorySetCartesian1D::Reconfigure()
{
    // Set the Tiles' m_priorMemSetTheta value.
    orxVECTOR pos = GetPosition();
    m_leftTile->m_priorMemSetTheta = CartesianToPolar(m_leftTile->GetPosition(), pos).fY;
    m_rightTile->m_priorMemSetTheta = CartesianToPolar(m_rightTile->GetPosition(), pos).fY;
    // Swap the Tiles, as appropriate.
    int leftTileRow = m_leftTile->m_row;
    int leftTileCol = m_leftTile->m_col;
    Tile *leftTile = m_leftTile;
    m_leftTile->m_row = m_rightTile->m_row;
    m_leftTile->m_col = m_rightTile->m_col;
    m_rightTile->m_row = leftTileRow;
    m_rightTile->m_col = leftTileCol;
    m_leftTile = m_rightTile;
    m_rightTile = leftTile;
}
