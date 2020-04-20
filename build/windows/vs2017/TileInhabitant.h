#pragma once

#include "Payload.h"
#include "Direction.h"
#include "Doer.h"
#include "Tile.h"
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
        bool m_bIsMoving;
        bool m_bIsTeleporting;
        bool m_bIsSlipping;
        bool m_bIsInfected;
        float m_timeToMove;
        float m_timeSpentMoving;
        float m_tileRatio;
        Direction m_movementDirection;
        orxVECTOR m_priorPos;
        orxVECTOR m_tileSetPos;
        Tile *m_target;
        // A stack of pairs of Tiles and whether or not those Tiles were reached via teleportation or
        // standard movement (if the second pair element is true, then the Tile was reached via teleportation.
        std::stack<std::pair<Tile*, bool>> m_priorTargetStack;

        virtual void Undo();
        virtual void Cohabitate(TileInhabitant *_other, const bool _dueToShifting) = 0;

        void SetTarget(Tile *_target, const Direction _movementDirection, const bool _undoing = false);
        void TeleportTo(Tile *_dest, const bool _undoing = false);
        void SlipTo(Tile *_dest, const Direction _movementDirection);
        void Die();
        //! Called whenever the TileInhabitant interacts with another TileInhabitant on its Tile.
        virtual void Cohabitate(const bool _dueToShifting);
        const bool IsCohabitable();
        const bool IsCohabitating(TileInhabitant *_other);
    };
}
