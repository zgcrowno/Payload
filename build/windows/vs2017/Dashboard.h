#pragma once

#include "Payload.h"
#include "ScrollMod.h"
#include <vector>

namespace payload
{
    class Dashboard : public ScrollMod
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
        float m_minLeafBunchScale;
        float m_maxLeafBunchScale;
        float m_timeToGrowLeaves;
        std::vector<ScrollMod*> m_leafBunches;
    };
}
