#pragma once

#include "Payload.h"
#include "ScrollMod.h"

namespace payload
{
    class Tile : public ScrollMod
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
    public:
        bool m_bCartesian;
        float m_topRadius;
        float m_bottomRadius;
        orxVECTOR m_leftEdgeTopPoint;
        orxVECTOR m_leftEdgeBottomPoint;
        orxVECTOR m_rightEdgeTopPoint;
        orxVECTOR m_rightEdgeBottomPoint;
        orxVECTOR m_visualCenter;
        orxVECTOR m_visualScale;
    };
}
