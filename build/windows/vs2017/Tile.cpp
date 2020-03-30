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

    switch (_tileSetState)
    {
    case TileSetState::Cartesian1D:
        SetParentSpacePosition({
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_col * (_normalizedBorderSize + _normalizedTileSize)),
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_row * (_normalizedBorderSize + _normalizedTileSize)) });
        // Foreground.
        m_bCartesian = tileSetIsCartesian;
        m_leftEdgeTopPoint = {
            (float)_col / _square,
            (float)payloadRow / _square };
        m_leftEdgeBottomPoint = {
            (float)_col / _square,
            (payloadRow + 1.0f) / _square };
        m_rightEdgeTopPoint = {
            (_col + 1.0f) / _square,
            (float)payloadRow / _square };
        m_rightEdgeBottomPoint = {
            (_col + 1.0f) / _square,
            (payloadRow + 1.0f) / _square };
        m_topRadius = ((float)unitDistanceFromOrigin / halfSquare) * 0.5f;
        m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / halfSquare) * 0.5f;
        m_visualCenter = {
            _tileSetPos.fX - _tileSetRadius + (_tileSetRadius / _square) + (_col * (_tileSetRadius / halfSquare)),
            _tileSetPos.fY - _tileSetRadius + (_tileSetRadius / _square) + (payloadRow * (_tileSetRadius / halfSquare)) };
        SetVisualScale();
        // Background.
        m_leftEdgeTopPointBG = {
            (float)_col / _square,
            (float)_row / _square };
        m_leftEdgeBottomPointBG = {
            (float)_col / _square,
            (_row + 1.0f) / _square };
        m_rightEdgeTopPointBG = {
            (_col + 1.0f) / _square,
            (float)_row / _square };
        m_rightEdgeBottomPointBG = {
            (_col + 1.0f) / _square,
            (_row + 1.0f) / _square };
        m_topRadiusBG = ((float)unitDistanceFromOriginBG / halfSquare) * 0.5f;
        m_bottomRadiusBG = ((unitDistanceFromOriginBG - 1.0f) / halfSquare) * 0.5f;
        break;
    case TileSetState::Cartesian2D:
        SetParentSpacePosition({
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_col * (_normalizedBorderSize + _normalizedTileSize)),
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_row * (_normalizedBorderSize + _normalizedTileSize)) });
        // Foreground.
        m_bCartesian = tileSetIsCartesian;
        m_leftEdgeTopPoint = {
            (float)_col / _square,
            (float)_row / _square };
        m_leftEdgeBottomPoint = {
            (float)_col / _square,
            (_row + 1.0f) / _square };
        m_rightEdgeTopPoint = {
            (_col + 1.0f) / _square,
            (float)_row / _square };
        m_rightEdgeBottomPoint = {
            (_col + 1.0f) / _square,
            (_row + 1.0f) / _square };
        m_topRadius = ((float)unitDistanceFromOrigin / halfSquare) * 0.5f;
        m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / halfSquare) * 0.5f;
        m_visualCenter = {
            _tileSetPos.fX - _tileSetRadius + (_tileSetRadius / _square) + (_col * (_tileSetRadius / halfSquare)),
            _tileSetPos.fY - _tileSetRadius + (_tileSetRadius / _square) + (_row * (_tileSetRadius / halfSquare)) };
        SetVisualScale();
        // Background.
        m_leftEdgeTopPointBG = {
            (float)_col / _square,
            (float)_row / _square };
        m_leftEdgeBottomPointBG = {
            (float)_col / _square,
            (_row + 1.0f) / _square };
        m_rightEdgeTopPointBG = {
            (_col + 1.0f) / _square,
            (float)_row / _square };
        m_rightEdgeBottomPointBG = {
            (_col + 1.0f) / _square,
            (_row + 1.0f) / _square };
        m_topRadiusBG = ((float)unitDistanceFromOriginBG / halfSquare) * 0.5f;
        m_bottomRadiusBG = ((unitDistanceFromOriginBG - 1.0f) / halfSquare) * 0.5f;
        break;
    case TileSetState::Polar1D:
        SetParentSpacePosition({
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_col * (_normalizedBorderSize + _normalizedTileSize)),
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_row * (_normalizedBorderSize + _normalizedTileSize)) });
        SetPolarPosition2(
            _tileSetPos,
            _tileSetSize);
        // Foreground.
        m_bCartesian = tileSetIsCartesian;
        SetPolarPosition(m_leftEdgeTopPoint,
            { 0.5f, 0.5f },
            ((radialDistance / _tileSetRadius) + ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_leftEdgeBottomPoint,
            { 0.5f, 0.5f },
            ((radialDistance / _tileSetRadius) - ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_rightEdgeTopPoint,
            { 0.5f, 0.5f },
            ((radialDistance / _tileSetRadius) + ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_rightEdgeBottomPoint,
            { 0.5f, 0.5f },
            ((radialDistance / _tileSetRadius) - ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        m_topRadius = ((float)unitDistanceFromOrigin / _square) * 0.5f;
        m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / _square) * 0.5f;
        SetPolarPosition(m_visualCenter, _tileSetPos, radialDistance, theta);
        SetVisualScale();
        // Background.
        SetPolarPosition(m_leftEdgeTopPointBG,
            { 0.5f, 0.5f },
            ((radialDistanceBG / _tileSetRadius) + ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_leftEdgeBottomPointBG,
            { 0.5f, 0.5f },
            ((radialDistanceBG / _tileSetRadius) - ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_rightEdgeTopPointBG,
            { 0.5f, 0.5f },
            ((radialDistanceBG / _tileSetRadius) + ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_rightEdgeBottomPointBG,
            { 0.5f, 0.5f },
            ((radialDistanceBG / _tileSetRadius) - ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        m_topRadiusBG = ((float)unitDistanceFromOriginBG / _square) * 0.5f;
        m_bottomRadiusBG = ((unitDistanceFromOriginBG - 1.0f) / _square) * 0.5f;
        break;
    case TileSetState::Polar2D:
        SetParentSpacePosition(SquareToCircle({
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_col * (_normalizedBorderSize + _normalizedTileSize)),
            -0.5f + (_normalizedBorderSize + (_normalizedTileSize / 2.0f)) + (_row * (_normalizedBorderSize + _normalizedTileSize)) }));
        // Foreground.
        m_bCartesian = tileSetIsCartesian;
        SetPolarPosition(m_leftEdgeTopPoint,
            { 0.5f, 0.5f },
            ((radialDistance / _tileSetRadius) + ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_leftEdgeBottomPoint,
            { 0.5f, 0.5f },
            ((radialDistance / _tileSetRadius) - ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_rightEdgeTopPoint,
            { 0.5f, 0.5f },
            ((radialDistance / _tileSetRadius) + ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_rightEdgeBottomPoint,
            { 0.5f, 0.5f },
            ((radialDistance / _tileSetRadius) - ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        m_topRadius = ((float)unitDistanceFromOrigin / halfSquare) * 0.5f;
        m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / halfSquare) * 0.5f;
        SetPolarPosition(m_visualCenter, _tileSetPos, radialDistance, theta);
        SetVisualScale();
        // Background.
        SetPolarPosition(m_leftEdgeTopPointBG,
            { 0.5f, 0.5f },
            ((radialDistanceBG / _tileSetRadius) + ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_leftEdgeBottomPointBG,
            { 0.5f, 0.5f },
            ((radialDistanceBG / _tileSetRadius) - ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_rightEdgeTopPointBG,
            { 0.5f, 0.5f },
            ((radialDistanceBG / _tileSetRadius) + ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        SetPolarPosition(m_rightEdgeBottomPointBG,
            { 0.5f, 0.5f },
            ((radialDistanceBG / _tileSetRadius) - ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
        m_topRadiusBG = ((float)unitDistanceFromOriginBG / halfSquare) * 0.5f;
        m_bottomRadiusBG = ((unitDistanceFromOriginBG - 1.0f) / halfSquare) * 0.5f;
        break;
    }
}

void Tile::Shift(
    const int &_row,
    const int &_col,
    const int &_square,
    const float &_tileSetRadius,
    const float &_lerpWeight,
    const orxVECTOR &_payloadRowAndCol,
    const orxVECTOR &_tileSetPos,
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
    // The center, in normalized parent space, of the Tile.
    orxVECTOR tileCenter = { 0.5f, 0.5f };

    switch (_tileSetState)
    {
    case TileSetState::Cartesian1D:
    {
        // Foreground.
        m_bCartesian = tileSetIsCartesian;
        orxVECTOR leftEdgeTopPointDest = { (float)_col / _square, (float)payloadRow / _square };
        orxVector_Lerp(&m_leftEdgeTopPoint, &m_leftEdgeTopPoint, &leftEdgeTopPointDest, _lerpWeight);
        orxVECTOR leftEdgeBottomPointDest = { (float)_col / _square, (payloadRow + 1.0f) / _square };
        orxVector_Lerp(&m_leftEdgeBottomPoint, &m_leftEdgeBottomPoint, &leftEdgeBottomPointDest, _lerpWeight);
        orxVECTOR rightEdgeTopPointDest = { (_col + 1.0f) / _square, (float)payloadRow / _square };
        orxVector_Lerp(&m_rightEdgeTopPoint, &m_rightEdgeTopPoint, &rightEdgeTopPointDest, _lerpWeight);
        orxVECTOR rightEdgeBottomPointDest = { (_col + 1.0f) / _square, (payloadRow + 1.0f) / _square };
        orxVector_Lerp(&m_rightEdgeBottomPoint, &m_rightEdgeBottomPoint, &rightEdgeBottomPointDest, _lerpWeight);
        m_topRadius = orxLERP(m_topRadius, ((float)unitDistanceFromOrigin / halfSquare) * 0.5f, _lerpWeight);
        m_bottomRadius = orxLERP(m_bottomRadius, ((unitDistanceFromOrigin - 1.0f) / halfSquare) * 0.5f, _lerpWeight);
        orxVECTOR visualCenterDest = {
            _tileSetPos.fX - _tileSetRadius + (_tileSetRadius / _square) + (_col * (_tileSetRadius / halfSquare)),
            _tileSetPos.fY - _tileSetRadius + (_tileSetRadius / _square) + (payloadRow * (_tileSetRadius / halfSquare)) };
        orxVector_Lerp(&m_visualCenter, &m_visualCenter, &visualCenterDest, _lerpWeight);
        SetVisualScale();
        // Background.
        orxVECTOR leftEdgeTopPointBGDest = { (float)_col / _square, (float)_row / _square };
        orxVector_Lerp(&m_leftEdgeTopPointBG, &m_leftEdgeTopPointBG, &leftEdgeTopPointBGDest, _lerpWeight);
        orxVECTOR leftEdgeBottomPointBGDest = { (float)_col / _square, (_row + 1.0f) / _square };
        orxVector_Lerp(&m_leftEdgeBottomPointBG, &m_leftEdgeBottomPointBG, &leftEdgeBottomPointBGDest, _lerpWeight);
        orxVECTOR rightEdgeTopPointBGDest = { (_col + 1.0f) / _square, (float)_row / _square };
        orxVector_Lerp(&m_rightEdgeTopPointBG, &m_rightEdgeTopPointBG, &rightEdgeTopPointBGDest, _lerpWeight);
        orxVECTOR rightEdgeBottomPointBGDest = { (_col + 1.0f) / _square, (_row + 1.0f) / _square };
        orxVector_Lerp(&m_rightEdgeBottomPointBG, &m_rightEdgeBottomPointBG, &rightEdgeBottomPointBGDest, _lerpWeight);
        m_topRadiusBG = orxLERP(m_topRadiusBG, ((float)unitDistanceFromOriginBG / halfSquare) * 0.5f, _lerpWeight);
        m_bottomRadiusBG = orxLERP(m_bottomRadiusBG, ((unitDistanceFromOriginBG - 1.0f) / halfSquare) * 0.5f, _lerpWeight);
        break;
    }
    case TileSetState::Cartesian2D:
    {
        // Foreground.
        m_bCartesian = tileSetIsCartesian;
        orxVECTOR leftEdgeTopPointDest = { (float)_col / _square, (float)_row / _square };
        orxVector_Lerp(&m_leftEdgeTopPoint, &m_leftEdgeTopPoint, &leftEdgeTopPointDest, _lerpWeight);
        orxVECTOR leftEdgeBottomPointDest = { (float)_col / _square, (_row + 1.0f) / _square };
        orxVector_Lerp(&m_leftEdgeBottomPoint, &m_leftEdgeBottomPoint, &leftEdgeBottomPointDest, _lerpWeight);
        orxVECTOR rightEdgeTopPointDest = { (_col + 1.0f) / _square, (float)_row / _square };
        orxVector_Lerp(&m_rightEdgeTopPoint, &m_rightEdgeTopPoint, &rightEdgeTopPointDest, _lerpWeight);
        orxVECTOR rightEdgeBottomPointDest = { (_col + 1.0f) / _square, (_row + 1.0f) / _square };
        orxVector_Lerp(&m_rightEdgeBottomPoint, &m_rightEdgeBottomPoint, &rightEdgeBottomPointDest, _lerpWeight);
        m_topRadius = orxLERP(m_topRadius, ((float)unitDistanceFromOrigin / halfSquare) * 0.5f, _lerpWeight);
        m_bottomRadius = orxLERP(m_bottomRadius, ((unitDistanceFromOrigin - 1.0f) / halfSquare) * 0.5f, _lerpWeight);
        orxVECTOR visualCenterDest = {
            _tileSetPos.fX - _tileSetRadius + (_tileSetRadius / _square) + (_col * (_tileSetRadius / halfSquare)),
            _tileSetPos.fY - _tileSetRadius + (_tileSetRadius / _square) + (_row * (_tileSetRadius / halfSquare)) };
        orxVector_Lerp(&m_visualCenter, &m_visualCenter, &visualCenterDest, _lerpWeight);
        SetVisualScale();
        // Background.
        orxVECTOR leftEdgeTopPointBGDest = { (float)_col / _square, (float)_row / _square };
        orxVector_Lerp(&m_leftEdgeTopPointBG, &m_leftEdgeTopPointBG, &leftEdgeTopPointBGDest, _lerpWeight);
        orxVECTOR leftEdgeBottomPointBGDest = { (float)_col / _square, (_row + 1.0f) / _square };
        orxVector_Lerp(&m_leftEdgeBottomPointBG, &m_leftEdgeBottomPointBG, &leftEdgeBottomPointBGDest, _lerpWeight);
        orxVECTOR rightEdgeTopPointBGDest = { (_col + 1.0f) / _square, (float)_row / _square };
        orxVector_Lerp(&m_rightEdgeTopPointBG, &m_rightEdgeTopPointBG, &rightEdgeTopPointBGDest, _lerpWeight);
        orxVECTOR rightEdgeBottomPointBGDest = { (_col + 1.0f) / _square, (_row + 1.0f) / _square };
        orxVector_Lerp(&m_rightEdgeBottomPointBG, &m_rightEdgeBottomPointBG, &rightEdgeBottomPointBGDest, _lerpWeight);
        m_topRadiusBG = orxLERP(m_topRadiusBG, ((float)unitDistanceFromOriginBG / halfSquare) * 0.5f, _lerpWeight);
        m_bottomRadiusBG = orxLERP(m_bottomRadiusBG, ((unitDistanceFromOriginBG - 1.0f) / halfSquare) * 0.5f, _lerpWeight);
        break;
    }
    case TileSetState::Polar1D:
    {
        // Foreground.
        m_bCartesian = tileSetIsCartesian;
        float leftEdgeTopPointRadialDistance = orxLERP(
            orxVector_GetDistance(&m_leftEdgeTopPoint, &tileCenter),
            ((radialDistance / _tileSetRadius) + ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float leftEdgeTopPointTheta = LerpAngle(
            CartesianToPolar(m_leftEdgeTopPoint, tileCenter).fY,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_leftEdgeTopPoint,
            tileCenter,
            leftEdgeTopPointRadialDistance,
            leftEdgeTopPointTheta);
        float leftEdgeBottomPointRadialDistance = orxLERP(
            orxVector_GetDistance(&m_leftEdgeBottomPoint, &tileCenter),
            ((radialDistance / _tileSetRadius) - ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float leftEdgeBottomPointTheta = LerpAngle(
            CartesianToPolar(m_leftEdgeBottomPoint, tileCenter).fY,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_leftEdgeBottomPoint,
            tileCenter,
            leftEdgeBottomPointRadialDistance,
            leftEdgeBottomPointTheta);
        float rightEdgeTopPointRadialDistance = orxLERP(
            orxVector_GetDistance(&m_rightEdgeTopPoint, &tileCenter),
            ((radialDistance / _tileSetRadius) + ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float rightEdgeTopPointTheta = LerpAngle(
            CartesianToPolar(m_rightEdgeTopPoint, tileCenter).fY,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_rightEdgeTopPoint,
            tileCenter,
            rightEdgeTopPointRadialDistance,
            rightEdgeTopPointTheta);
        float rightEdgeBottomPointRadialDistance = orxLERP(
            orxVector_GetDistance(&m_rightEdgeBottomPoint, &tileCenter),
            ((radialDistance / _tileSetRadius) - ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float rightEdgeBottomPointTheta = LerpAngle(
            CartesianToPolar(m_rightEdgeBottomPoint, tileCenter).fY,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_rightEdgeBottomPoint,
            tileCenter,
            rightEdgeBottomPointRadialDistance,
            rightEdgeBottomPointTheta);
        m_topRadius = orxLERP(
            m_topRadius,
            ((float)unitDistanceFromOrigin / _square) * 0.5f,
            _lerpWeight);
        m_bottomRadius = orxLERP(
            m_bottomRadius,
            ((unitDistanceFromOrigin - 1.0f) / _square) * 0.5f,
            _lerpWeight);
        float visualCenterRadialDistance = orxLERP(
            orxVector_GetDistance(&m_visualCenter, &_tileSetPos),
            radialDistance,
            _lerpWeight);
        float visualCenterTheta = LerpAngle(
            CartesianToPolar(m_visualCenter, _tileSetPos).fY,
            theta,
            _lerpWeight);
        SetPolarPosition(m_visualCenter,
            _tileSetPos,
            visualCenterRadialDistance,
            visualCenterTheta);
        SetVisualScale();
        // Background.
        float leftEdgeTopPointBGRadialDistance = orxLERP(
            orxVector_GetDistance(&m_leftEdgeTopPointBG, &tileCenter),
            ((radialDistanceBG / _tileSetRadius) + ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float leftEdgeTopPointBGTheta = LerpAngle(
            CartesianToPolar(m_leftEdgeTopPointBG, tileCenter).fY,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_leftEdgeTopPointBG,
            tileCenter,
            leftEdgeTopPointBGRadialDistance,
            leftEdgeTopPointBGTheta);
        float leftEdgeBottomPointBGRadialDistance = orxLERP(
            orxVector_GetDistance(&m_leftEdgeBottomPointBG, &tileCenter),
            ((radialDistanceBG / _tileSetRadius) - ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float leftEdgeBottomPointBGTheta = LerpAngle(
            CartesianToPolar(m_leftEdgeBottomPointBG, tileCenter).fY,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_leftEdgeBottomPointBG,
            tileCenter,
            leftEdgeBottomPointBGRadialDistance,
            leftEdgeBottomPointBGTheta);
        float rightEdgeTopPointBGRadialDistance = orxLERP(
            orxVector_GetDistance(&m_rightEdgeTopPointBG, &tileCenter),
            ((radialDistanceBG / _tileSetRadius) + ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float rightEdgeTopPointBGTheta = LerpAngle(
            CartesianToPolar(m_rightEdgeTopPointBG, tileCenter).fY,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_rightEdgeTopPointBG,
            tileCenter,
            rightEdgeTopPointBGRadialDistance,
            rightEdgeTopPointBGTheta);
        float rightEdgeBottomPointBGRadialDistance = orxLERP(
            orxVector_GetDistance(&m_rightEdgeBottomPointBG, &tileCenter),
            ((radialDistanceBG / _tileSetRadius) - ((_tileSetRadius / (_square * 2.0f)) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float rightEdgeBottomPointBGTheta = LerpAngle(
            CartesianToPolar(m_rightEdgeBottomPointBG, tileCenter).fY,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_rightEdgeBottomPointBG,
            tileCenter,
            rightEdgeBottomPointBGRadialDistance,
            rightEdgeBottomPointBGTheta);
        m_topRadiusBG = orxLERP(
            m_topRadiusBG,
            ((float)unitDistanceFromOriginBG / _square) * 0.5f,
            _lerpWeight);
        m_bottomRadiusBG = orxLERP(
            m_bottomRadiusBG,
            ((unitDistanceFromOriginBG - 1.0f) / _square) * 0.5f,
            _lerpWeight);
        break;
    }
    case TileSetState::Polar2D:
    {
        // Foreground.
        m_bCartesian = tileSetIsCartesian;
        float leftEdgeTopPointRadialDistance = orxLERP(
            orxVector_GetDistance(&m_leftEdgeTopPoint, &tileCenter),
            ((radialDistance / _tileSetRadius) + ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float leftEdgeTopPointTheta = LerpAngle(
            CartesianToPolar(m_leftEdgeTopPoint, tileCenter).fY,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_leftEdgeTopPoint,
            tileCenter,
            leftEdgeTopPointRadialDistance,
            leftEdgeTopPointTheta);
        float leftEdgeBottomPointRadialDistance = orxLERP(
            orxVector_GetDistance(&m_leftEdgeBottomPoint, &tileCenter),
            ((radialDistance / _tileSetRadius) - ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float leftEdgeBottomPointTheta = LerpAngle(
            CartesianToPolar(m_leftEdgeBottomPoint, tileCenter).fY,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_leftEdgeBottomPoint,
            tileCenter,
            leftEdgeBottomPointRadialDistance,
            leftEdgeBottomPointTheta);
        float rightEdgeTopPointRadialDistance = orxLERP(
            orxVector_GetDistance(&m_rightEdgeTopPoint, &tileCenter),
            ((radialDistance / _tileSetRadius) + ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float rightEdgeTopPointTheta = LerpAngle(
            CartesianToPolar(m_rightEdgeTopPoint, tileCenter).fY,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_rightEdgeTopPoint,
            tileCenter,
            rightEdgeTopPointRadialDistance,
            rightEdgeTopPointTheta);
        float rightEdgeBottomPointRadialDistance = orxLERP(
            orxVector_GetDistance(&m_rightEdgeBottomPoint, &tileCenter),
            ((radialDistance / _tileSetRadius) - ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float rightEdgeBottomPointTheta = LerpAngle(
            CartesianToPolar(m_rightEdgeBottomPoint, tileCenter).fY,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_rightEdgeBottomPoint,
            tileCenter,
            rightEdgeBottomPointRadialDistance,
            rightEdgeBottomPointTheta);
        m_topRadius = orxLERP(
            m_topRadius,
            ((float)unitDistanceFromOrigin / halfSquare) * 0.5f,
            _lerpWeight);
        m_bottomRadius = orxLERP(
            m_bottomRadius,
            ((unitDistanceFromOrigin - 1.0f) / halfSquare) * 0.5f,
            _lerpWeight);
        float visualCenterRadialDistance = orxLERP(
            orxVector_GetDistance(&m_visualCenter, &_tileSetPos),
            radialDistance,
            _lerpWeight);
        float visualCenterTheta = LerpAngle(
            CartesianToPolar(m_visualCenter, _tileSetPos).fY,
            theta,
            _lerpWeight);
        SetPolarPosition(m_visualCenter,
            _tileSetPos,
            visualCenterRadialDistance,
            visualCenterTheta);
        SetVisualScale();
        // Background.
        float leftEdgeTopPointBGRadialDistance = orxLERP(
            orxVector_GetDistance(&m_leftEdgeTopPointBG, &tileCenter),
            ((radialDistanceBG / _tileSetRadius) + ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float leftEdgeTopPointBGTheta = LerpAngle(
            CartesianToPolar(m_leftEdgeTopPointBG, tileCenter).fY,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_leftEdgeTopPointBG,
            tileCenter,
            leftEdgeTopPointBGRadialDistance,
            leftEdgeTopPointBGTheta);
        float leftEdgeBottomPointBGRadialDistance = orxLERP(
            orxVector_GetDistance(&m_leftEdgeBottomPointBG, &tileCenter),
            ((radialDistanceBG / _tileSetRadius) - ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float leftEdgeBottomPointBGTheta = LerpAngle(
            CartesianToPolar(m_leftEdgeBottomPointBG, tileCenter).fY,
            theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_leftEdgeBottomPointBG,
            tileCenter,
            leftEdgeBottomPointBGRadialDistance,
            leftEdgeBottomPointBGTheta);
        float rightEdgeTopPointBGRadialDistance = orxLERP(
            orxVector_GetDistance(&m_rightEdgeTopPointBG, &tileCenter),
            ((radialDistanceBG / _tileSetRadius) + ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float rightEdgeTopPointBGTheta = LerpAngle(
            CartesianToPolar(m_rightEdgeTopPointBG, tileCenter).fY,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_rightEdgeTopPointBG,
            tileCenter,
            rightEdgeTopPointBGRadialDistance,
            rightEdgeTopPointBGTheta);
        float rightEdgeBottomPointBGRadialDistance = orxLERP(
            orxVector_GetDistance(&m_rightEdgeBottomPointBG, &tileCenter),
            ((radialDistanceBG / _tileSetRadius) - ((_tileSetRadius / _square) / _tileSetRadius)) / 2.0f,
            _lerpWeight);
        float rightEdgeBottomPointBGTheta = LerpAngle(
            CartesianToPolar(m_rightEdgeBottomPointBG, tileCenter).fY,
            theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
            _lerpWeight);
        SetPolarPosition(m_rightEdgeBottomPointBG,
            tileCenter,
            rightEdgeBottomPointBGRadialDistance,
            rightEdgeBottomPointBGTheta);
        m_topRadiusBG = orxLERP(
            m_topRadiusBG,
            ((float)unitDistanceFromOriginBG / halfSquare) * 0.5f,
            _lerpWeight);
        m_bottomRadiusBG = orxLERP(
            m_bottomRadiusBG,
            ((unitDistanceFromOriginBG - 1.0f) / halfSquare) * 0.5f,
            _lerpWeight);
        break;
    }
    }
}

void Tile::SetVisualScale()
{
    orxVECTOR leftEdgeMidpoint = {
        (m_leftEdgeTopPoint.fX + m_leftEdgeBottomPoint.fX) / 2.0f,
        (m_leftEdgeTopPoint.fY + m_leftEdgeBottomPoint.fY) / 2.0f };
    orxVECTOR rightEdgeMidpoint = {
        (m_rightEdgeTopPoint.fX + m_rightEdgeBottomPoint.fX) / 2.0f,
        (m_rightEdgeTopPoint.fY + m_rightEdgeBottomPoint.fY) / 2.0f };
    m_visualScale = {
        orxVector_GetDistance(&leftEdgeMidpoint, &rightEdgeMidpoint),
        orxVector_GetDistance(&m_leftEdgeBottomPoint, &m_leftEdgeTopPoint) };
}
