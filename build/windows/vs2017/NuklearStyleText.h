#pragma once

#include "Payload.h"
#include "ScrollMod.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearStyleText : public ScrollMod
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
        orxVECTOR m_textColor;
        int m_textAlpha;
        orxVECTOR m_padding;
    };
}
