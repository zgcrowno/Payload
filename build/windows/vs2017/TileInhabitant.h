#pragma once

#include "Payload.h"
#include "Doer.h"
#include "Tile.h"
#include <functional>
#include <stack>

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
        float m_timeToMove;
        float m_timeSpentMoving;
        float m_tileRatio;
        orxVECTOR m_priorPos;
        orxVECTOR m_tileSetPos;
        Tile *m_target;
        std::stack<Tile*> m_priorTargetStack;

        virtual void Undo();
        //! Called whenever the TileInhabitant interacts with another TileInhabitant on its Tile.
        virtual void Cohabitate(const bool _dueToShifting) = 0;

        void SetTarget(Tile *_target);
        const bool IsCohabitable();
    };
}
