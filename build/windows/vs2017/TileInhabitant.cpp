#include "TileInhabitant.h"
#include "EventType.h"
#include "Infection.h"

using namespace payload;

void TileInhabitant::OnCreate()
{
    m_precedence = GetU32("Precedence", GetModelName());
    m_timeToMove = GetFloat("TimeToMove", GetModelName());
    m_tileRatio = GetFloat("TileRatio", GetModelName());
    m_bIsAMover = GetBool("IsAMover", GetModelName());
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
                // Destination has been reached.
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
                // If the TileInhabitant is dead after just having finished moving, then it necessarily
                // died by its own movement.
                m_bDiedByOwnMovement = IsDead();
                // If the TileInhabitant is pending infection after just having finished moving, then it
                // necessarily was infected by its own movement.
                m_bInfectedByOwnMovement = m_bIsPendingInfection;
            }
        }
        // TARGET TELEPORTATION
        else if (m_bIsTeleporting)
        {
            m_bIsTeleporting = false;
            SetPosition(m_target->GetPosition());
            m_priorPos = GetPosition();
            HandleInfection();
            // If the TileInhabitant is undoing (and not as part of the TileSet's overarching undoing), and it's just
            // finished a teleportation, we want to undo the original movement that led it to the Proxy to begin with.
            if (m_bIsUndoing && !*m_bTileSetIsUndoing)
            {
                Undo();
            }
            // Otherwise, we know that the TileInhabitant is no longer undoing.
            else
            {
                m_bIsUndoing = false;
            }
        }
        // HANDLE SLIPPING
        else if (m_bIsSlipping)
        {
            Tile *adjacentTileInMovementDirection = m_target->GetTileInDirection(1, m_movementDirection);
            if (adjacentTileInMovementDirection != nullptr)
            {
                SetTarget(adjacentTileInMovementDirection, 1, m_movementDirection);
            }
            else
            {
                m_bIsSlipping = false;
            }
        }
    }
    else
    {
        // The TileInhabitant has been assigned a null target, which means that it will move
        // out of the TileSet to its death.
        if (m_bIsMoving)
        {
            // Increment the time spent moving.
            m_timeSpentMoving += _rstInfo.fDT;
            // The amount by which the TileInhabitant is to move.
            float lerpWeight = m_timeSpentMoving / m_timeToMove;
            // The target from which the TileInhabitant is moving.
            Tile *priorTarget = m_priorTargetStack.top().first;
            // The position to which the TileInhabitant is moving.
            orxVECTOR targetPos = priorTarget->GetPositionInDirection(m_movementUnitDistance, m_movementDirection);

            if (m_timeSpentMoving <= m_timeToMove)
            {
                orxVECTOR pos;

                orxVector_Lerp(&pos, &m_priorPos, &targetPos, lerpWeight);
                SetPosition(pos);
            }
            else
            {
                // Destination has been reached.
                m_bIsMoving = false;
                m_timeSpentMoving = 0.0f;
                SetPosition(targetPos);
                // The TileInhabitant necessarily dies if it lands on a non-Tile, and has necessarily died by
                // its own movement.
                Die();
                m_bDiedByOwnMovement = true;
            }
        }
    }
}

void TileInhabitant::Undo()
{
    // Only execute Undo if there are things to be undone.
    if (!m_priorActions.empty())
    {
        TileInhabitantAction priorAction = m_priorActions.top();

        m_priorActions.pop();

        // Undo behavior is dependent on the type of action we're undoing.
        switch (priorAction)
        {
        case TileInhabitantAction::Move:
            UndoMove();
            break;
        case TileInhabitantAction::Die:
            UndoDie();
            break;
        case TileInhabitantAction::ContractInfection:
            UndoContractInfection();
            break;
        case TileInhabitantAction::SpawnInfection:
            UndoSpawnInfection();
            break;
        }
    }
}

void TileInhabitant::Cohabitate(const bool _dueToShifting)
{
    bool preserveIsUndoing = false;
    bool isPendingMovement = false;

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
            else if (ti->m_precedence > m_precedence)
            {
                // Here, we know that this TileInhabitant will be cohabitated by ti.
                if (m_bIsSlipping || (ti->m_bIsAMover && !*m_bTileSetIsUndoing))
                {
                    isPendingMovement = true;
                }
                // If the TileInhabitant is cohabitating with a mover, we know we need to preserve its
                // m_bIsUndoing status.
                if (ti->m_bIsAMover)
                {
                    preserveIsUndoing = true;
                }
            }
        }
    }

    // If we needn't preserve the TileInhabitant's m_bIsUndoing status, set it to false here.
    if (!preserveIsUndoing)
    {
        m_bIsUndoing = false;
    }

    // If the TileInhabitant doesn't have a movement pending at this point, we clear m_tileTraversedThisMovement
    // since the TileInhabitant has come to rest.
    if (!isPendingMovement)
    {
        m_tilesTraversedThisMovement.clear();
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

void TileInhabitant::SetTarget(Tile *_target, const int _movementUnitDistance, const Direction _movementDirection, const bool _undoing)
{
    bool targetHasAlreadyBeenTraversedThisMovement = std::find(m_tilesTraversedThisMovement.begin(), m_tilesTraversedThisMovement.end(), m_target) != m_tilesTraversedThisMovement.end();
    // Send an event if the TileInhabitant isn't undoing and it's not caught in a loop. This event will be picked up by the TileSet
    // and used to fill out its prior Doers stack.
    if (!_undoing && !targetHasAlreadyBeenTraversedThisMovement)
    {
        m_priorActions.push(TileInhabitantAction::Move);
        m_priorTargetStack.push({ m_target, false });
        // Movement time is the event's payload.
        orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_SET_TARGET, this, Payload::GetInstance().GetTileSet(), &m_timeToMove);
    }
    else if (targetHasAlreadyBeenTraversedThisMovement)
    {
        // If the target has already been traversed by the TileInhabitant in the same inescapable sequence of movements, we know
        // the latter is caught in a loop.
        m_bIsCaughtInLoop = true;
    }

    // Only push _target to m_tilesTraversedThisMovement if the TileInhabitant isn't caught in a loop;
    // that way, we're not cluttering up the vector unnecessarily.
    if (!m_bIsCaughtInLoop)
    {
        m_tilesTraversedThisMovement.push_back(m_target);
    }

    m_target = _target;
    m_bIsMoving = true;
    m_movementUnitDistance = _movementUnitDistance;
    m_movementDirection = _movementDirection;
}

void TileInhabitant::TeleportTo(Tile *_dest, const bool _undoing)
{
    // Send an event if the TileInhabitant isn't undoing. This event will be picked up by the TileSet
    // and used to fill out its prior Doers stack.
    if (!_undoing)
    {
        float timeToTeleport = 0.0f;
        m_priorActions.push(TileInhabitantAction::Move);
        m_priorTargetStack.push({ m_target, true });
        // Teleportation time is the event's payload.
        orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_TELEPORT_TO, this, Payload::GetInstance().GetTileSet(), &timeToTeleport);
    }
    m_target = _dest;
    m_bIsTeleporting = true;
}

void TileInhabitant::SlipTo(Tile *_dest, const Direction _movementDirection)
{
    SetTarget(_dest, 1, m_movementDirection);
    m_bIsSlipping = true;
}

void TileInhabitant::Die()
{
    float timeToDie = 0.0f;
    m_priorActions.push(TileInhabitantAction::Die);
    // Death time is the event's payload.
    orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_DIE, this, Payload::GetInstance().GetTileSet(), &timeToDie);

    Enable(false);
    m_tilesTraversedThisMovement.clear();
}

void TileInhabitant::Revive()
{
    Enable(true);
}

void TileInhabitant::Infect()
{
    float timeToInfect = 0.0f;
    m_priorActions.push(TileInhabitantAction::ContractInfection);
    // Infection time is the event's payload.
    orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_CONTRACT_INFECTION, this, Payload::GetInstance().GetTileSet(), &timeToInfect);

    m_bIsInfected = true;
    m_bIsPendingInfection = false;
}

void TileInhabitant::Disinfect()
{
    m_bIsInfected = false;
}

void TileInhabitant::SpawnInfection()
{
    float timeToSpawnInfection = 0.0f;
    m_priorActions.push(TileInhabitantAction::SpawnInfection);
    // Infection time is the event's payload.
    orxEVENT_SEND(EVENT_TYPE_TILE_INHABITANT, EVENT_TILE_INHABITANT_SPAWN_INFECTION, this, Payload::GetInstance().GetTileSet(), &timeToSpawnInfection);

    ScrollMod *tileSet = static_cast<ScrollMod*>(Payload::GetInstance().GetTileSet());
    Infection *infection = static_cast<Infection*>(CreateObject("O-Infection"));
    infection->SetParent(tileSet);
    infection->m_target = m_target;
    infection->SetPosition(m_target->GetPosition());
    infection->m_priorPos = m_target->GetPosition();
    infection->m_tileSetPos = tileSet->GetPosition();
    m_priorSpawnedInfections.push(infection);
}

void TileInhabitant::HandleInfection()
{
    // If the TileInhabitant's was infected on its last turn, we ACTUALLY infect it now so as
    // to prevent the spawning of infections on the tile which infected the TileInhabitant
    // in the first place.
    if (m_bIsPendingInfection)
    {
        Infect();
    }
    // If the TileInhabitant's infected, it must spawn an infection after each movement.
    else if (m_bIsInfected)
    {
        SpawnInfection();
    }
}

void TileInhabitant::UndoMove()
{
    m_bIsCaughtInLoop = false;

    // Only execute Undo if there are things to be undone.
    if (!m_priorTargetStack.empty())
    {
        if (m_target == nullptr)
        {
            // Set m_priorPos here in the event m_target is null, because we're refraining from setting m_priorPos
            // on update of TileInhabitants with null targets so as to prevent undesired TileSet shader behavior
            // when PlayerPayload is lerping to its death (to a null target).
            m_priorPos = GetPosition();
        }

        m_bIsUndoing = true;
        bool moveByTeleportation = m_priorTargetStack.top().second;

        if (moveByTeleportation)
        {
            TeleportTo(m_priorTargetStack.top().first, true);
        }
        else
        {
            SetTarget(m_priorTargetStack.top().first, m_movementUnitDistance, m_movementDirection, true);
        }

        m_priorTargetStack.pop();
    }
}

void TileInhabitant::UndoDie()
{
    // Revive
    Revive();
    // If the TileInhabitant died by its own movement, and the overarching TileSet is
    // not undoing, we want to make sure to call Undo() again so as to undo the movement
    // which led to the TileInhabitant's death.
    if (m_bDiedByOwnMovement && !*m_bTileSetIsUndoing)
    {
        Undo();
    }
    // This is necessarily false, since it's as if the TileInhabitant's death never happened.
    m_bDiedByOwnMovement = false;
}

void TileInhabitant::UndoContractInfection()
{
    // Disinfect
    Disinfect();
    // If the TileInhabitant was infected by its own movement, and the overarching TileSet is
    // not undoing, we want to make sure to call Undo() again so as to undo the movement
    // which led to the TileInhabitant's infection.
    if (m_bInfectedByOwnMovement && !*m_bTileSetIsUndoing)
    {
        Undo();
    }
    // This is necessarily false, since it's as if the TileInhabitant's infection never happened.
    m_bInfectedByOwnMovement = false;
}

void TileInhabitant::UndoSpawnInfection()
{
    // Destroy the spawned infection, and pop it from the stack.
    TileInhabitant *previouslySpawnedInfection = m_priorSpawnedInfections.top();
    previouslySpawnedInfection->Destroy();
    m_priorSpawnedInfections.pop();
    // If the overarching TileSet is not undoing, we want to make sure to call Undo() again so
    // as to undo the movement which led to the TileInhabitant's spawning of the infection.
    if (!*m_bTileSetIsUndoing)
    {
        Undo();
    }
}

const bool TileInhabitant::IsDead()
{
    return !IsEnabled();
}

const bool TileInhabitant::IsCohabitable()
{
    //return m_target != nullptr && !*m_bTileSetIsUndoing && !m_bIsMoving && !m_bIsTeleporting && !m_target->m_bIsMoving;
    return m_target != nullptr && !*m_bTileSetIsUndoing && !m_bIsMoving && !m_bIsTeleporting && !m_target->m_bIsMoving;
}

const bool TileInhabitant::IsCohabitating(TileInhabitant *_other)
{
    if (m_target == nullptr || _other->m_target == nullptr)
    {
        // If the TileInhabitant's or _other's target is null, one of them is either dead or on its way to death, so it's not cohabitating with anything.
        return false;
    }
    else
    {
        return
            m_target->m_b2D ?
            _other != this && _other->m_target->m_row == m_target->m_row && _other->m_target->m_col == m_target->m_col && _other->IsCohabitable() :
            _other != this && _other->m_target->m_col == m_target->m_col && _other->IsCohabitable();
    }
}

const int TileInhabitant::GetRow() const
{
    return m_target != nullptr ? m_target->m_row : m_priorTargetStack.top().first->m_row;
}

const int TileInhabitant::GetCol() const
{
    return m_target != nullptr ? m_target->m_col : m_priorTargetStack.top().first->m_col;
}
