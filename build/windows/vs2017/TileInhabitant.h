#pragma once

#include "Payload.h"
#include "ScrollMod.h"
#include "Tile.h"

namespace payload
{
    class TileInhabitant : public ScrollMod
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
        //! Called whenever the TileInhabitant interacts with another TileInhabitant on its Tile.
        virtual void Cohabitate(TileInhabitant *_cohabitant) = 0;
    public:
        int m_precedence;
        bool m_bIsMoving;
        float m_timeToMove;
        float m_timeSpentMoving;
        float m_tileRatio;
        orxVECTOR m_priorPos;
        orxVECTOR m_tileSetPos;
        Tile *m_target;

        void SetTarget(Tile *_target);
    };
}
