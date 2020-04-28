#include "TileInhabitant.h"
#include "EventType.h"
#include "Infection.h"

using namespace payload;

void TileInhabitant::OnCreate()
{
    m_precedence = GetU32("Precedence", GetModelName());
    m_timeToMove = GetFloat("TimeToMove", GetModelName());
    m_tileRatio = GetFloat("TileRatio", GetModelName());
}

void TileInhabitant::OnDelete()
{

}

orxBOOL TileInhabitant::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void TileInhabitant::Update(const orxCLOCK_INFO &_rstInfo)
{
    if (m_target != nullptr)
    {
        // TARGET SCALING
        float minAxis = orxMIN(m_target->m_visualScale.fX, m_target->m_visualScale.fY);
        float scaleRatio = minAxis * m_tileRatio;
        orxVECTOR targetScale = m_target->GetScale();
        SetScale({ targetScale.fX * scaleRatio, targetScale.fY * scaleRatio });
        // TARGET MOVEMENT
        if (m_bIsMoving)
        {
            // Increment the time spent moving.
            m_timeSpentMoving += _rstInfo.fDT;
            // The amount by which the TileInhabitant is to move.
            float lerpWeight = m_timeSpentMoving / m_timeToMove;

            if (m_timeSpentMoving <= m_timeToMove)
            {
                orxVECTOR pos;

                // Whether the TileInhabitant's moving linearly or rotationally is dependent upon whether the TileSet is
                // in a Cartesian or polar coordinate system.
                if (m_target->m_bCartesian)
                {
                    orxVector_Lerp(&pos, &m_priorPos, &m_target->GetPosition(), lerpWeight);
                    SetPosition(pos);
                }
                else
                {
                    orxVECTOR targetPos = m_target->GetPosition();
                    float radialDistance = orxLERP(
                        orxVector_GetDistance(&m_priorPos, &m_tileSetPos),
                        orxVector_GetDistance(&m_tileSetPos, &targetPos),
                        lerpWeight);
                    float theta = LerpAngle(
                        CartesianToPolar(m_priorPos, m_tileSetPos).fY,
                        CartesianToPolar(targetPos, m_tileSetPos).fY,
                        lerpWeight);
                    SetPolarPosition(m_tileSetPos, radialDistance, theta);
                }
            }
            else
            {
                m_bIsMoving = false;
                m_timeSpentMoving = 0.0f;
                SetPosition(m_target->GetPosition());
                m_priorPos = GetPosition();
                // Cohabitation and ExertInfluence actions.
                for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
                {
                    TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
                    ti->Cohabitate(false);
                    ti->ExertInfluence();
                }
                HandleInfection();
                // Set undoing to false at end of this block so the TileInhabitant won't cohabitate while undoing.
                m_bIsUndoing = false;
            }
        }
        // TARGET TELEPORTATION
        else if (m_bIsTeleporting)
        {
            m_bIsTeleporting = false;
            SetPosition(m_target->GetPosition());
            m_priorPos = GetPosition();
            HandleInfection();
            // Set undoing to false at end of this block so the TileInhabitant won't cohabitate while undoing.
            m_bIsUndoing = false;
        }
        // HANDLE SLIPPING
        else if (m_bIsSlipping)
        {
            Tile *adjacentTileInMovementDirection = m_target->GetTileInDirection(1, m_movementDirection);
            if (adjacentTileInMovementDirection != nullptr)
            {
                SetTarget(adjacentTileInMovementDirection, m_movementDirection);
            }
            else
            {
                m_bIsSlipping = false;
            }
        }
    }
}

// TODO: Account for teleportation and virus contraction as well.
void TileInhabitant::Undo()
{
    // Only execute Undo if there are things to be undone.
    if (!m_priorTargetStack.empty())
    {
        m_bIsUndoing = true;
        bool moveByTeleportation = m_priorTargetStack.top().second;

        if (moveByTeleportation)
        {
            TeleportTo(m_priorTargetStack.top().first, true);
        }
        else
        {
            SetTarget(m_priorTargetStack.top().first, m_movementDirection, true);
        }

        m_priorTargetStack.pop();
    }
}

void TileInhabitant::Cohabitate(const bool _dueToShifting)
{
    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        if (IsCohabitating(ti))
        {
            // Only execute Cohabitate behavior if the interacting TileInhabitant has a lower precedence.
            if (ti->m_precedence < m_precedence)
            {
                Cohabitate(ti, _dueToShifting);
            }
        }
    }
}

void TileInhabitant::Cohabitate(TileInhabitant *_other, const bool _dueToShifting)
{

}

void TileInhabitant::ExertInfluence()
{
    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        // Only execute ExertInfluence behavior if the interacting TileInhabitant is in this TileInhabitant's purview.
        if (IsInPurview(ti))
        {
            ExertInfluence(ti);
        }
    }
}

void TileInhabitant::ExertInfluence(TileInhabitant *_other)
{

}

const bool TileInhabitant::IsInPurview(TileInhabitant *_other)
{
    return false;
}

void TileInhabitant::SetTarget(Tile *_target, const Direction _movementDirection, const bool _undoing)
{
    // Send an event if the TileInhabitant isn't undoing. This event will be picked up by the TileSet
    // and used to fill out its prior Doers stack.
    if (!_undoing)
    {
        m_priorTargetStack.push({ m_target, false });
        // Movement time is the event's payload.
        orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_SET_TARGET, this, Payload::GetInstance().GetTileSet(), &m_timeToMove);
    }
    m_target = _target;
    m_bIsMoving = true;
    m_movementDirection = _movementDirection;
}

void TileInhabitant::TeleportTo(Tile *_dest, const bool _undoing)
{
    // Send an event if the TileInhabitant isn't undoing. This event will be picked up by the TileSet
    // and used to fill out its prior Doers stack.
    if (!_undoing)
    {
        float timeToTeleport = 0.0f;
        m_priorTargetStack.push({ m_target, true });
        // Teleportation time is the event's payload.
        orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_TELEPORT_TO, this, Payload::GetInstance().GetTileSet(), &timeToTeleport);
    }
    m_target = _dest;
    m_bIsTeleporting = true;
}

void TileInhabitant::SlipTo(Tile *_dest, const Direction _movementDirection)
{
    SetTarget(_dest, m_movementDirection);
    m_bIsSlipping = true;
}

void TileInhabitant::Die()
{
    Enable(false);
}

void TileInhabitant::SpawnInfection()
{
    ScrollMod *tileSet = static_cast<ScrollMod*>(Payload::GetInstance().GetTileSet());
    Infection *infection = static_cast<Infection*>(CreateObject("O-Infection"));
    infection->SetParent(tileSet);
    infection->m_target = m_target;
    infection->SetPosition(m_target->GetPosition());
    infection->m_priorPos = m_target->GetPosition();
    infection->m_tileSetPos = tileSet->GetPosition();
}

void TileInhabitant::HandleInfection()
{
    // If the TileInhabitant's was infected on its last turn, we ACTUALLY infect it now so as
    // to prevent the spawning of infections on the tile which infected the TileInhabitant
    // in the first place.
    if (m_bJustInfected)
    {
        m_bIsInfected = true;
        m_bJustInfected = false;
    }
    // If the TileInhabitant's infected, it must spawn an infection after each movement.
    else if (m_bIsInfected)
    {
        SpawnInfection();
    }
}

const bool TileInhabitant::IsCohabitable()
{
    return !m_bIsUndoing && !m_bIsMoving && !m_bIsTeleporting && !m_target->m_bIsMoving;
}

const bool TileInhabitant::IsCohabitating(TileInhabitant *_other)
{
    return
        m_target->m_b2D ?
        _other != this && _other->m_target->m_row == m_target->m_row && _other->m_target->m_col == m_target->m_col && _other->IsCohabitable() :
        _other != this && _other->m_target->m_col == m_target->m_col && _other->IsCohabitable();
}
