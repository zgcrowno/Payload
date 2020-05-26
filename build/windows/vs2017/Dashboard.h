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
        //! At what passage of time (in seconds) does the Dashboard start scrolling?
        float m_animStartTimeScroll;
        //! At what passage of time (in seconds) does the Dashboard stop scrolling?
        float m_animEndTimeScroll;
        //! The Dashboard's starting position.
        orxVECTOR m_startingPosition;
        //! The Dashboard's ending position.
        orxVECTOR m_endingPosition;
        //! The minimum and maximum scales of the Dashboard's leaf bunches.
        float m_minLeafBunchScale;
        float m_maxLeafBunchScale;
        //! The Dashboard's Moon.
        Moon *m_moon;
        //! The Dashboard's leaf bunches.
        std::vector<ScrollMod*> m_leafBunches;

        void FinalizeLeafBunches();
        void FinalizeMoon();
        void FinalizeScroll();
    };
}
