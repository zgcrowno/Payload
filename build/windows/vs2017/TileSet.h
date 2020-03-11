#pragma once

#include "Payload.h"
#include "PlayerPayload.h"
#include "ScrollMod.h"
#include "Tile.h"
#include <vector>

namespace payload
{
    class TileSet : public ScrollMod
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
    private:
        bool m_2D;
        bool m_cartesian;
        int m_square;
        int m_halfSquare;
        float m_width;
        float m_height;
        float m_radius;
        orxVECTOR m_payloadOrigin;
        PlayerPayload *m_payload;
        std::vector<std::vector<Tile*>> m_tileRows;

        const bool Cartesian2D();
        const bool Cartesian1D();
        const bool Polar2D();
        const bool Polar1D();
    };
}
