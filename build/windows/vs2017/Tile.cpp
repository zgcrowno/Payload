#include "Tile.h"

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

const int Tile::GetUnitDistanceFromTileSetCenter(
    const int &_row,
    const int &_col,
    const int &_square,
    const int &_payloadRow,
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
        unitDistanceFromOrigin = _square - _payloadRow;
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

void Tile::SetUp(
    const int &_row,
    const int &_col,
    const int &_square,
    const float &_tileSetRadius,
    const float &_normalizedTileSize,
    const float &_normalizedBorderSize,
    const int &_payloadRow,
    const orxVECTOR &_tileSetPos,
    const TileSetState &_tileSetState)
{
    m_bCartesian = _tileSetState == TileSetState::Cartesian1D || _tileSetState == TileSetState::Cartesian2D;
    m_b2D = _tileSetState == TileSetState::Cartesian2D || _tileSetState == TileSetState::Polar2D;
    m_row = _row;
    m_col = _col;

    // Is the TileSet 2D?
    bool tileSetIs2D = _tileSetState == TileSetState::Cartesian2D || _tileSetState == TileSetState::Polar2D;
    // How many tiles away from the TileSet's pivot is this Tile's foreground?
    int unitDistanceFromOrigin = GetUnitDistanceFromTileSetCenter(m_row, m_col, _square, _payloadRow, _tileSetState);
    // How many tiles are in this Tile's circular, polar-2D row?
    int tilesInPolarRow = GetNumTilesInPolarRow(_square, unitDistanceFromOrigin, tileSetIs2D);

    switch (_tileSetState)
    {
    case TileSetState::Cartesian1D:
        // Position.
        m_priorTileSetState = TileSetState::Cartesian1D;
        m_targetParentSpacePos = GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize);
        SetParentSpacePosition(m_targetParentSpacePos);
        // Visual scale.
        m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
        m_visualScale = m_targetVisualScale;
        m_priorVisualScale = m_visualScale;
        break;
    case TileSetState::Cartesian2D:
        // Position.
        m_priorTileSetState = TileSetState::Cartesian2D;
        m_targetParentSpacePos = GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize);
        SetParentSpacePosition(m_targetParentSpacePos);
        // Visual scale.
        m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
        m_visualScale = m_targetVisualScale;
        m_priorVisualScale = m_visualScale;
        break;
    case TileSetState::Polar1D:
        // Position.
        m_priorTileSetState = TileSetState::Polar1D;
        m_targetParentSpacePos = CartesianToPolar2(GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize));
        SetParentSpacePosition(m_targetParentSpacePos);
        // Visual scale.
        m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
        m_visualScale = m_targetVisualScale;
        m_priorVisualScale = m_visualScale;
        break;
    case TileSetState::Polar2D:
        // Position.
        m_priorTileSetState = TileSetState::Polar2D;
        m_targetParentSpacePos = SquareToCircle(GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize));
        SetParentSpacePosition(m_targetParentSpacePos);
        // Visual scale.
        m_targetVisualScale = CalculateVisualScale(_square, tilesInPolarRow, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
        m_visualScale = m_targetVisualScale;
        m_priorVisualScale = m_visualScale;
        break;
    }
    m_priorParentSpacePos = GetParentSpacePosition();
}

void Tile::Shift(
    const int &_square,
    const int &_greatest1DUnitDistanceOfPayloadRowFromThreshold,
    const float &_tileSetRadius,
    const float &_normalizedTileSize,
    const float &_normalizedBorderSize,
    const float &_lerpWeight,
    const int &_payloadRow,
    const orxVECTOR &_tileSetPos,
    const TileSetState &_tileSetState,
    const TileSetShiftStatus &_tileSetShiftStatus)
{
    m_bCartesian = _tileSetState == TileSetState::Cartesian1D || _tileSetState == TileSetState::Cartesian2D;
    m_b2D = _tileSetState == TileSetState::Cartesian2D || _tileSetState == TileSetState::Polar2D;

    // Is the TileSet 2D?
    bool tileSetIs2D = _tileSetState == TileSetState::Cartesian2D || _tileSetState == TileSetState::Polar2D;
    // How many tiles away from the TileSet's pivot is this Tile's foreground?
    int unitDistanceFromOrigin = GetUnitDistanceFromTileSetCenter(m_row, m_col, _square, _payloadRow, _tileSetState);
    // How many tiles are in this Tile's circular, polar-2D row?
    int tilesInPolarRow = GetNumTilesInPolarRow(_square, unitDistanceFromOrigin, tileSetIs2D);

    if (_tileSetShiftStatus == TileSetShiftStatus::D1Tiles || ((m_priorTileSetState == TileSetState::Cartesian1D || m_priorTileSetState == TileSetState::Polar1D) && !tileSetIs2D))
    {
        int unitDistanceFromPayloadRow = fabsf(m_row - _payloadRow);
        // Set this so as to prevent division by 0.
        float lerpWeight =
            _tileSetShiftStatus != TileSetShiftStatus::D1Tiles || unitDistanceFromPayloadRow == 0 ?
            _lerpWeight :
            orxCLAMP(_lerpWeight * ((float)_greatest1DUnitDistanceOfPayloadRowFromThreshold / unitDistanceFromPayloadRow), 0.0f, 1.0f);
        // The Tile is only moving if it hasn't reached the end of its lerp.
        if (lerpWeight < 1)
        {
            m_bIsMoving = true;
        }
        else
        {
            m_bIsMoving = false;
        }

        switch (_tileSetState)
        {
        case TileSetState::Cartesian1D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = GetGridRelativeCartesianPosition(_payloadRow, m_col, _normalizedBorderSize, _normalizedTileSize);
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Cartesian2D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = GetGridRelativeCartesianPosition(_payloadRow, m_col, _normalizedBorderSize, _normalizedTileSize);
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Polar1D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = CartesianToPolar2(GetGridRelativeCartesianPosition(_payloadRow, m_col, _normalizedBorderSize, _normalizedTileSize));
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, _square, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, lerpWeight);
            m_visualScale = visualScale;
            break;
        }
        case TileSetState::Polar2D:
        {
            unitDistanceFromOrigin = GetUnitDistanceFromTileSetCenter(_payloadRow, m_col, _square, _payloadRow, _tileSetState);
            tilesInPolarRow = GetNumTilesInPolarRow(_square, unitDistanceFromOrigin, tileSetIs2D);

            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = SquareToCircle(GetGridRelativeCartesianPosition(_payloadRow, m_col, _normalizedBorderSize, _normalizedTileSize));
            orxVector_Lerp(&parentSpacePos, &m_priorParentSpacePos, &m_targetParentSpacePos, lerpWeight);
            SetParentSpacePosition(parentSpacePos);
            // Visual scale.
            orxVECTOR visualScale;
            m_targetVisualScale = CalculateVisualScale(_square, tilesInPolarRow, _tileSetRadius, _normalizedTileSize, _tileSetPos, _tileSetState);
            orxVector_Lerp(&visualScale, &m_priorVisualScale, &m_targetVisualScale, lerpWeight);
            m_visualScale = visualScale;
        }
        }
    }
    else
    {
        // The Tile is only moving if it hasn't reached the end of its lerp.
        if (_lerpWeight < 1)
        {
            m_bIsMoving = true;
        }
        else
        {
            m_bIsMoving = false;
        }

        switch (_tileSetState)
        {
        case TileSetState::Cartesian1D:
        {
            // Position.
            orxVECTOR parentSpacePos;
            m_targetParentSpacePos = GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize);
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
            m_targetParentSpacePos = GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize);
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
            m_targetParentSpacePos = CartesianToPolar2(GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize));
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
            m_targetParentSpacePos = SquareToCircle(GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize));
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

void Tile::Reconfigure(
    const int &_payloadRow,
    const float &_lerpWeight,
    const float &_normalizedBorderSize,
    const float &_normalizedTileSize,
    const orxVECTOR &_memorySetPos,
    const TileSetState &_tileSetState)
{
    orxVECTOR polarPos = CartesianToPolar(GetPosition(), _memorySetPos);
    float thetaDest = m_priorMemSetTheta - orxMATH_KF_PI;
    switch (_tileSetState)
    {
    case TileSetState::Cartesian1D:
        m_targetParentSpacePos = GetGridRelativeCartesianPosition(_payloadRow, m_col, _normalizedBorderSize, _normalizedTileSize);
        break;
    case TileSetState::Cartesian2D:
        thetaDest = m_priorMemSetTheta - orxMATH_KF_PI_BY_2;
        m_targetParentSpacePos = GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize);
        break;
    case TileSetState::Polar1D:
        m_targetParentSpacePos = CartesianToPolar2(GetGridRelativeCartesianPosition(_payloadRow, m_col, _normalizedBorderSize, _normalizedTileSize));
        break;
    case TileSetState::Polar2D:
        m_targetParentSpacePos = SquareToCircle(GetGridRelativeCartesianPosition(m_row, m_col, _normalizedBorderSize, _normalizedTileSize));
        break;
    }
    float theta = orxLERP(m_priorMemSetTheta, thetaDest, _lerpWeight);
    if (_lerpWeight < 1)
    {
        m_bIsMoving = true;
    }
    else
    {
        m_bIsMoving = false;
    }
    SetPolarPosition(_memorySetPos, polarPos.fX, theta);
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
