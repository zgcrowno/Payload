#pragma once

#include "Payload.h"
#include "ScrollMod.h"
#include "Moon.h"
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
        //! At what passage of time (in seconds) does the Dashboard's leaf bunch animation start?
        float m_animStartTimeLeafBunch;
        //! At what passage of time (in seconds) does the Dashboard's leaf bunch animation end?
        float m_animEndTimeLeafBunch;
        //! At what passage of time (in seconds) does the Dashboard's moon animation start?
        float m_animStartTimeMoon;
        //! At what passage of time (in seconds) does the Dashboard's moon bunch animation end?
        float m_animEndTimeMoon;


        float m_minLeafBunchScale;
        float m_maxLeafBunchScale;
        Moon *m_moon;
        std::vector<ScrollMod*> m_leafBunches;
    };
}
