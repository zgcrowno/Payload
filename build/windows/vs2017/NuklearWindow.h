#pragma once

#include "Payload.h"
#include "ScrollMod.h"

namespace payload
{
    class NuklearWindow : public ScrollMod
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
        //! The NuklearWindow's title (to be displayed in its header, if it has a header).
        std::string m_title;
        //! nk_panel_flags bools.
        bool m_bIsBordered;
        bool m_bIsMovable;
        bool m_bIsScalable;
        bool m_bIsClosable;
        bool m_bIsMinimizable;
        bool m_bHasNoScrollbar;
        bool m_bHasTitle;
        bool m_bAutoHidesScrollbar;
        bool m_bIsKeptInBackground;
        bool m_bIsScaledLeft;
        bool m_bDisallowsInput;
    };
}
