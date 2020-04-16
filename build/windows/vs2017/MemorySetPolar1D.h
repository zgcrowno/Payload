#pragma once

#include "Payload.h"
#include "MemorySetPolar2D.h"
#include "Tile.h"

namespace payload
{
    class MemorySetPolar1D : public MemorySetPolar2D
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
        float m_minTheta;
        float m_maxTheta;

        virtual void SetTiles();
        virtual void Reconfigure();

        void SetUp(
            const int &_row,
            const int &_col,
            const int &_tileSetSquare,
            const float &_tileSetRadius,
            const float &_polarTheta,
            std::vector<std::vector<Tile*>> &_tileRows);
    };
}
