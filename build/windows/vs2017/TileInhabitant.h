#pragma once

#include "Payload.h"
#include "Direction.h"
#include "Doer.h"
#include "Tile.h"
#include "TileInhabitantAction.h"
#include <functional>
#include <stack>
#include <utility>

namespace payload
{
    class TileInhabitant : public Doer
    {
    protected:
        //! Called on object creation
        virtual void OnCreate();
        //! Called on object deletion
        virtual void OnDelete();
        //! Called on object collision
        virtual orxBOOL OnCollide(
            ScrollObject *_poCollider,
            const orxSTRING _zPartName,
            const orxSTRING _zColliderPartName,
            const orxVECTOR &_rvPosition,
            const orxVECTOR &_rvNormal);
        //! Called on clock update
        virtual void Update(const orxCLOCK_INFO &_rstInfo);
    public:
        int m_precedence;
        int m_movementUnitDistance;
        bool m_bIsAMover;
        bool m_bIsMoving;
        bool m_bIsTeleporting;
        bool m_bIsSlipping;
        bool m_bIsInfected;
        bool m_bIsPendingInfection;
        bool m_bIsCaughtInLoop;
        bool m_bDiedByOwnMovement;
        bool m_bInfectedByOwnMovement;
        bool *m_bTileSetIsUndoing;
        float m_timeToMove;
        float m_timeSpentMoving;
        float m_tileRatio;
        Direction m_movementDirection;
        orxVECTOR m_priorPos;
        orxVECTOR m_tileSetPos;
        Tile *m_target;
        // A stack of all prior actions the TileInhabitant has committed.
        std::stack<TileInhabitantAction> m_priorActions;
        // A stack of pairs of Tiles and whether or not those Tiles were reached via teleportation or
        // standard movement (if the second pair element is true, then the Tile was reached via teleportation).
        std::stack<std::pair<Tile*, bool>> m_priorTargetStack;
        // A stack of all the Infections the TileInhabitant has previously spawned.
        std::stack<TileInhabitant*> m_priorSpawnedInfections;
        // A vector containing either no Tiles if the TileInhabitant isn't moving, or all of the unique Tiles it's
        // traversed since it began moving.
        std::vector<Tile*> m_tilesTraversedThisMovement;

        virtual void Undo();
        virtual void Cohabitate(TileInhabitant *_other, const bool _dueToShifting) = 0;
        virtual void ExertInfluence(TileInhabitant *_other);
        virtual const bool IsInPurview(TileInhabitant *_other);

        void SetTarget(Tile *_target, const int _movementUnitDistance, const Direction _movementDirection, const bool _undoing = false);
        void TeleportTo(Tile *_dest, const bool _undoing = false);
        void SlipTo(Tile *_dest, const Direction _movementDirection);
        void Die();
        void Revive();
        void Infect();
        void Disinfect();
        void SpawnInfection();
        void HandleInfection();
        void UndoMove();
        void UndoDie();
        void UndoContractInfection();
        void UndoSpawnInfection();
        // TODO: Maybe consolidate Cohabitate and ExertInfluence into one method?
        //! Called whenever the TileInhabitant interacts with another TileInhabitant on its Tile.
        void Cohabitate(const bool _dueToShifting);
        //! Called whenever the TileInhabitant interacts with another TileInhabitant on a Tile different from its own.
        void ExertInfluence();
        const bool IsDead();
        const bool IsCohabitable();
        const bool IsCohabitating(TileInhabitant *_other);
        const int GetRow() const;
        const int GetCol() const;
    };
}
