#pragma once

#include "Payload.h"
#include "TileInhabitant.h"
#include "Direction.h"

namespace payload
{
    class Protocol : public TileInhabitant
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

        virtual void Cohabitate(TileInhabitant *_other, const bool _dueToShifting);
    };
}
