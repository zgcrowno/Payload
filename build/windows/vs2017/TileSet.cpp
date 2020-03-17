#include "TileSet.h"
#include <iostream>

using namespace payload;

void TileSet::OnCreate()
{
    m_2D = GetBool("2D", GetModelName());
    m_cartesian = GetBool("Cartesian", GetModelName());
    m_square = GetU32("Square", GetModelName());
    m_halfSquare = m_square / 2;
    m_shiftLerpT = GetFloat("ShiftLerpT", GetModelName());
    orxVECTOR parentCameraScale = GetVector("Scale", GetModelName());
    m_width = GetScaledSize().fX;
    m_height = GetScaledSize().fY;
    m_radius = m_width / 2.0f;
    m_payloadOrigin = GetVector("PayloadOrigin", GetModelName());
    m_goalOrigin = GetVector("GoalOrigin", GetModelName());
    m_payload = static_cast<PlayerPayload*>(GetChildByName("O-PlayerPayload"));
    m_goal = static_cast<Goal*>(GetChildByName("O-Goal"));

    orxVECTOR pos = GetPosition();
    orxVECTOR scale = GetScale();
    for (int i = 0; i < m_square; i++)
    {
        m_tileRows.push_back({});
        for (int j = 0; j < m_square; j++)
        {
            // How many tiles away from the TileSet's pivot is this Tile?
            int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(i, j);
            // How many tiles are in this Tile's circular, polar-2D row?
            int tilesInPolarRow = GetNumTilesInPolarRow(unitDistanceFromOrigin);
            // How many tiles away from the polar axis is this Tile?
            int unitDistanceFromPolarAxis = GetUnitDistanceFromPolarAxis(i, j, unitDistanceFromOrigin);
            // How far away from the TileSet's pivot is this Tile's visual center?
            float radialDistance = GetRadialDistance(unitDistanceFromOrigin);
            // At what angle is this Tile, in reference to the TileSet's pivot?
            float theta = GetPolarTheta(unitDistanceFromPolarAxis, tilesInPolarRow);
            // Create the Tile at this row/column pair.
            Tile *tile = ScrollCast<Tile*>(CreateObject("O-Tile"));
            // Get the config size of the Tile (in pixels).
            orxVECTOR tileSize = tile->GetSize();
            // Ensure that the Tile is owned by the TileSet.
            tile->SetOwner(this);
            // Ensure that the Tile has the same world position as the TileSet.
            tile->SetPolarPosition(pos, 0, 0);
            // Ensure that the Tile's scale results in it taking up the exact same screen space as the TileSet.
            tile->SetScale({ scale.fX / tileSize.fX, scale.fY / tileSize.fY });

            // Perform initial setup of Tile.
            if (Cartesian2D())
            {
                tile->m_bCartesian = m_cartesian;
                tile->m_leftEdgeTopPoint = {
                    (float)j / m_square,
                    (float)i / m_square };
                tile->m_leftEdgeBottomPoint = {
                    (float)j / m_square,
                    (i + 1.0f) / m_square };
                tile->m_rightEdgeTopPoint = {
                    (j + 1.0f) / m_square,
                    (float)i / m_square };
                tile->m_rightEdgeBottomPoint = {
                    (j + 1.0f) / m_square,
                    (i + 1.0f) / m_square };
                tile->m_topRadius = ((float)unitDistanceFromOrigin / m_halfSquare) * 0.5f;
                tile->m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / m_halfSquare) * 0.5f;
                tile->m_visualCenter = {
                    pos.fX - m_radius + (m_radius / m_square) + (j * (m_radius / m_halfSquare)),
                    pos.fY - m_radius + (m_radius / m_square) + (i * (m_radius / m_halfSquare)) };
                tile->SetVisualScale();
            }
            else if (Cartesian1D())
            {

            }
            else if (Polar2D())
            {
                tile->m_bCartesian = m_cartesian;
                SetPolarPosition(tile->m_leftEdgeTopPoint,
                    { 0.5f, 0.5f },
                    ((radialDistance / m_radius) + ((m_radius / m_square) / m_radius)) / 2.0f,
                    theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
                SetPolarPosition(tile->m_leftEdgeBottomPoint,
                    { 0.5f, 0.5f },
                    ((radialDistance / m_radius) - ((m_radius / m_square) / m_radius)) / 2.0f,
                    theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
                SetPolarPosition(tile->m_rightEdgeTopPoint,
                    { 0.5f, 0.5f },
                    ((radialDistance / m_radius) + ((m_radius / m_square) / m_radius)) / 2.0f,
                    theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
                SetPolarPosition(tile->m_rightEdgeBottomPoint,
                    { 0.5f, 0.5f },
                    ((radialDistance / m_radius) - ((m_radius / m_square) / m_radius)) / 2.0f,
                    theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
                tile->m_topRadius = ((float)unitDistanceFromOrigin / m_halfSquare) * 0.5f;
                tile->m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / m_halfSquare) * 0.5f;
                SetPolarPosition(tile->m_visualCenter, pos, radialDistance, theta);
                tile->SetVisualScale();
            }
            else // Polar1D()
            {
                tile->m_bCartesian = m_cartesian;
                SetPolarPosition(tile->m_leftEdgeTopPoint,
                    { 0.5f, 0.5f },
                    ((radialDistance / m_radius) + ((m_radius / (m_square * 2.0f)) / m_radius)) / 2.0f,
                    theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
                SetPolarPosition(tile->m_leftEdgeBottomPoint,
                    { 0.5f, 0.5f },
                    ((radialDistance / m_radius) - ((m_radius / (m_square * 2.0f)) / m_radius)) / 2.0f,
                    theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
                SetPolarPosition(tile->m_rightEdgeTopPoint,
                    { 0.5f, 0.5f },
                    ((radialDistance / m_radius) + ((m_radius / (m_square * 2.0f)) / m_radius)) / 2.0f,
                    theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
                SetPolarPosition(tile->m_rightEdgeBottomPoint,
                    { 0.5f, 0.5f },
                    ((radialDistance / m_radius) - ((m_radius / (m_square * 2.0f)) / m_radius)) / 2.0f,
                    theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)));
                tile->m_topRadius = ((float)unitDistanceFromOrigin / m_square) * 0.5f;
                tile->m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / m_square) * 0.5f;
                SetPolarPosition(tile->m_visualCenter, pos, radialDistance, theta);
                tile->SetVisualScale();
            }
            m_tileRows.at(i).push_back(tile);
        }
    }
    // Set m_payload's and m_goal's default targets and positions.
    m_payload->m_target = m_tileRows.at(m_payloadOrigin.fX).at(m_payloadOrigin.fY);
    m_payload->SetPosition(m_payload->m_target->m_visualCenter);
    m_goal->m_target = m_tileRows.at(m_goalOrigin.fX).at(m_goalOrigin.fY);
    m_goal->SetPosition(m_goal->m_target->m_visualCenter);
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
    // HANDLE INPUTS
    if (orxInput_HasBeenActivated("DimShift"))
    {
        if (m_2D)
        {
            Shift(D1);
        }
        else // !m_2D
        {
            Shift(D2);
        }
    }
    else if (orxInput_HasBeenActivated("CoShift"))
    {
        if (m_cartesian)
        {
            Shift(Polar);
        }
        else // !m_cartesian
        {
            Shift(Cartesian);
        }
    }
    else if (orxInput_HasBeenActivated("MemRight"))
    {

    }
    else if (orxInput_HasBeenActivated("MemUp"))
    {

    }
    else if (orxInput_HasBeenActivated("MemDown"))
    {

    }
    else if (orxInput_HasBeenActivated("MemLeft"))
    {

    }
    else if (orxInput_HasBeenActivated("Recon"))
    {

    }
    else if (orxInput_HasBeenActivated("Undo"))
    {

    }
    // HANDLE SHIFTING
    if (m_shiftStatus != None)
    {
        orxVECTOR pos = GetPosition();
        for (int i = 0; i < m_square; i++)
        {
            for (int j = 0; j < m_square; j++)
            {
                // How many tiles away from the TileSet's pivot is this Tile?
                int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(i, j);
                // How many tiles are in this Tile's circular, polar-2D row?
                int tilesInPolarRow = GetNumTilesInPolarRow(unitDistanceFromOrigin);
                // How many tiles away from the polar axis is this Tile?
                int unitDistanceFromPolarAxis = GetUnitDistanceFromPolarAxis(i, j, unitDistanceFromOrigin);
                // How far away from the TileSet's pivot is this Tile's visual center?
                float radialDistance = GetRadialDistance(unitDistanceFromOrigin);
                // At what angle is this Tile, in reference to the TileSet's pivot?
                float theta = GetPolarTheta(unitDistanceFromPolarAxis, tilesInPolarRow);
                // Grab the Tile at this row/column pair.
                Tile *tile = m_tileRows.at(i).at(j);

                // Shift tile via various interpolations.
                if (Cartesian2D())
                {
                    tile->m_bCartesian = m_cartesian;
                    orxVECTOR leftEdgeTopPointDest = { (float)j / m_square, (float)i / m_square };
                    orxVector_Lerp(&tile->m_leftEdgeTopPoint, &tile->m_leftEdgeTopPoint, &leftEdgeTopPointDest, m_shiftLerpT);
                    orxVECTOR leftEdgeBottomPointDest = { (float)j / m_square, (i + 1.0f) / m_square };
                    orxVector_Lerp(&tile->m_leftEdgeBottomPoint, &tile->m_leftEdgeBottomPoint, &leftEdgeBottomPointDest, m_shiftLerpT);
                    orxVECTOR rightEdgeTopPointDest = { (j + 1.0f) / m_square, (float)i / m_square };
                    orxVector_Lerp(&tile->m_rightEdgeTopPoint, &tile->m_rightEdgeTopPoint, &rightEdgeTopPointDest, m_shiftLerpT);
                    orxVECTOR rightEdgeBottomPointDest = { (j + 1.0f) / m_square, (i + 1.0f) / m_square };
                    orxVector_Lerp(&tile->m_rightEdgeBottomPoint, &tile->m_rightEdgeBottomPoint, &rightEdgeBottomPointDest, m_shiftLerpT);
                    tile->m_topRadius = orxLERP(tile->m_topRadius, ((float)unitDistanceFromOrigin / m_halfSquare) * 0.5f, m_shiftLerpT);
                    tile->m_bottomRadius = orxLERP(tile->m_bottomRadius, ((unitDistanceFromOrigin - 1.0f) / m_halfSquare) * 0.5f, m_shiftLerpT);
                    orxVECTOR visualCenterDest = {
                        pos.fX - m_radius + (m_radius / m_square) + (j * (m_radius / m_halfSquare)),
                        pos.fY - m_radius + (m_radius / m_square) + (i * (m_radius / m_halfSquare)) };
                    orxVector_Lerp(&tile->m_visualCenter, &tile->m_visualCenter, &visualCenterDest, m_shiftLerpT);
                    tile->SetVisualScale();
                }
                else if (Cartesian1D())
                {

                }
                else if (Polar2D())
                {
                    orxVECTOR tileCenter = { 0.5f, 0.5f };
                    tile->m_bCartesian = m_cartesian;
                    float leftEdgeTopPointRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_leftEdgeTopPoint, &tileCenter),
                        ((radialDistance / m_radius) + ((m_radius / m_square) / m_radius)) / 2.0f,
                        m_shiftLerpT);
                    float leftEdgeTopPointTheta = LerpAngle(
                        CartesianToPolar(tile->m_leftEdgeTopPoint, tileCenter).fY,
                        theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_leftEdgeTopPoint,
                        tileCenter,
                        leftEdgeTopPointRadialDistance,
                        leftEdgeTopPointTheta);
                    float leftEdgeBottomPointRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_leftEdgeBottomPoint, &tileCenter),
                        ((radialDistance / m_radius) - ((m_radius / m_square) / m_radius)) / 2.0f,
                        m_shiftLerpT);
                    float leftEdgeBottomPointTheta = LerpAngle(
                        CartesianToPolar(tile->m_leftEdgeBottomPoint, tileCenter).fY,
                        theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_leftEdgeBottomPoint,
                        tileCenter,
                        leftEdgeBottomPointRadialDistance,
                        leftEdgeBottomPointTheta);
                    float rightEdgeTopPointRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_rightEdgeTopPoint, &tileCenter),
                        ((radialDistance / m_radius) + ((m_radius / m_square) / m_radius)) / 2.0f,
                        m_shiftLerpT);
                    float rightEdgeTopPointTheta = LerpAngle(
                        CartesianToPolar(tile->m_rightEdgeTopPoint, tileCenter).fY,
                        theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_rightEdgeTopPoint,
                        tileCenter,
                        rightEdgeTopPointRadialDistance,
                        rightEdgeTopPointTheta);
                    float rightEdgeBottomPointRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_rightEdgeBottomPoint, &tileCenter),
                        ((radialDistance / m_radius) - ((m_radius / m_square) / m_radius)) / 2.0f,
                        m_shiftLerpT);
                    float rightEdgeBottomPointTheta = LerpAngle(
                        CartesianToPolar(tile->m_rightEdgeBottomPoint, tileCenter).fY,
                        theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_rightEdgeBottomPoint,
                        tileCenter,
                        rightEdgeBottomPointRadialDistance,
                        rightEdgeBottomPointTheta);
                    tile->m_topRadius = orxLERP(
                        tile->m_topRadius,
                        ((float)unitDistanceFromOrigin / m_halfSquare) * 0.5f,
                        m_shiftLerpT);
                    tile->m_bottomRadius = orxLERP(
                        tile->m_bottomRadius,
                        ((unitDistanceFromOrigin - 1.0f) / m_halfSquare) * 0.5f,
                        m_shiftLerpT);
                    float visualCenterRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_visualCenter, &pos),
                        radialDistance,
                        m_shiftLerpT);
                    float visualCenterTheta = LerpAngle(
                        CartesianToPolar(tile->m_visualCenter, pos).fY,
                        theta,
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_visualCenter,
                        pos,
                        visualCenterRadialDistance,
                        visualCenterTheta);
                    tile->SetVisualScale();
                }
                else // Polar1D()
                {
                    orxVECTOR tileCenter = { 0.5f, 0.5f };
                    tile->m_bCartesian = m_cartesian;
                    float leftEdgeTopPointRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_leftEdgeTopPoint, &tileCenter),
                        ((radialDistance / m_radius) + ((m_radius / (m_square * 2.0f)) / m_radius)) / 2.0f,
                        m_shiftLerpT);
                    float leftEdgeTopPointTheta = LerpAngle(
                        CartesianToPolar(tile->m_leftEdgeTopPoint, tileCenter).fY,
                        theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_leftEdgeTopPoint,
                        tileCenter,
                        leftEdgeTopPointRadialDistance,
                        leftEdgeTopPointTheta);
                    float leftEdgeBottomPointRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_leftEdgeBottomPoint, &tileCenter),
                        ((radialDistance / m_radius) - ((m_radius / (m_square * 2.0f)) / m_radius)) / 2.0f,
                        m_shiftLerpT);
                    float leftEdgeBottomPointTheta = LerpAngle(
                        CartesianToPolar(tile->m_leftEdgeBottomPoint, tileCenter).fY,
                        theta + (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_leftEdgeBottomPoint,
                        tileCenter,
                        leftEdgeBottomPointRadialDistance,
                        leftEdgeBottomPointTheta);
                    float rightEdgeTopPointRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_rightEdgeTopPoint, &tileCenter),
                        ((radialDistance / m_radius) + ((m_radius / (m_square * 2.0f)) / m_radius)) / 2.0f,
                        m_shiftLerpT);
                    float rightEdgeTopPointTheta = LerpAngle(
                        CartesianToPolar(tile->m_rightEdgeTopPoint, tileCenter).fY,
                        theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_rightEdgeTopPoint,
                        tileCenter,
                        rightEdgeTopPointRadialDistance,
                        rightEdgeTopPointTheta);
                    float rightEdgeBottomPointRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_rightEdgeBottomPoint, &tileCenter),
                        ((radialDistance / m_radius) - ((m_radius / (m_square * 2.0f)) / m_radius)) / 2.0f,
                        m_shiftLerpT);
                    float rightEdgeBottomPointTheta = LerpAngle(
                        CartesianToPolar(tile->m_rightEdgeBottomPoint, tileCenter).fY,
                        theta - (orxMATH_KF_2_PI / (2 * tilesInPolarRow)),
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_rightEdgeBottomPoint,
                        tileCenter,
                        rightEdgeBottomPointRadialDistance,
                        rightEdgeBottomPointTheta);
                    tile->m_topRadius = orxLERP(
                        tile->m_topRadius,
                        ((float)unitDistanceFromOrigin / m_square) * 0.5f,
                        m_shiftLerpT);
                    tile->m_bottomRadius = orxLERP(
                        tile->m_bottomRadius,
                        ((unitDistanceFromOrigin - 1.0f) / m_square) * 0.5f,
                        m_shiftLerpT);
                    float visualCenterRadialDistance = orxLERP(
                        orxVector_GetDistance(&tile->m_visualCenter, &pos),
                        radialDistance,
                        m_shiftLerpT);
                    float visualCenterTheta = LerpAngle(
                        CartesianToPolar(tile->m_visualCenter, pos).fY,
                        theta,
                        m_shiftLerpT);
                    SetPolarPosition(tile->m_visualCenter,
                        pos,
                        visualCenterRadialDistance,
                        visualCenterTheta);
                    tile->SetVisualScale();
                }
            }
        }
    }
}

void TileSet::Shift(ShiftStatus _shiftStatus)
{
    m_shiftStatus = _shiftStatus;

    switch (_shiftStatus)
    {
    case D1:
        m_2D = false;
        break;
    case D2:
        m_2D = true;
        break;
    case Cartesian:
        m_cartesian = true;
        break;
    case Polar:
        m_cartesian = false;
        break;
    }
}

const bool TileSet::Cartesian2D()
{
    return m_cartesian && m_2D;
}

const bool TileSet::Cartesian1D()
{
    return m_cartesian && !m_2D;
}

const bool TileSet::Polar2D()
{
    return !m_cartesian && m_2D;
}

const bool TileSet::Polar1D()
{
    return !m_cartesian && !m_2D;
}

const int TileSet::GetQuadrant(const int &_row, const int &_col)
{
    // In what Cartesian quadrant is this Tile?
    return _col >= m_halfSquare && _row < m_halfSquare ? 1 :
        _col < m_halfSquare && _row < m_halfSquare ? 2 :
        _col < m_halfSquare && _row >= m_halfSquare ? 3 :
        4;
}

const int TileSet::GetUnitDistanceFromOrigin(const int &_row, const int &_col)
{
    // How many tiles away from the TileSet's pivot is this Tile?
    int unitDistanceFromOrigin;

    if (Cartesian1D())
    {
        unitDistanceFromOrigin = _col < m_halfSquare ? m_halfSquare - _col : (_col + 1) - m_halfSquare;
    }
    else if (Polar1D())
    {
        unitDistanceFromOrigin = m_square - _row;
    }
    else // Cartesian2D() || Polar2D()
    {
        // The imaginary value of an x or y point between halfNumColumns and halfNumColumns + 1 (used for determining unit distance from center of TileSet).
        float imaginaryPivotValue = m_halfSquare + 0.5f;
        // How many tiles away from the TileSet's pivot (on the X-axis) is this Tile?
        int unitDistanceFromPivotX = abs(imaginaryPivotValue - (_col + 1)) + 1;
        // How many tiles away from the TileSet's pivot (on the Y-axis) is this Tile?
        int unitDistanceFromPivotY = abs(imaginaryPivotValue - (_row + 1)) + 1;
        unitDistanceFromOrigin = orxMAX(unitDistanceFromPivotX, unitDistanceFromPivotY);
    }

    return unitDistanceFromOrigin;
}

const int TileSet::GetUnitDistanceFromPolarAxis(const int &_row, const int &_col, const int &_unitDistanceFromOrigin)
{
    // How many tiles away from the polar axis is this Tile?
    int unitDistanceFromPolarAxis;

    if (m_2D)
    {
        // In what Cartesian quadrant is this Tile?
        const int quadrant = GetQuadrant(_row, _col);

        unitDistanceFromPolarAxis = ((quadrant - 1) * (2 * _unitDistanceFromOrigin - 1));

        switch (quadrant)
        {
        case 1:
            unitDistanceFromPolarAxis += (m_halfSquare - _row) + (m_halfSquare + _unitDistanceFromOrigin - _col) - 1;
            break;
        case 2:
            unitDistanceFromPolarAxis += (m_halfSquare - _col) + ((_row + 1) - (m_halfSquare - _unitDistanceFromOrigin)) - 1;
            break;
        case 3:
            unitDistanceFromPolarAxis += ((_row + 1) - m_halfSquare) + ((_col + 1) - (m_halfSquare - _unitDistanceFromOrigin)) - 1;
            break;
        case 4:
            unitDistanceFromPolarAxis += ((_col + 1) - m_halfSquare) + (m_halfSquare + _unitDistanceFromOrigin - _row) - 1;
            break;
        }
    }
    else
    {
        unitDistanceFromPolarAxis = ((3 * m_square) / 4) - _col;
        if (unitDistanceFromPolarAxis <= 0)
        {
            unitDistanceFromPolarAxis += m_square;
        }
        if (unitDistanceFromPolarAxis == m_square && m_halfSquare % 2 != 0)
        {
            unitDistanceFromPolarAxis = 0;
        }
    }

    return unitDistanceFromPolarAxis;
}

const int TileSet::GetNumTilesInPolarRow(const int &_unitDistanceFromOrigin)
{
    // How many tiles are in this Tile's circular, polar-2D row?
    int numTilesInPolarRow;

    if (m_2D)
    {
        numTilesInPolarRow = (_unitDistanceFromOrigin * 4) + ((_unitDistanceFromOrigin - 1) * 4);
    }
    else
    {
        numTilesInPolarRow = m_square;
    }

    return numTilesInPolarRow;
}

const float TileSet::GetRadialDistance(const int &_unitDistanceFromOrigin)
{
    // How far away from the TileSet's pivot is this Tile's visual center?
    float radialDistance;

    if (m_2D)
    {
        radialDistance = ((1.0f / m_square) * m_radius) + ((_unitDistanceFromOrigin - 1) * ((1.0f / m_halfSquare) * m_radius));
    }
    else
    {
        radialDistance = ((1.0f / (m_square * 2.0f)) * m_radius) + ((_unitDistanceFromOrigin - 1) * ((1.0f / m_square) * m_radius));
    }

    return radialDistance;
}

const float TileSet::GetPolarTheta(const int &_unitDistanceFromPolarAxis, const int &_tilesInPolarRow)
{
    // At what angle is this Tile, in reference to the TileSet's pivot?
    float theta;

    if (m_2D)
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
            if (m_halfSquare % 2 != 0)
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

const orxVECTOR TileSet::GetPayloadRowAndColumn()
{
    for (int i = 0; i < m_square; i++)
    {
        std::vector<Tile*> tileRow = m_tileRows.at(i);
        for (int j = 0; j < m_square; j++)
        {
            if (m_payload->m_target != nullptr && m_payload->m_target == tileRow.at(j))
            {
                return { (float)i, (float)j };
            }
        }
    }
}
