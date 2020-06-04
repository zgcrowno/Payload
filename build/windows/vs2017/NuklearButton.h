#pragma once

#include "Payload.h"
#include "NuklearInteractable.h"
#include "NuklearStyleButton.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearButton : public NuklearInteractable
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
        bool m_bHasText;
        //! The NuklearButton's text.
        std::string m_buttonText;
        //! The NuklearButton's text alignment flags.
        bool m_bTextIsAlignedLeft;
        bool m_bTextIsAlignedRight;
        bool m_bTextIsAlignedTop;
        bool m_bTextIsAlignedBottom;
        bool m_bTextIsAlignedCenteredHorizontal;
        bool m_bTextIsAlignedCenteredVertical;
        //! The NuklearButton's style attribute.
        NuklearStyleButton *m_style;

        virtual void Interact();
    };
}
