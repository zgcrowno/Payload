#pragma once

#include "Payload.h"
#include "NuklearWindowElement.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearText : public NuklearWindowElement
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
        int m_fontIndex;
        std::string m_staticContent;
        bool m_wrap;
        orxVECTOR m_textColor;
        float m_textAlpha;
        //! Text alignment flags.
        bool m_bIsAlignedBottom;
        bool m_bIsAlignedCenteredHorizontal;
        bool m_bIsAlignedLeft;
        bool m_bIsAlignedCenteredVertical;
        bool m_bIsAlignedRight;
        bool m_bIsAlignedTop;
    };
}
