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
            float lerpWeight = m_timeSpentMoving / m_timeToMove;

            if (m_timeSpentMoving < m_timeToMove)
            {
                orxVECTOR pos;

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
                // Cohabitation and purview action.
                for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
                {
                    TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
                    ti->Cohabitate(false);
                    ti->ExertInfluence();
                }
                // If the TileInhabitant's infected, it must spawn an infection after each movement.
                if (m_bJustInfected)
                {
                    m_bIsInfected = true;
                    m_bJustInfected = false;
                }
                else if (m_bIsInfected)
                {
                    SpawnInfection();
                }
            }
        }
        else if (m_bIsTeleporting)
        {
            m_bIsTeleporting = false;
            SetPosition(m_target->GetPosition());
            m_priorPos = GetPosition();
            // If the TileInhabitant's infected, it must spawn an infection after each teleportation.
            if (m_bJustInfected)
            {
                m_bIsInfected = true;
                m_bJustInfected = false;
            }
            else if (m_bIsInfected)
            {
                SpawnInfection();
            }
        }
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
            // Only execute Cohabitate behavior if the interacting TileInhabitant has a lower precedence and the cohabitation is not due to shifting.
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
    if (!_undoing)
    {
        m_priorTargetStack.push({ m_target, false });
        orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_SET_TARGET, this, Payload::GetInstance().GetTileSet(), nullptr);
    }
    m_target = _target;
    m_bIsMoving = true;
    m_movementDirection = _movementDirection;
}

void TileInhabitant::TeleportTo(Tile *_dest, const bool _undoing)
{
    if (!_undoing)
    {
        m_priorTargetStack.push({ m_target, true });
        orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_TELEPORT_TO, this, Payload::GetInstance().GetTileSet(), nullptr);
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

const bool TileInhabitant::IsCohabitable()
{
    return m_bIsMoving == false && m_bIsTeleporting == false && m_target->m_bIsMoving == false;
}

const bool TileInhabitant::IsCohabitating(TileInhabitant *_other)
{
    return
        m_target->m_b2D ?
        _other != this && _other->m_target->m_row == m_target->m_row && _other->m_target->m_col == m_target->m_col && _other->IsCohabitable() :
        _other != this && _other->m_target->m_col == m_target->m_col && _other->IsCohabitable();
}
