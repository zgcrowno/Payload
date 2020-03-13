#pragma once

#include "Payload.h"
#include "TileInhabitant.h"

namespace payload
{
    class Bypass : public TileInhabitant
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
        //! Called whenever the PlayerPayload interacts with another Bypass on its Tile.
        virtual void Cohabitate(TileInhabitant *_cohabitant);
    };
}
