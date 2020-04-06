#include "TileSet.h"

using namespace payload;

void TileSet::OnCreate()
{
    m_state = static_cast<TileSetState>(GetU32("State", GetModelName()));
    m_square = GetU32("Square", GetModelName());
    m_halfSquare = m_square / 2;
    m_timeToShift = GetFloat("TimeToShift", GetModelName());
    m_timeSpentShifting = m_timeToShift;
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
            tile->SetUp(i, j, m_square, m_radius, m_normalizedTileSize, NORMALIZED_BORDER_SIZE, payloadRowAndCol, pos, m_state);
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
    if (InputAllowed())
    {
        if (orxInput_HasBeenActivated("DimShift"))
        {
            if (Is2D())
            {
                if (IsCartesian())
                {
                    Shift(TileSetShiftStatus::D1Tiles);
                }
                else
                {
                    Shift(TileSetShiftStatus::D1);
                }
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
        // Increment the time spent shifting.
        m_timeSpentShifting += _rstInfo.fDT;

        // Has the lerp finished?
        if (m_timeSpentShifting <= m_timeToShift)
        {
            ShiftTiles();
        }
        else
        {
            TileSetShiftStatus nextShiftStatus;

            if (NeedToShiftD1Tiles())
            {
                nextShiftStatus = TileSetShiftStatus::D1Tiles;
            }
            else
            {
                nextShiftStatus = TileSetShiftStatus::None;
            }

            FinalizeTileAndInhabitantLerps();
            Shift(nextShiftStatus);
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
        m_state = Is2D() && IsCartesian() ? TileSetState::Cartesian1D : m_state;
        break;
    }
}

void TileSet::ShiftTiles()
{
    // The payload's row and column.
    orxVECTOR payloadRowAndCol = GetPayloadRowAndColumn();
    // The payload's row's greatest 1D unit distance from threshold.
    int greatest1DUnitDistanceOfPayloadRowFromThreshold = GetGreatest1DUnitDistanceOfPayloadRowFromThreshold();
    // The TileSet's position.
    orxVECTOR pos = GetPosition();
    // The amount by which we're shifting the tiles.
    float lerpWeight = m_timeSpentShifting / m_timeToShift;

    // Shift each tile individually.
    for (int i = 0; i < m_square; i++)
    {
        for (int j = 0; j < m_square; j++)
        {
            // Grab the Tile at this row/column pair.
            Tile *tile = m_tileRows.at(i).at(j);
            // Shift the Tile.
            tile->Shift(i, j, m_square, greatest1DUnitDistanceOfPayloadRowFromThreshold, m_radius, m_normalizedTileSize, NORMALIZED_BORDER_SIZE, lerpWeight, payloadRowAndCol, pos, m_state, m_shiftStatus);
        }
    }
    // Ensure that while shifting is occurring, all TileInhabitants are bound to their respective targets.
    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        ti->SetPosition(ti->m_target->GetPosition());
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

const bool TileSet::PriorStateIs2D()
{
    return m_priorState == TileSetState::Cartesian2D || m_priorState == TileSetState::Polar2D;
}

const bool TileSet::IsCartesian()
{
    return m_state == TileSetState::Cartesian1D || m_state == TileSetState::Cartesian2D;
}

const bool TileSet::InputAllowed()
{
    // Only allow inputs if the player isn't moving and the TileSet isn't shifting.
    return !m_payload->m_bIsMoving && m_shiftStatus == TileSetShiftStatus::None;
}

const bool TileSet::NeedToShiftD1Tiles()
{
    // We only need to D1-shift Tiles if a lerp has finished (m_timeSpentShifting > m_timeToShift),
    // the TileSet is 1D (!Is2D()), the TileSet was previously 2D (PriorStateIs2D()), and the TileSet
    // isn't currently D2-shifting Tiles (m_shiftStatus != TileSetShiftStatus::D1Tiles).
    return m_timeSpentShifting > m_timeToShift && !Is2D() && PriorStateIs2D() && m_shiftStatus != TileSetShiftStatus::D1Tiles;
}

const int TileSet::GetUnitDistanceFromOrigin(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol)
{
    // How many tiles away from the TileSet's pivot is this Tile?
    int unitDistanceFromOrigin;
    // The payload's row.
    int payloadRow = _payloadRowAndCol.fX;

    switch (m_state)
    {
    case TileSetState::Cartesian1D:
        unitDistanceFromOrigin = _col < m_halfSquare ? m_halfSquare - _col : (_col + 1) - m_halfSquare;
        break;
    case TileSetState::Polar1D:
        unitDistanceFromOrigin = m_square - payloadRow;
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

const int TileSet::GetGreatest1DUnitDistanceOfPayloadRowFromThreshold()
{
    int payloadRow = GetPayloadRowAndColumn().fX;

    return orxMAX(payloadRow, (m_square - 1) - payloadRow);
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
        int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(_row, _col, _payloadRowAndCol);
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
        int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(_row, _col, _payloadRowAndCol);
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
        int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(_row, _col, _payloadRowAndCol);
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
        int unitDistanceFromOrigin = GetUnitDistanceFromOrigin(_row, _col, _payloadRowAndCol);
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
