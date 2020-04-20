#pragma once

#include "Payload.h"
#include "Doer.h"
#include "Tile.h"

namespace payload
{
    class MemorySet : public Doer
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
        int m_leftBound;
        int m_rightBound;
        int m_lowerBound;
        int m_upperBound;
        std::vector<Tile*> m_tiles;
        std::vector<std::vector<Tile*>> *m_tileSetTileRows;

        virtual void SetTiles() = 0;
        virtual void Reconfigure(const bool _undoing = false) = 0;

        void Undo();
    };
}
