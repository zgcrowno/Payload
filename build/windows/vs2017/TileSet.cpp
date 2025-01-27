#include "TileSet.h"
#include "Bypass.h"
#include "Firewall.h"
#include "Protocol.h"
#include "Proxy.h"
#include "Unreachable.h"
#include "Recursive.h"
#include "Virus.h"
#include <algorithm>

using namespace payload;

void TileSet::OnCreate()
{
    m_state = static_cast<TileSetState>(GetU32("State", GetModelName()));
    m_square = GetU32("Square", GetModelName());
    m_halfSquare = m_square / 2;
    m_timeToShift = GetFloat("TimeToShift", GetModelName());
    m_timeSpentShifting = m_timeToShift;
    m_timeToReconfigure = GetFloat("TimeToReconfigure", GetModelName());
    m_timeSpentReconfiguring = m_timeToReconfigure;
    int numBorders = m_square + 1;
    float textureThickness = GetSize().fX;
    m_normalizedTileSize = ((NATIVE_TEXTURE_SIZE - (numBorders * NATIVE_BORDER_SIZE)) / m_square) / NATIVE_TEXTURE_SIZE;
    m_width = GetScaledSize().fX;
    m_height = GetScaledSize().fY;
    // TODO: Since width and height are slightly different, I may want to use a different value for m_radius.
    m_radius = m_width / 2.0f;
    m_payload = static_cast<PlayerPayload*>(GetChildByName("O-PlayerPayload"));
    m_goal = static_cast<Goal*>(GetChildByName("O-Goal"));

    // TileSet position, scale and scaledSize;
    orxVECTOR pos = GetPosition();
    orxVECTOR scale = GetScale();
    orxVECTOR scaledSize = GetScaledSize();
    // CREATE TILE ROWS.
    for (int i = 0; i < m_square; i++)
    {
        // Set up the row.
        m_tileRows.push_back({});
        for (int j = 0; j < m_square; j++)
        {
            // Create the Tile at this row/column pair.
            Tile *tile = ScrollCast<Tile*>(CreateChild("O-Tile"));
            // Get the config size of the Tile (in pixels).
            orxVECTOR tileSize = tile->GetSize();
            // Ensure that the Tile has the same world position as the TileSet.
            tile->SetPolarPosition(pos, 0, 0);
            // Ensure that the Tile's scale results in it taking up the exact same screen space as the TileSet.
            tile->SetScale({ scale.fX / tileSize.fX, scale.fY / tileSize.fY });
            // Add the Tile.
            m_tileRows.at(i).push_back(tile);
        }
    }
    // Set m_payload's and m_goal's default targets.
    orxVECTOR payloadOrigin = GetVector("PayloadOrigin", GetModelName());
    orxVECTOR goalOrigin = GetVector("GoalOrigin", GetModelName());
    m_payload->m_target = m_tileRows.at(payloadOrigin.fX).at(payloadOrigin.fY);
    m_payload->m_target->m_row = payloadOrigin.fX;
    m_goal->m_target = m_tileRows.at(goalOrigin.fX).at(goalOrigin.fY);
    // PERFORM INITIAL SETUP OF TILES.
    for (int i = 0; i < m_square; i++)
    {
        for (int j = 0; j < m_square; j++)
        {
            // The Tile at this row/column pair.
            Tile *tile = m_tileRows.at(i).at(j);
            // Perform initial setup of Tile.
            tile->SetUp(i, j, m_square, m_radius, m_normalizedTileSize, NORMALIZED_BORDER_SIZE, m_payload->GetRow(), pos, m_state, m_tileRows);
        }
    }
    // CREATE MEMORYSETS.
    // Cartesian1D
    for (int i = 0; i < m_square; i++)
    {
        for (int j = 0; j < m_square - 1; j++)
        {
            // Create the MemorySetCartesian1D at this row/column pair.
            MemorySetCartesian1D *msc1d = ScrollCast<MemorySetCartesian1D*>(CreateChild("O-MemorySetCartesian1D"));
            // Set it up.
            msc1d->SetUp(i, j, m_square, m_width, m_height, NORMALIZED_BORDER_SIZE, m_normalizedTileSize, pos, m_tileRows);
            // Add it to the member vector.
            m_memorySetsCartesian1D.push_back(msc1d);
        }
    }
    // Cartesian2D
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            // Create the MemorySetCartesian2D at the appropriate quadrant.
            MemorySetCartesian2D *msc2d = ScrollCast<MemorySetCartesian2D*>(CreateChild("O-MemorySetCartesian2D"));
            // Set it up.
            msc2d->SetUp(i, j, m_halfSquare, m_width, m_height, pos, m_tileRows);
            // Add it to the member vector.
            m_memorySetsCartesian2D.push_back(msc2d);
        }
    }
    // Polar1D
    for (int i = 0; i < m_square; i++)
    {
        for (int j = 0; j < m_square; j++)
        {
            // Create the MemorySetPolar1D.
            MemorySetPolar1D *msp1d = ScrollCast<MemorySetPolar1D*>(CreateChild("O-MemorySetPolar1D"));
            // Set it up.
            msp1d->SetUp(i, j, m_square, m_radius, GetPolarTheta(GetUnitDistanceFromPolarAxis(j), m_square, false), m_tileRows);
            // Add it to the member vector.
            m_memorySetsPolar1D.push_back(msp1d);
        }
    }
    // Polar2D
    for (int i = 0; i < m_halfSquare; i++)
    {
        // Create the MemorySetPolar2D.
        MemorySetPolar2D *msp2d = ScrollCast<MemorySetPolar2D*>(CreateChild("O-MemorySetPolar2D"));
        // Set it up.
        msp2d->SetUp(i, m_halfSquare, m_radius, m_tileRows);
        // Add it to the member vector.
        m_memorySetsPolar2D.push_back(msp2d);
    }
    // Set m_payload's and m_goal's default positions, tileSetCenters, and priorPositions.
    m_payload->SetPosition(m_payload->m_target->GetPosition());
    m_payload->m_priorPos = m_payload->GetPosition();
    m_payload->m_tileSetPos = GetPosition();
    m_payload->m_bTileSetIsUndoing = &m_bIsUndoing;
    m_goal->SetPosition(m_goal->m_target->GetPosition());
    m_goal->m_priorPos = m_goal->GetPosition();
    m_goal->m_tileSetPos = GetPosition();
    m_goal->m_bTileSetIsUndoing = &m_bIsUndoing;
    // CREATE TILEINHABITANTS.
    // Unreachable.
    for (int i = 0; i < GetListCount("UnreachableOrigins", GetModelName()); i++)
    {
        orxVECTOR unreachableOrigin = GetListVector("UnreachableOrigins", i, GetModelName());
        Unreachable *unreachable = ScrollCast<Unreachable*>(CreateChild("O-Unreachable"));
        unreachable->m_target = m_tileRows.at(unreachableOrigin.fX).at(unreachableOrigin.fY);
        unreachable->SetPosition(unreachable->m_target->GetPosition());
        unreachable->m_priorPos = unreachable->GetPosition();
        unreachable->m_tileSetPos = GetPosition();
        unreachable->m_bTileSetIsUndoing = &m_bIsUndoing;
    }
    // Protocol.
    for (int i = 0; i < GetListCount("ProtocolLeftOrigins", GetModelName()); i++)
    {
        orxVECTOR protocolOrigin = GetListVector("ProtocolLeftOrigins", i, GetModelName());
        Protocol *protocol = ScrollCast<Protocol*>(CreateChild("O-ProtocolLeft"));
        protocol->m_target = m_tileRows.at(protocolOrigin.fX).at(protocolOrigin.fY);
        protocol->SetPosition(protocol->m_target->GetPosition());
        protocol->m_priorPos = protocol->GetPosition();
        protocol->m_tileSetPos = GetPosition();
        protocol->m_bTileSetIsUndoing = &m_bIsUndoing;
    }
    for (int i = 0; i < GetListCount("ProtocolRightOrigins", GetModelName()); i++)
    {
        orxVECTOR protocolOrigin = GetListVector("ProtocolRightOrigins", i, GetModelName());
        Protocol *protocol = ScrollCast<Protocol*>(CreateChild("O-ProtocolRight"));
        protocol->m_target = m_tileRows.at(protocolOrigin.fX).at(protocolOrigin.fY);
        protocol->SetPosition(protocol->m_target->GetPosition());
        protocol->m_priorPos = protocol->GetPosition();
        protocol->m_tileSetPos = GetPosition();
        protocol->m_bTileSetIsUndoing = &m_bIsUndoing;
    }
    for (int i = 0; i < GetListCount("ProtocolUpOrigins", GetModelName()); i++)
    {
        orxVECTOR protocolOrigin = GetListVector("ProtocolUpOrigins", i, GetModelName());
        Protocol *protocol = ScrollCast<Protocol*>(CreateChild("O-ProtocolUp"));
        protocol->m_target = m_tileRows.at(protocolOrigin.fX).at(protocolOrigin.fY);
        protocol->SetPosition(protocol->m_target->GetPosition());
        protocol->m_priorPos = protocol->GetPosition();
        protocol->m_tileSetPos = GetPosition();
        protocol->m_bTileSetIsUndoing = &m_bIsUndoing;
    }
    for (int i = 0; i < GetListCount("ProtocolDownOrigins", GetModelName()); i++)
    {
        orxVECTOR protocolOrigin = GetListVector("ProtocolDownOrigins", i, GetModelName());
        Protocol *protocol = ScrollCast<Protocol*>(CreateChild("O-ProtocolDown"));
        protocol->m_target = m_tileRows.at(protocolOrigin.fX).at(protocolOrigin.fY);
        protocol->SetPosition(protocol->m_target->GetPosition());
        protocol->m_priorPos = protocol->GetPosition();
        protocol->m_tileSetPos = GetPosition();
        protocol->m_bTileSetIsUndoing = &m_bIsUndoing;
    }
    // Bypass.
    for (int i = 0; i < GetListCount("BypassOrigins", GetModelName()); i++)
    {
        orxVECTOR bypassOrigin = GetListVector("BypassOrigins", i, GetModelName());
        Bypass *bypass = ScrollCast<Bypass*>(CreateChild("O-Bypass"));
        bypass->m_target = m_tileRows.at(bypassOrigin.fX).at(bypassOrigin.fY);
        bypass->SetPosition(bypass->m_target->GetPosition());
        bypass->m_priorPos = bypass->GetPosition();
        bypass->m_tileSetPos = GetPosition();
        bypass->m_bTileSetIsUndoing = &m_bIsUndoing;
    }
    // Proxy.
    std::vector<Proxy*> tempProxies;
    for (int i = 0; i < GetListCount("ProxyOrigins", GetModelName()); i++)
    {
        orxVECTOR proxyOrigin = GetListVector("ProxyOrigins", i, GetModelName());
        Proxy *proxy = ScrollCast<Proxy*>(CreateChild("O-Proxy"));
        proxy->m_target = m_tileRows.at(proxyOrigin.fX).at(proxyOrigin.fY);
        proxy->SetPosition(proxy->m_target->GetPosition());
        proxy->m_priorPos = proxy->GetPosition();
        proxy->m_tileSetPos = GetPosition();
        proxy->m_bTileSetIsUndoing = &m_bIsUndoing;
        tempProxies.push_back(proxy);
    }
    if (!tempProxies.empty())
    {
        for (int i = 0; i < tempProxies.size() - 1; i++)
        {
            Proxy *proxy = tempProxies.at(i);
            Proxy *nextProxy = tempProxies.at(i + 1);
            proxy->m_counterpart = nextProxy;
            nextProxy->m_counterpart = proxy;
        }
    }
    // Recursive.
    for (int i = 0; i < GetListCount("RecursiveOrigins", GetModelName()); i++)
    {
        orxVECTOR recursiveOrigin = GetListVector("RecursiveOrigins", i, GetModelName());
        Recursive *recursive = ScrollCast<Recursive*>(CreateChild("O-Recursive"));
        recursive->m_target = m_tileRows.at(recursiveOrigin.fX).at(recursiveOrigin.fY);
        recursive->SetPosition(recursive->m_target->GetPosition());
        recursive->m_priorPos = recursive->GetPosition();
        recursive->m_tileSetPos = GetPosition();
        recursive->m_bTileSetIsUndoing = &m_bIsUndoing;
    }
    // Virus.
    for (int i = 0; i < GetListCount("VirusOrigins", GetModelName()); i++)
    {
        orxVECTOR virusOrigin = GetListVector("VirusOrigins", i, GetModelName());
        Virus *virus = ScrollCast<Virus*>(CreateChild("O-Virus"));
        virus->m_target = m_tileRows.at(virusOrigin.fX).at(virusOrigin.fY);
        virus->SetPosition(virus->m_target->GetPosition());
        virus->m_priorPos = virus->GetPosition();
        virus->m_tileSetPos = GetPosition();
        virus->m_bTileSetIsUndoing = &m_bIsUndoing;
    }
    // Firewall.
    for (int i = 0; i < GetListCount("FirewallLeftOrigins", GetModelName()); i++)
    {
        orxVECTOR firewallOrigin = GetListVector("FirewallLeftOrigins", i, GetModelName());
        Firewall *firewall = ScrollCast<Firewall*>(CreateChild("O-FirewallLeft"));
        firewall->m_target = m_tileRows.at(firewallOrigin.fX).at(firewallOrigin.fY);
        firewall->SetPosition(firewall->m_target->GetPosition());
        firewall->m_priorPos = firewall->GetPosition();
        firewall->m_tileSetPos = GetPosition();
        firewall->m_bTileSetIsUndoing = &m_bIsUndoing;
        firewall->m_beam->SetParentSpaceScale({ sqrtf(powf(m_square, 2.0f) + powf(m_square, 2.0f)) * (1.0f / firewall->m_tileRatio), firewall->m_beam->GetParentSpaceScale().fY });
    }
    for (int i = 0; i < GetListCount("FirewallRightOrigins", GetModelName()); i++)
    {
        orxVECTOR firewallOrigin = GetListVector("FirewallRightOrigins", i, GetModelName());
        Firewall *firewall = ScrollCast<Firewall*>(CreateChild("O-FirewallRight"));
        firewall->m_target = m_tileRows.at(firewallOrigin.fX).at(firewallOrigin.fY);
        firewall->SetPosition(firewall->m_target->GetPosition());
        firewall->m_priorPos = firewall->GetPosition();
        firewall->m_tileSetPos = GetPosition();
        firewall->m_bTileSetIsUndoing = &m_bIsUndoing;
        firewall->m_beam->SetParentSpaceScale({ sqrtf(powf(m_square, 2.0f) + powf(m_square, 2.0f)) * (1.0f / firewall->m_tileRatio), firewall->m_beam->GetParentSpaceScale().fY });
    }
    for (int i = 0; i < GetListCount("FirewallUpOrigins", GetModelName()); i++)
    {
        orxVECTOR firewallOrigin = GetListVector("FirewallUpOrigins", i, GetModelName());
        Firewall *firewall = ScrollCast<Firewall*>(CreateChild("O-FirewallUp"));
        firewall->m_target = m_tileRows.at(firewallOrigin.fX).at(firewallOrigin.fY);
        firewall->SetPosition(firewall->m_target->GetPosition());
        firewall->m_priorPos = firewall->GetPosition();
        firewall->m_tileSetPos = GetPosition();
        firewall->m_bTileSetIsUndoing = &m_bIsUndoing;
        firewall->m_beam->SetParentSpaceScale({ sqrtf(powf(m_square, 2.0f) + powf(m_square, 2.0f)) * (1.0f / firewall->m_tileRatio), firewall->m_beam->GetParentSpaceScale().fY });
    }
    for (int i = 0; i < GetListCount("FirewallDownOrigins", GetModelName()); i++)
    {
        orxVECTOR firewallOrigin = GetListVector("FirewallDownOrigins", i, GetModelName());
        Firewall *firewall = ScrollCast<Firewall*>(CreateChild("O-FirewallDown"));
        firewall->m_target = m_tileRows.at(firewallOrigin.fX).at(firewallOrigin.fY);
        firewall->SetPosition(firewall->m_target->GetPosition());
        firewall->m_priorPos = firewall->GetPosition();
        firewall->m_tileSetPos = GetPosition();
        firewall->m_bTileSetIsUndoing = &m_bIsUndoing;
        firewall->m_beam->SetParentSpaceScale({ sqrtf(powf(m_square, 2.0f) + powf(m_square, 2.0f)) * (1.0f / firewall->m_tileRatio), firewall->m_beam->GetParentSpaceScale().fY });
    }
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
    else if (!orxString_Compare(_rstPayload.zParamName, "State"))
    {
        _rstPayload.fValue = static_cast<float>(m_state);
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "PriorState"))
    {
        _rstPayload.fValue = static_cast<float>(m_priorState);
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "ShiftStatus"))
    {
        _rstPayload.fValue = static_cast<float>(m_shiftStatus);
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
    else if (!orxString_Compare(_rstPayload.zParamName, "PayloadTargetIsNull"))
    {
        _rstPayload.fValue = m_payload->m_target == nullptr;
    }

    return orxTRUE;
}

void TileSet::Update(const orxCLOCK_INFO &_rstInfo)
{
    // HANDLE INPUTS
    if (UndoInputIsAllowed())
    {
        if (orxInput_HasBeenActivated("Undo"))
        {
            Undo();
        }
    }
    if (NonUndoInputIsAllowed())
    {
        if (orxInput_HasBeenActivated("DimShift"))
        {
            m_priorDoers.push({ { this, m_timeSpentShifting }, false });
            m_priorStates.push(m_state);

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
                Shift(TileSetShiftStatus::D1Tiles);
            }
        }
        else if (orxInput_HasBeenActivated("CoShift"))
        {
            m_priorDoers.push({ { this, m_timeSpentShifting }, false });
            m_priorStates.push(m_state);

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
            Tile *tileToRight = m_payload->m_target->GetTileInDirection(1, Direction::Right);
            if (tileToRight != nullptr)
            {
                m_payload->SetTarget(tileToRight, 1, Direction::Right);
            }
        }
        else if (orxInput_HasBeenActivated("MoveUp"))
        {
            Tile *tileAbove = IsCartesian() ? m_payload->m_target->GetTileInDirection(1, Direction::Up) : m_payload->m_target->GetTileInDirection(1, Direction::Down);
            if (tileAbove != nullptr)
            {
                m_payload->SetTarget(tileAbove, 1, Direction::Up);
            }
        }
        else if (orxInput_HasBeenActivated("MoveDown"))
        {
            Tile *tileBelow = IsCartesian() ? m_payload->m_target->GetTileInDirection(1, Direction::Down) : m_payload->m_target->GetTileInDirection(1, Direction::Up);
            if (tileBelow != nullptr)
            {
                m_payload->SetTarget(tileBelow, 1, Direction::Down);
            }
        }
        else if (orxInput_HasBeenActivated("MoveLeft"))
        {
            Tile *tileToLeft = m_payload->m_target->GetTileInDirection(1, Direction::Left);
            if (tileToLeft != nullptr)
            {
                m_payload->SetTarget(tileToLeft, 1, Direction::Left);
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
            m_bInvertReconfigure = false;
            Reconfigure();
        }
    }
    // HANDLE SHIFTING
    if (m_shiftStatus != TileSetShiftStatus::None)
    {
        // Increment the time spent shifting.
        m_timeSpentShifting += _rstInfo.fDT;

        // If m_timeSpentShifting <= m_timeToShift OR something is either moving or pending movement, the
        // shift hasn't truly finished.
        if (m_timeSpentShifting <= m_timeToShift || SomethingIsMoving() || SomethingIsPendingMovement())
        {
            ShiftTiles();

            // HANDLE UNDOING
            if (m_bIsUndoing)
            {
                // We want to undo and pop the top element of m_priorDoers if it was pushed due to shifting AND
                // we're at the point in the shift at which the undo should occur.
                if (m_priorDoers.top().second && m_priorDoers.top().first.second >= m_timeToShift - m_timeSpentShifting)
                {
                    m_priorDoers.top().first.first->Undo();
                    m_priorDoers.pop();
                }
            }
        }
        else
        {
            TileSetShiftStatus nextShiftStatus;

            // We've already shifted to Polar2D, but we're in the process of filling out the tiles of a Polar1D visualization.
            if (m_shiftStatus == TileSetShiftStatus::D1Tiles && m_state == TileSetState::Polar2D)
            {
                nextShiftStatus = TileSetShiftStatus::D2;
            }
            // We've already shifted to Polar1D, but we need to unfill the tiles of the Polar1D visualization.
            else if (m_shiftStatus != TileSetShiftStatus::D1Tiles && m_state == TileSetState::Polar1D && m_priorState != TileSetState::Cartesian1D)
            {
                nextShiftStatus = TileSetShiftStatus::D1Tiles;
            }
            // We've fully completed all stages of whatever shift we've been performing.
            else
            {
                nextShiftStatus = TileSetShiftStatus::None;
            }

            // Ensure all Tiles and TileInhabitants are where they ought to be, come the end of the shift, and then shift.
            FinalizeTileAndInhabitantLerps();
            Shift(nextShiftStatus);
        }
    }
    else
    {
        // HANDLE RECONFIGURATION
        if (m_memorySetToReconfigure != nullptr)
        {
            // Increment the time spent shifting.
            m_timeSpentReconfiguring += _rstInfo.fDT;

            // Has the lerp finished?
            if (m_timeSpentReconfiguring <= m_timeToReconfigure)
            {
                ReconfigureTiles();
            }
            else
            {
                SetMemorySetToReconfigure(nullptr);
            }
        }
        // HANDLE UNDOING OUTSIDE OF SHIFTING
        if (m_bIsUndoing && m_shiftRelativeUndoTime < 0.0f)
        {
            // Increment the time spent undoing.
            m_shiftRelativeUndoTime += _rstInfo.fDT;

            // If m_bIsUndoing, we know that m_priorDoers.top() is a TileInhabitant.
            TileInhabitant *topTileInhabitant = dynamic_cast<TileInhabitant*>(m_priorDoers.top().first.first);
            // Note that we make sure topTileInhabitant isn't moving because it's possible we could execute this code before
            // it reaches its destination (assuming it's moving).
            if (m_priorDoers.top().second && !topTileInhabitant->m_bIsMoving && m_shiftRelativeUndoTime >= (m_timeToShift - m_priorDoers.top().first.second))
            {
                // We want to undo and pop the top element of m_priorDoers if it was pushed due to shifting AND
                // we're at the point in the shift at which the undo should occur.
                m_priorDoers.top().first.first->Undo();
                m_priorDoers.pop();
            }
            // If m_shiftRelativeUndoTime >= 0.0f, we know that we need to call
            // Undo() again to facilitate the Undo-style shifting of the TileSet.
            if (m_shiftRelativeUndoTime >= 0.0f)
            {
                Undo();
            }
        }
    }
}

void TileSet::Undo()
{
    if (!m_priorDoers.empty())
    {
        // The TileSet must undo its shift if we're explicitly undoing a shift OR if we're undoing an
        // action which was caused by a shift, and which was completed before the instigating shift was.
        if (m_priorDoers.top().first.first == this || (m_priorDoers.top().second && m_priorDoers.top().first.second <= m_timeToShift))
        {
            m_bIsUndoing = true;

            if (!m_priorStates.empty())
            {
                TileSetState priorState = m_priorStates.top();
                switch (priorState)
                {
                case TileSetState::Cartesian1D:
                    if (Is2D())
                    {
                        Shift(TileSetShiftStatus::D1Tiles);
                    }
                    else
                    {
                        Shift(TileSetShiftStatus::Cartesian);
                    }
                    break;
                case TileSetState::Cartesian2D:
                    if (Is2D())
                    {
                        Shift(TileSetShiftStatus::Cartesian);
                    }
                    else
                    {
                        Shift(TileSetShiftStatus::D1Tiles);
                    }
                    break;
                case TileSetState::Polar1D:
                    if (Is2D())
                    {
                        Shift(TileSetShiftStatus::D1);
                    }
                    else
                    {
                        Shift(TileSetShiftStatus::Polar);
                    }
                    break;
                case TileSetState::Polar2D:
                    if (Is2D())
                    {
                        Shift(TileSetShiftStatus::Polar);
                    }
                    else
                    {
                        Shift(TileSetShiftStatus::D1Tiles);
                    }
                    break;
                }

                // Pop m_priorStates, since we no longer need its top element at this point.
                m_priorStates.pop();
            }
        }
        else
        {
            // if we're undoing an action which was caused by a shift, but which was completed AFTER the instigating shift was.
            if (m_priorDoers.top().second)
            {
                // Set m_bIsUndoing to true since we've started an undo process that will at some point include undoing a shift.
                m_bIsUndoing = true;
                // Set m_shiftRelativeUndoTime equal to m_timeToShift - [the original m_timeSpentShifting value when the original action was performed
                // + the time taken to complete the original action].
                m_shiftRelativeUndoTime = m_timeToShift - m_priorDoers.top().first.second;
            }

            MemorySet *memSet = dynamic_cast<MemorySet*>(m_priorDoers.top().first.first);
            if (memSet != nullptr)
            {
                // If the topmost prior doer is a MemorySet, set m_bInvertReconfigure to true so its associated Tiles know to reverse their reconfigurations.
                m_bInvertReconfigure = true;
                SetMemorySetToReconfigure(memSet);
            }

            // Call the topmost doer's Undo method, and pop it from the stack.
            m_priorDoers.top().first.first->Undo();
            m_priorDoers.pop();
        }
    }
}

void TileSet::SetMemorySetToReconfigure(MemorySet *_memSet)
{
    if (_memSet == nullptr)
    {
        // Finalize lerps since reconfiguration is complete.
        FinalizeTileAndInhabitantLerps();
        // Clear m_memorySetToReconfigure's m_tiles since we're done with them.
        m_memorySetToReconfigure->m_tiles.clear();
    }
    else
    {
        // Reset m_timeSpentReconfiguring since we're starting a fresh reconfiguration.
        m_timeSpentReconfiguring = 0.0f;
        // Call its Reconfigure method only if we're not undoing a reconfiguration.
        if (!m_bInvertReconfigure)
        {
            _memSet->Reconfigure();
        }
    }

    m_memorySetToReconfigure = _memSet;
}

void TileSet::Shift(TileSetShiftStatus _shiftStatus)
{
    m_priorShiftStatus = m_shiftStatus;
    m_priorState = m_state;
    m_shiftStatus = _shiftStatus;

    if (_shiftStatus != TileSetShiftStatus::None)
    {
        // Shifting mid-shift.
        if (m_timeSpentShifting > 0.0f && m_timeSpentShifting < m_timeToShift)
        {
            // Set m_timeSpentShifting to its analogous time in a reversed shift.
            m_timeSpentShifting = m_timeToShift - m_timeSpentShifting;
            // Set all Tiles' prior values to their target values in order to accurately reverse the shift.
            for (int i = 0; i < m_square; i++)
            {
                for (int j = 0; j < m_square; j++)
                {
                    // Grab the Tile at this row/column pair.
                    Tile *tile = m_tileRows.at(i).at(j);
                    // Set the Tile's m_priorParentSpacePos to its m_targetParentSpacePos to accurately reverse the shift.
                    tile->m_priorParentSpacePos = tile->m_targetParentSpacePos;
                    // Set the Tile's m_priorTileSetState to the TileSet's current state to accurately reverse the shift.
                    tile->m_priorTileSetState = m_state;
                    // Set the Tile's m_priorVisualScale to its m_targetVisualScale to accurately reverse the shift.
                    tile->m_priorVisualScale = tile->m_targetVisualScale;
                }
            }
        }
        // Shifting fresh.
        else
        {
            m_timeSpentShifting = 0.0f;
        }
    }
    else
    {
        if (m_bIsUndoing)
        {
            // If the TileSet is undoing, a shift has been undone, so we pop the m_priorDoers stack, knowing that the
            // top element will be the TileSet, and its no longer needed at this point, since the shift has finished.
            m_priorDoers.pop();
            // The TileSet is necessarily not undoing if it has been shifted to TileSetShiftStatus::None.
            m_bIsUndoing = false;
        }
        else
        {
            // Ensure that TileInhabitants cohabitate upon the TileSet being shifted to TileSetShiftStatus::None,
            // because the only other enforcement of this is in the Update method if m_shiftStatus != TileSetShiftStatus::None.
            std::vector<ScrollObject*> tileInhabitants = Payload::GetInstance().GetTileInhabitants();
            for (ScrollObject *tileInhabitant : tileInhabitants)
            {
                TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
                if (ti->IsCohabitable())
                {
                    ti->ExertInfluence();
                    ti->Cohabitate(true);
                }
            }
        }
    }

    // Set m_state as appropriate.
    switch (_shiftStatus)
    {
    case TileSetShiftStatus::D1:
        m_state = IsCartesian() ? TileSetState::Cartesian1D : TileSetState::Polar1D;
        break;
    case TileSetShiftStatus::D2:
        m_state = IsCartesian() ? TileSetState::Cartesian2D : TileSetState::Polar2D;
        break;
    case TileSetShiftStatus::Cartesian:
        m_state = Is2D() ? TileSetState::Cartesian2D : TileSetState::Cartesian1D;
        break;
    case TileSetShiftStatus::Polar:
        m_state = Is2D() ? TileSetState::Polar2D : TileSetState::Polar1D;
        break;
    case TileSetShiftStatus::D1Tiles:
        switch (m_state)
        {
        case TileSetState::Cartesian1D:
            m_state = TileSetState::Cartesian2D;
            break;
        case TileSetState::Cartesian2D:
            m_state = TileSetState::Cartesian1D;
            break;
        case TileSetState::Polar1D:
            if (m_priorShiftStatus == TileSetShiftStatus::None || m_priorShiftStatus == TileSetShiftStatus::D1Tiles)
            {
                m_state = TileSetState::Polar2D;
            }
            else
            {
                m_state = TileSetState::Polar1D;
            }
            break;
        case TileSetState::Polar2D:
            m_state = TileSetState::Polar2D;
            break;
        }
        break;
    }
}

void TileSet::ShiftTiles()
{
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
            tile->Shift(m_square, greatest1DUnitDistanceOfPayloadRowFromThreshold, m_normalizedTileSize, NORMALIZED_BORDER_SIZE, lerpWeight, m_payload->GetRow(), m_shiftStatus);
        }
    }
    // Ensure that while shifting is occurring, all TileInhabitants are bound to their respective targets, as appropriate.
    std::vector<ScrollObject*> tileInhabitants = Payload::GetInstance().GetTileInhabitants();
    for (ScrollObject *tileInhabitant : tileInhabitants)
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        if (ti->m_target != nullptr)
        {
            // Bind TileInhabitant to its target if their positions aren't equal and the former isn't moving.
            if (!orxVector_AreEqual(&ti->GetPosition(), &ti->m_target->GetPosition()) && !ti->m_bIsMoving)
            {
                ti->SetPosition(ti->m_target->GetPosition());
                ti->m_priorPos = ti->GetPosition();
            }
            // TileInhabitant exerts influence and cohabitates if it's cohabitatable.
            if (ti->IsCohabitable())
            {
                ti->ExertInfluence();
                ti->Cohabitate(true);
            }
        }
    }
}

void TileSet::Reconfigure()
{
    orxVECTOR mousePos = orxVECTOR_0;

    // Pick MemorySet type depending on m_state.
    switch (m_state)
    {
    case TileSetState::Cartesian1D:
    {
        // Pick the MemorySet.
        ScrollObject *memSet = Payload::GetInstance().PickObject(ScreenToWorldSpace(*orxMouse_GetPosition(&mousePos)), orxString_GetID("memorySetCartesian1D"));
        MemorySetCartesian1D *msc1d = dynamic_cast<MemorySetCartesian1D*>(memSet);
        // It's the MemorySet we want if its row is the same as m_payload's.
        if (msc1d != nullptr && msc1d->m_row == m_payload->GetRow())
        {
            SetMemorySetToReconfigure(msc1d);
        }
    }
    break;
    case TileSetState::Cartesian2D:
    {
        // Pick the MemorySet.
        ScrollObject *memSet = Payload::GetInstance().PickObject(ScreenToWorldSpace(*orxMouse_GetPosition(&mousePos)), orxString_GetID("memorySetCartesian2D"));
        MemorySetCartesian2D *msc2d = dynamic_cast<MemorySetCartesian2D*>(memSet);
        if (msc2d != nullptr)
        {
            SetMemorySetToReconfigure(msc2d);
        }
    }
    break;
    case TileSetState::Polar1D:
    {
        // Polarize the mouse cursor's position.
        orxVECTOR polarMousePos = CartesianToPolar(ScreenToWorldSpace(*orxMouse_GetPosition(&mousePos)), GetPosition());
        // Ensure that the polarized mouse cursor position is never negative.
        float positivePolarMouseTheta = polarMousePos.fY < 0 ? orxMATH_KF_PI + (orxMATH_KF_PI - fabsf(polarMousePos.fY)) : polarMousePos.fY;
        for (MemorySetPolar1D *msp1d : m_memorySetsPolar1D)
        {
            bool inRadius = polarMousePos.fX >= msp1d->m_innerRadius && polarMousePos.fX < msp1d->m_outerRadius;
            bool inTheta = positivePolarMouseTheta >= msp1d->m_minTheta && positivePolarMouseTheta < msp1d->m_maxTheta;
            // It's the MemorySet we want if the mouse cursor's radius and angle (polar coordinates) are within the former's radial and angular bounds.
            if (inRadius && inTheta)
            {
                SetMemorySetToReconfigure(msp1d);
            }
        }
    }
    break;
    case TileSetState::Polar2D:
    {
        // Polarize the mouse cursor's position.
        orxVECTOR polarMousePos = CartesianToPolar(ScreenToWorldSpace(*orxMouse_GetPosition(&mousePos)), GetPosition());
        for (MemorySetPolar2D *msp2d : m_memorySetsPolar2D)
        {
            // It's the MemorySet we want if the mouse cursor's radius (first polar coordinate) is within the former's radial bounds.
            if (polarMousePos.fX >= msp2d->m_innerRadius && polarMousePos.fX < msp2d->m_outerRadius)
            {
                SetMemorySetToReconfigure(msp2d);
            }
        }
    }
    break;
    }
}

void TileSet::ReconfigureTiles()
{
    // The amount by which we're reconfiguring the tiles.
    float lerpWeight = m_timeSpentReconfiguring / m_timeToReconfigure;
    // Reconfigure each tile individually.
    for (Tile *tile : m_memorySetToReconfigure->m_tiles)
    {
        tile->Reconfigure(m_payload->GetRow(), lerpWeight, NORMALIZED_BORDER_SIZE, m_normalizedTileSize, m_memorySetToReconfigure->GetPosition(), m_bInvertReconfigure);
    }
    // Ensure that while shifting is occurring, all TileInhabitants are bound to their respective targets.
    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        if (ti->m_target != nullptr)
        {
            ti->SetPosition(ti->m_target->GetPosition());
        }
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
            // Set the Tile's m_priorVisualScale for future lerping Tile-side.
            tile->m_priorVisualScale = tile->m_visualScale;
            // Set the Tile's m_bIsMoving to false, since the lerp has obviously been completed.
            tile->m_bIsMoving = false;
        }
    }

    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        // Only finalize TileInhabitants' lerps if theyir target isn't null, and they're and not moving.
        if (ti->m_target != nullptr && !ti->m_bIsMoving)
        {
            // Ensure TI completes its lerp such that its position is exactly the same as its target's.
            ti->SetPosition(ti->m_target->GetPosition());
            // Set the TileInhabitant's m_priorPos for future lerping TI-side.
            ti->m_priorPos = ti->GetPosition();
        }
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

const bool TileSet::UndoInputIsAllowed()
{
    // TODO: Keep an eye on this, since we may need to ensure that the ONLY things that are moving are those TileInhabitants
    // that are caught in loops.
    // Only allow undo inputs if the TileSet isn't already undoing, the TileSet isn't shifting, the TileSet isn't reconfiguring,
    // and either nothing is moving OR something is caught in a loop.
    return !m_bIsUndoing && m_shiftStatus == TileSetShiftStatus::None && m_memorySetToReconfigure == nullptr && (!SomethingIsMoving() || SomethingIsCaughtInALoop());
}

const bool TileSet::NonUndoInputIsAllowed()
{
    // Only allow non-undo inputs if the TileSet isn't undoing, shifting or reconfiguring, and nothing is moving.
    return !m_bIsUndoing && m_shiftStatus == TileSetShiftStatus::None && m_memorySetToReconfigure == nullptr && !SomethingIsMoving();
}

const bool TileSet::SomethingIsMoving()
{
    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        if (ti->m_bIsMoving)
        {
            return true;
        }
    }

    return false;
}

const bool TileSet::SomethingIsPendingMovement()
{
    std::vector<ScrollObject*> tileInhabitants = Payload::GetInstance().GetTileInhabitants();

    for (ScrollObject *tileInhabitant1 : tileInhabitants)
    {
        TileInhabitant *ti1 = static_cast<TileInhabitant*>(tileInhabitant1);

        for (ScrollObject *tileInhabitant2 : tileInhabitants)
        {
            TileInhabitant *ti2 = static_cast<TileInhabitant*>(tileInhabitant2);

            if (ti1->IsCohabitating(ti2) && ti2->m_bIsAMover)
            {
                return true;
            }
        }
    }

    return false;
}

const bool TileSet::SomethingIsCaughtInALoop()
{
    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        if (ti->m_bIsCaughtInLoop)
        {
            return true;
        }
    }

    return false;
}

const int TileSet::GetUnitDistanceFromPolarAxis(const int &_col)
{
    int unitDistanceFromPolarAxis = ((3 * m_square) / 4) - _col;
    if (unitDistanceFromPolarAxis <= 0)
    {
        unitDistanceFromPolarAxis += m_square;
    }
    if (unitDistanceFromPolarAxis == m_square && m_halfSquare % 2 != 0)
    {
        unitDistanceFromPolarAxis = 0;
    }

    return unitDistanceFromPolarAxis;
}

const float TileSet::GetPolarTheta(const int &_unitDistanceFromPolarAxis, const int &_tilesInPolarRow, const bool _d2)
{
    // At what angle is the Tile, in reference to the TileSet's pivot?
    float theta;

    if (_d2)
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

const int TileSet::GetGreatest1DUnitDistanceOfPayloadRowFromThreshold()
{
    int payloadRow = m_payload->GetRow();

    return orxMAX(payloadRow, (m_square - 1) - payloadRow);
}

const orxVECTOR TileSet::GetNormalizedPosition(const orxVECTOR &_vec)
{
    orxVECTOR pos = GetPosition();
    orxVECTOR scaledSize = GetScaledSize();
    orxVECTOR upperLeftCorner = { pos.fX - (scaledSize.fX / 2.0f), pos.fY - (scaledSize.fY / 2.0f) };
    return { (_vec.fX - upperLeftCorner.fX) / scaledSize.fX, (_vec.fY - upperLeftCorner.fY) / scaledSize.fY };
}
