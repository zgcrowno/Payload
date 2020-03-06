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
        //! Called on shader event
        virtual orxBOOL OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload);
        //! Called on clock update
        virtual void Update(const orxCLOCK_INFO &_rstInfo);
        virtual void Draw();
    public:
        int m_unitDistanceFromOrigin;
        float m_topCenterAngle;
        float m_bottomCenterAngle;
        float m_leftEdgeTopAngle;
        float m_leftEdgeBottomAngle;
        float m_rightEdgeTopAngle;
        float m_rightEdgeBottomAngle;
        orxVECTOR m_topCenterPoint;
        orxVECTOR m_bottomCenterPoint;
        orxVECTOR m_leftEdgeTopPoint;
        orxVECTOR m_leftEdgeBottomPoint;
        orxVECTOR m_rightEdgeTopPoint;
        orxVECTOR m_rightEdgeBottomPoint;
    };
}
