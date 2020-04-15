#pragma once

#include "Payload.h"
#include "TileInhabitant.h"

namespace payload
{
    class Proxy : public TileInhabitant
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
        //! Called whenever the PlayerPayload interacts with another TileInhabitant on its Tile.
        virtual void Cohabitate(const bool _tileSetIs2D, const bool _dueToShifting);
    };
}
