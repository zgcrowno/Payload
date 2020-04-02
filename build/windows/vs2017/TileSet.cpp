#include "TileSet.h"
#include <iostream>

using namespace payload;

void TileSet::OnCreate()
{
    m_state = static_cast<TileSetState>(GetU32("State", GetModelName()));
    m_square = GetU32("Square", GetModelName());
    m_halfSquare = m_square / 2;
    m_timeToShift = GetFloat("TimeToShift", GetModelName());
    m_timeSpentShifting = m_timeToShift;
    orxVECTOR parentCameraScale = GetVector("Scale", GetModelName());
    int numBorders = m_square + 1;
    float textureThickness = GetSize().fX;
    m_normalizedTileSize = ((NATIVE_TEXTURE_SIZE - (numBorders * NATIVE_BORDER_SIZE)) / m_square) / NATIVE_TEXTURE_SIZE;
    m_width = GetScaledSize().fX;
    m_height = GetScaledSize().fY;
    m_radius = m_width / 2.0f;
    m_payloadOrigin = GetVector("PayloadOrigin", GetModelName());
    m_goalOrigin = GetVector("GoalOrigin", GetModelName());
    m_payload = static_cast<PlayerPayload*>(GetChildByName("O-PlayerPayload"));
    m_goal = static_cast<Goal*>(GetChildByName("O-Goal"));

    // TileSet position and scale;
    orxVECTOR pos = GetPosition();
    orxVECTOR scale = GetScale();
    //Create Tile rows.
    for (int i = 0; i < m_square; i++)
    {
        // Set up the row.
        m_tileRows.push_back({});
        for (int j = 0; j < m_square; j++)
        {
            // Create the Tile at this row/column pair.
            Tile *tile = ScrollCast<Tile*>(CreateObject("O-Tile"));
            // Get the config size of the Tile (in pixels).
            orxVECTOR tileSize = tile->GetSize();
            // Ensure that the Tile is owned by the TileSet.
            tile->SetParent(this);
            // Ensure that the Tile has the same world position as the TileSet.
            tile->SetPolarPosition(pos, 0, 0);
            // Ensure that the Tile's scale results in it taking up the exact same screen space as the TileSet.
            tile->SetScale({ scale.fX / tileSize.fX, scale.fY / tileSize.fY });
            // Add the Tile.
            m_tileRows.at(i).push_back(tile);
        }
    }
    // Set m_payload's and m_goal's default targets.
    m_payload->m_target = m_tileRows.at(m_payloadOrigin.fX).at(m_payloadOrigin.fY);
    m_goal->m_target = m_tileRows.at(m_goalOrigin.fX).at(m_goalOrigin.fY);
    // The payload's row and column.
    orxVECTOR payloadRowAndCol = GetPayloadRowAndColumn();
    int payloadRow = payloadRowAndCol.fX;
    int payloadCol = payloadRowAndCol.fY;
    // Perform initial setup of Tiles.
    for (int i = 0; i < m_square; i++)
    {
        for (int j = 0; j < m_square; j++)
        {
            // The Tile at this row/column pair.
            Tile *tile = m_tileRows.at(i).at(j);
            // Perform initial setup of Tile.
            tile->SetUp(i, j, m_square, m_radius, m_normalizedTileSize, NORMALIZED_BORDER_SIZE, payloadRowAndCol, pos, GetScaledSize(), m_state);
            // Add the tile to m_tileRows.
            m_tileRows.at(i).push_back(tile);
        }
    }
    // Set m_payload's and m_goal's default positions, tileSetCenters and priorPositions.
    m_payload->SetPosition(m_payload->m_target->GetPosition());
    m_payload->m_priorPos = m_payload->GetPosition();
    m_payload->m_tileSetPos = GetPosition();
    m_goal->SetPosition(m_goal->m_target->GetPosition());
    m_goal->m_priorPos = m_goal->GetPosition();
    m_goal->m_tileSetPos = GetPosition();
    /*m_payload->Enable(false);
    m_goal->Enable(false);*/
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

orxBOOL TileSet::OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload)
{
    if (!orxString_Compare(_rstPayload.zParamName, "Square"))
    {
        _rstPayload.fValue = m_square;
    }
    if (!orxString_Compare(_rstPayload.zParamName, "PriorState"))
    {
        _rstPayload.fValue = static_cast<float>(m_priorState);
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "ShiftStatus"))
    {
        _rstPayload.fValue = static_cast<float>(m_shiftStatus);
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "D2"))
    {
        _rstPayload.fValue = Is2D();
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "Cartesian"))
    {
        _rstPayload.fValue = IsCartesian();
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "Square"))
    {
        _rstPayload.fValue = m_square;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "TimeSpentShifting"))
    {
        _rstPayload.fValue = m_timeSpentShifting;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "PayloadPosition"))
    {
        _rstPayload.vValue = GetNormalizedPosition(m_payload->GetPosition());
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "PriorPayloadPosition"))
    {
        _rstPayload.vValue = GetNormalizedPosition(m_payload->m_priorPos);
    }

    return orxTRUE;
}

void TileSet::Update(const orxCLOCK_INFO &_rstInfo)
{
    // HANDLE INPUTS
    if (!m_payload->m_bIsMoving && m_shiftStatus == TileSetShiftStatus::None)
    {
        if (orxInput_HasBeenActivated("DimShift"))
        {
            if (Is2D())
            {
                Shift(TileSetShiftStatus::D1);
            }
            else // One-dimensional.
            {
                Shift(TileSetShiftStatus::D2);
            }
        }
        else if (orxInput_HasBeenActivated("CoShift"))
        {
            if (IsCartesian())
            {
                Shift(TileSetShiftStatus::Polar);
            }
            else // Polar.
            {
                Shift(TileSetShiftStatus::Cartesian);
            }
        }
        else if (orxInput_HasBeenActivated("MoveRight"))
        {
            // The payload's row and column.
            orxVECTOR payloadRowAndCol = GetPayloadRowAndColumn();
            int payloadRow = payloadRowAndCol.fX;
            int payloadCol = payloadRowAndCol.fY;
            Tile *tileToRight = GetTileToRight(payloadRow, payloadCol, payloadRowAndCol);
            if (tileToRight != nullptr)
            {
                m_payload->SetTarget(tileToRight);
            }
        }
        else if (orxInput_HasBeenActivated("MoveUp"))
        {
            // The payload's row and column.
            orxVECTOR payloadRowAndCol = GetPayloadRowAndColumn();
            int payloadRow = payloadRowAndCol.fX;
            int payloadCol = payloadRowAndCol.fY;
            Tile *tileAbove = GetTileAbove(payloadRow, payloadCol, payloadRowAndCol);
            if (tileAbove != nullptr)
            {
                m_payload->SetTarget(tileAbove);
            }
        }
        else if (orxInput_HasBeenActivated("MoveDown"))
        {
            // The payload's row and column.
            orxVECTOR payloadRowAndCol = GetPayloadRowAndColumn();
            int payloadRow = payloadRowAndCol.fX;
            int payloadCol = payloadRowAndCol.fY;
            Tile *tileBelow = GetTileBelow(payloadRow, payloadCol, payloadRowAndCol);
            if (tileBelow != nullptr)
            {
                m_payload->SetTarget(tileBelow);
            }
        }
        else if (orxInput_HasBeenActivated("MoveLeft"))
        {
            // The payload's row and column.
            orxVECTOR payloadRowAndCol = GetPayloadRowAndColumn();
            int payloadRow = payloadRowAndCol.fX;
            int payloadCol = payloadRowAndCol.fY;
            Tile *tileToLeft = GetTileToLeft(payloadRow, payloadCol, payloadRowAndCol);
            if (tileToLeft != nullptr)
            {
                m_payload->SetTarget(tileToLeft);
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
    }
    // HANDLE SHIFTING
    if (m_shiftStatus != TileSetShiftStatus::None)
    {
        // The payload's row and column.
        orxVECTOR payloadRowAndCol = GetPayloadRowAndColumn();
        // The TileSet's position.
        orxVECTOR pos = GetPosition();
        // Increment the time spent shifting.
        m_timeSpentShifting += _rstInfo.fDT;
        float lerpWeight = m_timeSpentShifting / m_timeToShift;

        // Only shift if lerp hasn't already finished.
        if (m_timeSpentShifting <= m_timeToShift)
        {
            // Shift each tile individually.
            for (int i = 0; i < m_square; i++)
            {
                for (int j = 0; j < m_square; j++)
                {
                    // Grab the Tile at this row/column pair.
                    Tile *tile = m_tileRows.at(i).at(j);
                    // Shift the Tile.
                    tile->Shift(i, j, m_square, m_radius, m_normalizedTileSize, NORMALIZED_BORDER_SIZE, lerpWeight, payloadRowAndCol, pos, m_state, m_shiftStatus);
                }
            }
            // Ensure that while shifting is occurring, all TileInhabitants are bound to their respective targets.
            for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
            {
                TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
                ti->SetPosition(ti->m_target->GetPosition());
            }
        }
        else if (!Is2D() && (m_priorState != TileSetState::Cartesian1D && m_priorState != TileSetState::Polar1D) && m_shiftStatus != TileSetShiftStatus::D1Tiles)
        {
            FinalizeTileAndInhabitantLerps();

            Shift(TileSetShiftStatus::D1Tiles);
        }
        else
        {
            FinalizeTileAndInhabitantLerps();

            Shift(TileSetShiftStatus::None);
        }
    }
}

void TileSet::Shift(TileSetShiftStatus _shiftStatus)
{
    m_priorState = m_state;
    m_shiftStatus = _shiftStatus;

    switch (_shiftStatus)
    {
    case TileSetShiftStatus::D1:
        m_timeSpentShifting = 0.0f;
        m_state = IsCartesian() ? TileSetState::Cartesian1D : TileSetState::Polar1D;
        break;
    case TileSetShiftStatus::D2:
        m_timeSpentShifting = 0.0f;
        m_state = IsCartesian() ? TileSetState::Cartesian2D : TileSetState::Polar2D;
        break;
    case TileSetShiftStatus::Cartesian:
        m_timeSpentShifting = 0.0f;
        m_state = Is2D() ? TileSetState::Cartesian2D : TileSetState::Cartesian1D;
        break;
    case TileSetShiftStatus::Polar:
        m_timeSpentShifting = 0.0f;
        m_state = Is2D() ? TileSetState::Polar2D : TileSetState::Polar1D;
        break;
    case TileSetShiftStatus::D1Tiles:
        m_timeSpentShifting = 0.0f;
        break;
    }
}

void TileSet::FinalizeTileAndInhabitantLerps()
{
    for (int i = 0; i < m_square; i++)
    {
        for (int j = 0; j < m_square; j++)
        {
            // Grab the Tile at this row/column pair.
            Tile *tile = m_tileRows.at(i).at(j);
            // Ensure Tile completes its lerp such that its position is exactly the same as its target's.
            tile->SetParentSpacePosition(tile->m_targetParentSpacePos);
            // Set the Tile's m_priorParentSpacePos for future lerping Tile-side.
            tile->m_priorParentSpacePos = tile->GetParentSpacePosition();
            // Set the Tile's m_priorTileSetState for future lerping Tile-side.
            tile->m_priorTileSetState = m_state;
            // Ensure Tile completes its lerp such that its visual scale is exactly the same as its target's.
            tile->m_visualScale = tile->m_targetVisualScale;
            // Set The Tile's m_priorVisualScale for future lerping Tile-side.
            tile->m_priorVisualScale = tile->m_visualScale;
        }
    }

    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        // Ensure TI completes its lerp such that its position is exactly the same as its target's.
        ti->SetPosition(ti->m_target->GetPosition());
        // Set the TileInhabitant's m_priorPos for future lerping TI-side.
        ti->m_priorPos = ti->GetPosition();
    }
}

const bool TileSet::Is2D()
{
    return m_state == TileSetState::Cartesian2D || m_state == TileSetState::Polar2D;
}

const bool TileSet::IsCartesian()
{
    return m_state == TileSetState::Cartesian1D || m_state == TileSetState::Cartesian2D;
}

const int TileSet::GetQuadrant(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol, const bool _background)
{
    // In what Cartesian quadrant is this Tile?
    int quadrant;

    if (_background)
    {
        quadrant = _col >= m_halfSquare && _row < m_halfSquare ? 1 :
            _col < m_halfSquare && _row < m_halfSquare ? 2 :
            _col < m_halfSquare && _row >= m_halfSquare ? 3 :
            4;
    }
    else
    {
        int row = _payloadRowAndCol.fX;
        quadrant = _col >= m_halfSquare && row < m_halfSquare ? 1 :
            _col < m_halfSquare && row < m_halfSquare ? 2 :
            _col < m_halfSquare && row >= m_halfSquare ? 3 :
            4;
    }

    return quadrant;
}

const int TileSet::GetUnitDistanceFromOrigin(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol, const bool _background)
{
    // How many tiles away from the TileSet's pivot is this Tile?
    int unitDistanceFromOrigin;

    switch (m_state)
    {
    case TileSetState::Cartesian1D:
        unitDistanceFromOrigin = _col < m_halfSquare ? m_halfSquare - _col : (_col + 1) - m_halfSquare;
        break;
    case TileSetState::Polar1D:
        if (_background)
        {
            unitDistanceFromOrigin = m_square - _row;
        }
        else
        {
            int row = _payloadRowAndCol.fX;
            unitDistanceFromOrigin = m_square - row;
        }
        break;
    default:
        // How many tiles away from the TileSet's pivot (on the X-axis) is this Tile?
        int unitDistanceFromPivotX = _col < m_halfSquare ? m_halfSquare - _col : (_col + 1) - m_halfSquare;
        // How many tiles away from the TileSet's pivot (on the Y-axis) is this Tile?
        int unitDistanceFromPivotY = _row < m_halfSquare ? m_halfSquare - _row : (_row + 1) - m_halfSquare;
        unitDistanceFromOrigin = orxMAX(unitDistanceFromPivotX, unitDistanceFromPivotY);
        break;
    }

    return unitDistanceFromOrigin;
}

const int TileSet::GetUnitDistanceFromPolarAxis(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol, const int &_unitDistanceFromOrigin)
{
    // How many tiles away from the polar axis is this Tile?
    int unitDistanceFromPolarAxis;

    if (Is2D())
    {
        // In what Cartesian quadrant is this Tile?
        const int quadrant = GetQuadrant(_row, _col, _payloadRowAndCol, true);

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

    if (Is2D())
    {
        numTilesInPolarRow = (_unitDistanceFromOrigin * 4) + ((_unitDistanceFromOrigin - 1) * 4);
    }
    else
    {
        numTilesInPolarRow = m_square;
    }

    return numTilesInPolarRow;
}

const float TileSet::GetRadialDistance(const int &_unitDistanceFromOrigin, const orxVECTOR &_payloadRowAndCol, const bool _background)
{
    // How far away from the TileSet's pivot is this Tile's visual center?
    float radialDistance;

    if (Is2D())
    {
        radialDistance = ((1.0f / m_square) * m_radius) + ((_unitDistanceFromOrigin - 1) * ((1.0f / m_halfSquare) * m_radius));
    }
    else
    {
        if (_background)
        {
            radialDistance = ((1.0f / (m_square * 2.0f)) * m_radius) + ((_unitDistanceFromOrigin - 1) * ((1.0f / m_square) * m_radius));
        }
        else
        {
            int payloadUnitDistanceFromOrigin = GetUnitDistanceFromOrigin(_payloadRowAndCol.fX, _payloadRowAndCol.fY, _payloadRowAndCol, _background);
            radialDistance = ((1.0f / (m_square * 2.0f)) * m_radius) + ((payloadUnitDistanceFromOrigin - 1) * ((1.0f / m_square) * m_radius));
        }
    }

    return radialDistance;
}

const float TileSet::GetPolarTheta(const int &_unitDistanceFromPolarAxis, const int &_tilesInPolarRow)
{
    // At what angle is this Tile, in reference to the TileSet's pivot?
    float theta;

    if (Is2D())
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

const orxVECTOR TileSet::GetCartesianUnitDistancesFromOrigin(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol, const bool _background)
{
    // How many tiles away from the TileSet's pivot (along both the X and Y-axes) is this Tile?
    orxVECTOR unitDistancesFromOrigin;
    // How many tiles away from the TileSet's pivot is this Tile along the X-axis.
    int unitDistanceFromOriginX = _col < m_halfSquare ? m_halfSquare - _col : (_col + 1) - m_halfSquare;

    if (_background)
    {
        unitDistancesFromOrigin = {
            (float)unitDistanceFromOriginX,
            (float)(_row < m_halfSquare ? m_halfSquare - _row : (_row + 1) - m_halfSquare) };
    }
    else
    {
        int payloadRow = _payloadRowAndCol.fX;
        unitDistancesFromOrigin = {
            (float)unitDistanceFromOriginX,
            (float)(payloadRow < m_halfSquare ? m_halfSquare - payloadRow : (payloadRow + 1) - m_halfSquare) };
    }

    return unitDistancesFromOrigin;
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

const orxVECTOR TileSet::GetNormalizedPosition(const orxVECTOR &_vec)
{
    orxVECTOR pos = GetPosition();
    orxVECTOR scaledSize = GetScaledSize();
    orxVECTOR upperLeftCorner = { pos.fX - (scaledSize.fX / 2.0f), pos.fY - (scaledSize.fY / 2.0f) };
    return { (_vec.fX - upperLeftCorner.fX) / scaledSize.fX, (_vec.fY - upperLeftCorner.fY) / scaledSize.fY };
}

Tile *TileSet::GetTileToRight(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol)
{
    switch (m_state)
    {
    case TileSetState::Cartesian1D:
    case TileSetState::Cartesian2D:
        if (_col < m_square - 1)
        {
            return m_tileRows.at(_row).at(_col + 1);
        }
        break;
    case TileSetState::Polar1D:
        if (_col < m_square - 1)
        {
            return m_tileRows.at(_row).at(_col + 1);
        }
        else
        {
            return m_tileRows.at(_row).at(0);
        }
        break;
    case TileSetState::Polar2D:
        int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(_row, _col, _payloadRowAndCol, false);
        if (_col >= m_halfSquare)
        {
            if (_col - unitDistanceFromOrigin == m_halfSquare - 1)
            {
                if (_row >= m_halfSquare)
                {
                    if (_row - unitDistanceFromOrigin == m_halfSquare - 1)
                    {
                        return m_tileRows.at(_row).at(_col - 1);
                    }
                    else
                    {
                        return m_tileRows.at(_row + 1).at(_col);
                    }
                }
                else
                {
                    return m_tileRows.at(_row + 1).at(_col);
                }
            }
            else
            {
                if (_row >= m_halfSquare)
                {
                    return m_tileRows.at(_row).at(_col - 1);
                }
                else
                {
                    return m_tileRows.at(_row).at(_col + 1);
                }
            }
        }
        else
        {
            if (_col + unitDistanceFromOrigin == m_halfSquare)
            {
                if (_row < m_halfSquare)
                {
                    if (_row + unitDistanceFromOrigin == m_halfSquare)
                    {
                        return m_tileRows.at(_row).at(_col + 1);
                    }
                    else
                    {
                        return m_tileRows.at(_row - 1).at(_col);
                    }
                }
                else
                {
                    return m_tileRows.at(_row - 1).at(_col);
                }
            }
            else
            {
                if (_row < m_halfSquare)
                {
                    return m_tileRows.at(_row).at(_col + 1);
                }
                else
                {
                    return m_tileRows.at(_row).at(_col - 1);
                }
            }
        }
        break;
    }

    return nullptr;
}

Tile *TileSet::GetTileToLeft(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol)
{
    switch (m_state)
    {
    case TileSetState::Cartesian1D:
    case TileSetState::Cartesian2D:
        if (_col > 0)
        {
            return m_tileRows.at(_row).at(_col - 1);
        }
        break;
    case TileSetState::Polar1D:
        if (_col > 0)
        {
            return m_tileRows.at(_row).at(_col - 1);
        }
        else
        {
            return m_tileRows.at(_row).at(m_square - 1);
        }
        break;
    case TileSetState::Polar2D:
        int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(_row, _col, _payloadRowAndCol, false);
        if (_col >= m_halfSquare)
        {
            if (_col - unitDistanceFromOrigin == m_halfSquare - 1)
            {
                if (_row >= m_halfSquare)
                {
                    return m_tileRows.at(_row - 1).at(_col);
                }
                else
                {
                    if (_row + unitDistanceFromOrigin == m_halfSquare)
                    {
                        return m_tileRows.at(_row).at(_col - 1);
                    }
                    else
                    {
                        return m_tileRows.at(_row - 1).at(_col);
                    }
                }
            }
            else
            {
                if (_row >= m_halfSquare)
                {
                    return m_tileRows.at(_row).at(_col + 1);
                }
                else
                {
                    return m_tileRows.at(_row).at(_col - 1);
                }
            }
        }
        else
        {
            if (_col + unitDistanceFromOrigin == m_halfSquare)
            {
                if (_row < m_halfSquare)
                {
                    return m_tileRows.at(_row + 1).at(_col);
                }
                else
                {
                    if (_row - unitDistanceFromOrigin == m_halfSquare - 1)
                    {
                        return m_tileRows.at(_row).at(_col + 1);
                    }
                    else
                    {
                        return m_tileRows.at(_row + 1).at(_col);
                    }
                }
            }
            else
            {
                if (_row < m_halfSquare)
                {
                    return m_tileRows.at(_row).at(_col - 1);
                }
                else
                {
                    return m_tileRows.at(_row).at(_col + 1);
                }
            }
        }
        break;
    }

    return nullptr;
}

Tile *TileSet::GetTileAbove(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol)
{
    switch (m_state)
    {
    case TileSetState::Cartesian2D:
        if (_row > 0)
        {
            return m_tileRows.at(_row - 1).at(_col);
        }
        break;
    case TileSetState::Polar2D:
        int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(_row, _col, _payloadRowAndCol, false);
        if (_col >= m_halfSquare)
        {
            if (_row >= m_halfSquare)
            {
                return m_tileRows.at(_row - 1).at(_col - 1);
            }
            else
            {
                return m_tileRows.at(_row + 1).at(_col - 1);
            }
        }
        else
        {
            if (_row >= m_halfSquare)
            {
                return m_tileRows.at(_row - 1).at(_col + 1);
            }
            else
            {
                return m_tileRows.at(_row + 1).at(_col + 1);
            }
        }
        break;
    }

    return nullptr;
}

Tile *TileSet::GetTileBelow(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol)
{
    switch (m_state)
    {
    case TileSetState::Cartesian2D:
        if (_row < m_square - 1)
        {
            return m_tileRows.at(_row + 1).at(_col);
        }
        break;
    case TileSetState::Polar2D:
        int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(_row, _col, _payloadRowAndCol, false);
        if (_col >= m_halfSquare)
        {
            if (_row >= m_halfSquare)
            {
                if (_row < m_square - 1 && _col < m_square - 1)
                {
                    return m_tileRows.at(_row + 1).at(_col + 1);
                }
            }
            else
            {
                if (_row > 0 && _col < m_square - 1)
                {
                    return m_tileRows.at(_row - 1).at(_col + 1);
                }
            }
        }
        else
        {
            if (_row >= m_halfSquare)
            {
                if (_row < m_square - 1 && _col > 0)
                {
                    return m_tileRows.at(_row + 1).at(_col - 1);
                }
            }
            else
            {
                if (_row > 0 && _col > 0)
                {
                    return m_tileRows.at(_row - 1).at(_col - 1);
                }
            }
        }
        break;
    }

    return nullptr;
}
