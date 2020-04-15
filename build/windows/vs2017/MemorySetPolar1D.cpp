#include "MemorySetPolar1D.h"

using namespace payload;

void MemorySetPolar1D::OnCreate()
{

}

void MemorySetPolar1D::OnDelete()
{

}

orxBOOL MemorySetPolar1D::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void MemorySetPolar1D::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void MemorySetPolar1D::Undo()
{

}

void MemorySetPolar1D::Reconfigure(std::vector<std::vector<Tile*>> &_tileRows)
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

void MemorySetPolar1D::SetUp(
    const int &_row,
    const int &_col,
    const int &_tileSetSquare,
    const float &_tileSetRadius,
    const float &_polarTheta)
{
    // Set position relative to TileSet.
    SetParentSpacePosition({ 0.0f, 0.0f });
    // Set inner and outer radii.
    float radialDistance = (_tileSetRadius / (_tileSetSquare * 2.0f)) + (((_tileSetSquare - 1) - _row) * (_tileSetRadius / _tileSetSquare));
    m_innerRadius = radialDistance - (_tileSetRadius / (_tileSetSquare * 2));
    m_outerRadius = radialDistance + (_tileSetRadius / (_tileSetSquare * 2));
    // Set min and max theta.
    m_minTheta = _polarTheta - (orxMATH_KF_2_PI / (_tileSetSquare * 2));
    m_maxTheta = _polarTheta + (orxMATH_KF_2_PI / (_tileSetSquare * 2));
    // Set MemorySet's bounds.
    int halfSquare = _tileSetSquare / 2;
    if (_col < halfSquare)
    {
        m_leftBound = _col;
        m_rightBound = _col + halfSquare;
    }
    else
    {
        m_leftBound = _col - halfSquare;
        m_rightBound = _col;
    }
    m_lowerBound = 0;
    m_upperBound = _tileSetSquare - 1;
}
