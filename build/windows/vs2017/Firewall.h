#pragma once

#include "Payload.h"
#include "TileInhabitant.h"
#include "Direction.h"

namespace payload
{
    class Firewall : public TileInhabitant
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
        Direction m_direction;
        ScrollMod *m_beam;

        virtual void Cohabitate(TileInhabitant *_other, const bool _dueToShifting);
        virtual void ExertInfluence(TileInhabitant *_other);
        virtual const bool IsInPurview(TileInhabitant *_other);
    };
}
