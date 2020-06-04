#pragma once

#include "Payload.h"
#include "ScrollMod.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearStyleButton : public ScrollMod
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
        // Background attributes.
        orxVECTOR m_normalColor;
        int m_normalAlpha;
        orxVECTOR m_hoverColor;
        int m_hoverAlpha;
        orxVECTOR m_activeColor;
        int m_activeAlpha;
        orxVECTOR m_borderColor;
        int m_borderAlpha;
        // Text attributes.
        orxVECTOR m_textBackgroundColor;
        int m_textBackgroundAlpha;
        orxVECTOR m_textNormalColor;
        int m_textNormalAlpha;
        orxVECTOR m_textHoverColor;
        int m_textHoverAlpha;
        orxVECTOR m_textActiveColor;
        int m_textActiveAlpha;
        // Properties.
        float m_borderThickness;
        float m_borderRounding;
        orxVECTOR m_padding;
        orxVECTOR m_imagePadding;
        orxVECTOR m_touchPadding;
    };
}
