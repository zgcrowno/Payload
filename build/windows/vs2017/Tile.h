#pragma once

#include "Payload.h"
#include "Drawn.h"
#include "TileEdge.h"
#include <vector>

namespace payload
{
    class Tile : public Drawn
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
        virtual void Draw();
    private:
        orxRGBA m_fillColor;
        orxRGBA m_borderColor;
    public:
        std::vector<TileEdge*> m_edges;
    };
}
