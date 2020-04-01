#include "Tile.h"
#include <iostream>
#include <iomanip>

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

void Tile::Update(const orxCLOCK_INFO &_rstInfo)
{
    
}

const orxVECTOR Tile::GetGridRelativeCartesianPosition(const int &_row, const int &_col, const float &_normalizedBorderSize, const float &_normalizedTileSize)
{
    return {
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_col * (_normalizedBorderSize + _normalizedTileSize)),
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_row * (_normalizedBorderSize + _normalizedTileSize)) };
}

const int Tile::GetQuadrant(
    const int &_row,
    const int &_col,
    const int &_square,
    const orxVECTOR &_payloadRowAndCol,
    const bool _background)
{
    // In what Cartesian quadrant is this Tile?
    int quadrant;
    // Half of the TileSet's width/height.
    int halfSquare = _square / 2;

    if (_background)
    {
        quadrant = _col >= halfSquare && _row < halfSquare ? 1 :
            _col < halfSquare && _row < halfSquare ? 2 :
            _col < halfSquare && _row >= halfSquare ? 3 :
            4;
    }
    else
    {
        int row = _payloadRowAndCol.fX;
        quadrant = _col >= halfSquare && row < halfSquare ? 1 :
            _col < halfSquare && row < halfSquare ? 2 :
            _col < halfSquare && row >= halfSquare ? 3 :
            4;
    }

    return quadrant;
}

const int Tile::GetUnitDistanceFromTileSetCenter(
    const int &_row,
    const int &_col,
    const int &_square,
    const orxVECTOR &_payloadRowAndCol,
    const bool _background,
    const TileSetState &_tileSetState)
{
    // How many tiles away from the TileSet's pivot is this Tile?
    int unitDistanceFromOrigin;
    // Half of the TileSet's width/height.
    int halfSquare = _square / 2;

    switch (_tileSetState)
    {
    case TileSetState::Cartesian1D:
        unitDistanceFromOrigin = _col < halfSquare ? halfSquare - _col : (_col + 1) - halfSquare;
        break;
    case TileSetState::Polar1D:
        if (_background)
        {
            unitDistanceFromOrigin = _square - _row;
        }
        else
        {
            int row = _payloadRowAndCol.fX;
            unitDistanceFromOrigin = _square - row;
        }
        break;
    default:
        // How many tiles away from the TileSet's pivot (on the X-axis) is this Tile?
        int unitDistanceFromPivotX = _col < halfSquare ? halfSquare - _col : (_col + 1) - halfSquare;
        // How many tiles away from the TileSet's pivot (on the Y-axis) is this Tile?
        int unitDistanceFromPivotY = _row < halfSquare ? halfSquare - _row : (_row + 1) - halfSquare;
        unitDistanceFromOrigin = orxMAX(unitDistanceFromPivotX, unitDistanceFromPivotY);
        break;
    }

    return unitDistanceFromOrigin;
}

const int Tile::GetUnitDistanceFromTileSetPolarAxis(
    const int &_row,
    const int &_col,
    const int &_square,
    const int &_unitDistanceFromOrigin,
    const orxVECTOR &_payloadRowAndCol,
    const bool &_tileSetIs2D)
{
    // How many tiles away from the polar axis is this Tile?
    int unitDistanceFromPolarAxis;
    // Half of the TileSet's width/height.
    int halfSquare = _square / 2;

    if (_tileSetIs2D)
    {
        // In what Cartesian quadrant is this Tile?
        const int quadrant = GetQuadrant(_row, _col, _square, _payloadRowAndCol, true);

        unitDistanceFromPolarAxis = ((quadrant - 1) * (2 * _unitDistanceFromOrigin - 1));

        switch (quadrant)
        {
        case 1:
            unitDistanceFromPolarAxis += (halfSquare - _row) + (halfSquare + _unitDistanceFromOrigin - _col) - 1;
            break;
        case 2:
            unitDistanceFromPolarAxis += (halfSquare - _col) + ((_row + 1) - (halfSquare - _unitDistanceFromOrigin)) - 1;
            break;
        case 3:
            unitDistanceFromPolarAxis += ((_row + 1) - halfSquare) + ((_col + 1) - (halfSquare - _unitDistanceFromOrigin)) - 1;
            break;
        case 4:
            unitDistanceFromPolarAxis += ((_col + 1) - halfSquare) + (halfSquare + _unitDistanceFromOrigin - _row) - 1;
            break;
        }
    }
    else
    {
        unitDistanceFromPolarAxis = ((3 * _square) / 4) - _col;
        if (unitDistanceFromPolarAxis <= 0)
        {
            unitDistanceFromPolarAxis += _square;
        }
        if (unitDistanceFromPolarAxis == _square && halfSquare % 2 != 0)
        {
            unitDistanceFromPolarAxis = 0;
        }
    }

    return unitDistanceFromPolarAxis;
}

const int Tile::GetNumTilesInPolarRow(
    const int &_square,
    const int &_unitDistanceFromOrigin,
    const bool &_tileSetIs2D)
{
    // How many tiles are in this Tile's circular, polar-2D row?
    int numTilesInPolarRow;

    if (_tileSetIs2D)
    {
        numTilesInPolarRow = (_unitDistanceFromOrigin * 4) + ((_unitDistanceFromOrigin - 1) * 4);
    }
    else
    {
        numTilesInPolarRow = _square;
    }

    return numTilesInPolarRow;
}

const float Tile::GetDistanceFromTileSetCenter(
    const int &_square,
    const int &_unitDistanceFromOrigin,
    const float &_tileSetRadius,
    const orxVECTOR &_payloadRowAndCol,
    const bool &_tileSetIs2D,
    const bool &_background,
    const TileSetState &_tileSetState)
{
    // How far away from the TileSet's pivot is this Tile's visual center?
    float radialDistance;
    // Half of the TileSet's width/height.
    int halfSquare = _square / 2;

    if (_tileSetIs2D)
    {
        radialDistance = ((1.0f / _square) * _tileSetRadius) + ((_unitDistanceFromOrigin - 1) * ((1.0f / halfSquare) * _tileSetRadius));
    }
    else
    {
        if (_background)
        {
            radialDistance = ((1.0f / (_square * 2.0f)) * _tileSetRadius) + ((_unitDistanceFromOrigin - 1) * ((1.0f / _square) * _tileSetRadius));
        }
        else
        {
            int payloadUnitDistanceFromOrigin = GetUnitDistanceFromTileSetCenter(_payloadRowAndCol.fX, _payloadRowAndCol.fY, _square, _payloadRowAndCol, _background, _tileSetState);
            radialDistance = ((1.0f / (_square * 2.0f)) * _tileSetRadius) + ((payloadUnitDistanceFromOrigin - 1) * ((1.0f / _square) * _tileSetRadius));
        }
    }

    return radialDistance;
}

const float Tile::GetPolarTheta(
    const int &_square,
    const int &_unitDistanceFromPolarAxis,
    const int &_tilesInPolarRow,
    const bool &_tileSetIs2D)
{
    // At what angle is this Tile, in reference to the TileSet's pivot?
    float theta;
    // Half of the TileSet's width/height.
    int halfSquare = _square / 2;

    if (_tileSetIs2D)
    {
        theta = orxMATH_KF_2_PI * ((1.0f / (_tilesInPolarRow * 2.0f)) + ((_unitDistanceFromPolarAxis - 1) * (1.0f / _tilesInPolarRow)));
    }
    else
    {
        theta = orxMATH_KF_2_PI;
        if (_unitDistanceFromPolarAxis == 0)
        {
            theta = 0;
        }
        else
        {
            if (halfSquare % 2 != 0)
            {
                theta *= (_unitDistanceFromPolarAxis * (1.0f / _tilesInPolarRow));
            }
            else
            {
                theta *= ((1.0f / (_tilesInPolarRow * 2.0f)) + ((_unitDistanceFromPolarAxis - 1) * (1.0f / _tilesInPolarRow)));
            }
        }
    }

    return theta;
}

void Tile::SetUp(
    const int &_row,
    const int &_col,
    const int &_square,
    const float &_tileSetRadius,
    const float &_normalizedTileSize,
    const float &_normalizedBorderSize,
    const orxVECTOR &_payloadRowAndCol,
    const orxVECTOR &_tileSetPos,
    const orxVECTOR &_tileSetSize,
    const TileSetState &_tileSetState)
{
    // Is the TileSet 2D?
    bool tileSetIs2D = _tileSetState == TileSetState::Cartesian2D || _tileSetState == TileSetState::Polar2D;
    // Is the TileSet Cartesian?
    bool tileSetIsCartesian = _tileSetState == TileSetState::Cartesian1D || _tileSetState == TileSetState::Cartesian2D;
    // Half the width/height of the TileSet.
    int halfSquare = _square / 2;
    // The payload's row within the TileSet.
    int payloadRow = _payloadRowAndCol.fX;
    // How many tiles away from the TileSet's pivot is this Tile's foreground?
    int unitDistanceFromOrigin = GetUnitDistanceFromTileSetCenter(_row, _col, _square, _payloadRowAndCol, false, _tileSetState);
    // How many tiles away from the TileSet's pivot is this Tile's background?
    int unitDistanceFromOriginBG = GetUnitDistanceFromTileSetCenter(_row, _col, _square, _payloadRowAndCol, true, _tileSetState);
    // How many tiles are in this Tile's circular, polar-2D row?
    int tilesInPolarRow = GetNumTilesInPolarRow(_square, unitDistanceFromOrigin, tileSetIs2D);
    // How many tiles away from the polar axis is this Tile?
    int unitDistanceFromPolarAxis = GetUnitDistanceFromTileSetPolarAxis(_row, _col, _square, unitDistanceFromOrigin, _payloadRowAndCol, tileSetIs2D);
    // How far away from the TileSet's pivot is this Tile's foreground's visual center?
    float radialDistance = GetDistanceFromTileSetCenter(_square, unitDistanceFromOrigin, _tileSetRadius, _payloadRowAndCol, tileSetIs2D, false, _tileSetState);
    // How far away from the TileSet's pivot is this Tile's background's visual center?
    float radialDistanceBG = GetDistanceFromTileSetCenter(_square, unitDistanceFromOriginBG, _tileSetRadius, _payloadRowAndCol, tileSetIs2D, true, _tileSetState);
    // At what angle is this Tile, in reference to the TileSet's pivot?
    float theta = GetPolarTheta(_square, unitDistanceFromPolarAxis, tilesInPolarRow, tileSetIs2D);

    m_bCartesian = tileSetIsCartesian;

    switch (_tileSetState)
    {
    case TileSetState::Cartesian1D:
        // Position.
        m_priorTileSetState = TileSetState::Cartesian1D;
        SetParentSpacePosition(GetGridRelativeCartesianPosition(_row, _col, _normalizedBorderSize, _normalizedTileSize));
        // Visual scale.
        m_visualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
        m_priorVisualScale = m_visualScale;
        break;
    case TileSetState::Cartesian2D:
        // Position.
        m_priorTileSetState = TileSetState::Cartesian2D;
        SetParentSpacePosition(GetGridRelativeCartesianPosition(_row, _col, _normalizedBorderSize, _normalizedTileSize));
        // Visual scale.
        m_visualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
        m_priorVisualScale = m_visualScale;
        break;
    case TileSetState::Polar1D:
        // Position.
        m_priorTileSetState = TileSetState::Polar1D;
        SetParentSpacePosition(CartesianToPolar2(GetGridRelativeCartesianPosition(_row, _col, _normalizedBorderSize, _normalizedTileSize)));
        // Visual scale.
        m_visualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
        m_priorVisualScale = m_visualScale;
        break;
    case TileSetState::Polar2D:
        // Position.
        m_priorTileSetState = TileSetState::Polar2D;
        SetParentSpacePosition(SquareToCircle(GetGridRelativeCartesianPosition(_row, _col, _normalizedBorderSize, _normalizedTileSize)));
        // Visual scale.
        m_visualScale = CalculateVisualScale(_square, tilesInPolarRow, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
        m_priorVisualScale = m_visualScale;
        break;
    }
    m_priorParentSpacePos = GetParentSpacePosition();
}

void Tile::Shift(
    const int &_row,
    const int &_col,
    const int &_square,
    const float &_tileSetRadius,
    const float &_normalizedTileSize,
    const float &_normalizedBorderSize,
    const float &_lerpWeight,
    const orxVECTOR &_payloadRowAndCol,
    const orxVECTOR &_tileSetPos,
    const TileSetState &_tileSetState,
    const TileSetShiftStatus &_tileSetShiftStatus)
{
    // Is the TileSet 2D?
    bool tileSetIs2D = _tileSetState == TileSetState::Cartesian2D || _tileSetState == TileSetState::Polar2D;
    // Is the TileSet Cartesian?
    bool tileSetIsCartesian = _tileSetState == TileSetState::Cartesian1D || _tileSetState == TileSetState::Cartesian2D;
    // Half the width/height of the TileSet.
    int halfSquare = _square / 2;
    // The payload's row within the TileSet.
    int payloadRow = _payloadRowAndCol.fX;
    // How many tiles away from the TileSet's pivot is this Tile's foreground?
    int unitDistanceFromOrigin = GetUnitDistanceFromTileSetCenter(_row, _col, _square, _payloadRowAndCol, false, _tileSetState);
    // How many tiles away from the TileSet's pivot is this Tile's background?
    int unitDistanceFromOriginBG = GetUnitDistanceFromTileSetCenter(_row, _col, _square, _payloadRowAndCol, true, _tileSetState);
    // How many tiles are in this Tile's circular, polar-2D row?
    int tilesInPolarRow = GetNumTilesInPolarRow(_square, unitDistanceFromOrigin, tileSetIs2D);
    // How many tiles away from the polar axis is this Tile?
    int unitDistanceFromPolarAxis = GetUnitDistanceFromTileSetPolarAxis(_row, _col, _square, unitDistanceFromOrigin, _payloadRowAndCol, tileSetIs2D);
    // How far away from the TileSet's pivot is this Tile's foreground's visual center?
    float radialDistance = GetDistanceFromTileSetCenter(_square, unitDistanceFromOrigin, _tileSetRadius, _payloadRowAndCol, tileSetIs2D, false, _tileSetState);
    // How far away from the TileSet's pivot is this Tile's background's visual center?
    float radialDistanceBG = GetDistanceFromTileSetCenter(_square, unitDistanceFromOriginBG, _tileSetRadius, _payloadRowAndCol, tileSetIs2D, true, _tileSetState);
    // At what angle is this Tile, in reference to the TileSet's pivot?
    float theta = GetPolarTheta(_square, unitDistanceFromPolarAxis, tilesInPolarRow, tileSetIs2D);
    // The center, in normalized parent space, of the Tile.
    orxVECTOR tileCenter = { 0.5f, 0.5f };

    m_bCartesian = tileSetIsCartesian;

    if (_tileSetShiftStatus == TileSetShiftStatus::D1Tiles || ((m_priorTileSetState == TileSetState::Cartesian1D || m_priorTileSetState == TileSetState::Polar1D) && !tileSetIs2D))
    {
        switch (_tileSetState)
        {
        case TileSetState::Cartesian1D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = GetGridRelativeCartesianPosition(payloadRow, _col, _normalizedBorderSize, _normalizedTileSize);
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, _lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, _lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Cartesian2D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = GetGridRelativeCartesianPosition(payloadRow, _col, _normalizedBorderSize, _normalizedTileSize);
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, _lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, _lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Polar1D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = CartesianToPolar2(GetGridRelativeCartesianPosition(payloadRow, _col, _normalizedBorderSize, _normalizedTileSize));
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, _lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, _lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Polar2D:
        {
            unitDistanceFromOrigin = GetUnitDistanceFromTileSetCenter(payloadRow, _col, _square, _payloadRowAndCol, false, _tileSetState);
            tilesInPolarRow = GetNumTilesInPolarRow(_square, unitDistanceFromOrigin, tileSetIs2D);

            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = SquareToCircle(GetGridRelativeCartesianPosition(payloadRow, _col, _normalizedBorderSize, _normalizedTileSize));
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, _lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, tilesInPolarRow, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, _lerpWeight);
            m_visualScale = visualScale;
        }
        }
    }
    else
    {
        switch (_tileSetState)
        {
        case TileSetState::Cartesian1D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = GetGridRelativeCartesianPosition(_row, _col, _normalizedBorderSize, _normalizedTileSize);
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, _lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, _lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Cartesian2D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = GetGridRelativeCartesianPosition(_row, _col, _normalizedBorderSize, _normalizedTileSize);
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, _lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, _lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Polar1D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = CartesianToPolar2(GetGridRelativeCartesianPosition(_row, _col, _normalizedBorderSize, _normalizedTileSize));
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, _lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, _lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Polar2D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = SquareToCircle(GetGridRelativeCartesianPosition(_row, _col, _normalizedBorderSize, _normalizedTileSize));
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, _lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, tilesInPolarRow, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, _lerpWeight);
            m_visualScale = visualScale;
        }
        }
    }
}

const orxVECTOR Tile::CalculateVisualScale(
    const int &_square,
    const int &_tilesInRow,
    const float &_tileSetRadius,
    const float &_normalizedTileSize,
    const orxVECTOR &_tileSetPos,
    const TileSetState &_tileSetState)
{
    float normalizedRadialDistance;
    float normalizedCircumference;
    float normalizedWidth;
    float normalizedHeight;

    switch (_tileSetState)
    {
    case TileSetState::Polar1D:
        normalizedRadialDistance = orxVector_GetDistance(&GetPosition(), &_tileSetPos) / _tileSetRadius;
        normalizedCircumference = 2.0f * orxMATH_KF_PI * normalizedRadialDistance;
        normalizedWidth = normalizedCircumference / _tilesInRow;
        normalizedHeight = (_tileSetRadius / _tilesInRow) / _tileSetRadius;
        break;
    case TileSetState::Polar2D:
        normalizedRadialDistance = orxVector_GetDistance(&GetPosition(), &_tileSetPos) / _tileSetRadius;
        normalizedCircumference = 2.0f * orxMATH_KF_PI * normalizedRadialDistance;
        normalizedWidth = normalizedCircumference / _tilesInRow;
        normalizedHeight = _tileSetRadius / (_square / 2.0f) / _tileSetRadius;
        break;
    default:
        normalizedWidth = _normalizedTileSize * 2.0f;
        normalizedHeight = _normalizedTileSize * 2.0f;
        break;
    }

    return { normalizedWidth, normalizedHeight };
}
