#pragma once

#include "Payload.h"
#include "MemorySet.h"
#include "Tile.h"
#include <vector>

namespace payload
{
    class MemorySetPolar2D : public MemorySet
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
        float m_innerRadius;
        float m_outerRadius;

        virtual void Reconfigure(std::vector<std::vector<Tile*>> &_tileRows);

        void SetUp(
            const int &_row,
            const int &_tileSetHalfSquare,
            const float &_tileSetRadius);
    };
}
