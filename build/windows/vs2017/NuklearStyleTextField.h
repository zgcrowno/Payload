#pragma once

#include "Payload.h"
#include "ScrollMod.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearStyleTextField : public ScrollMod
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
        // Background.
        orxVECTOR m_normalColor;
        float m_normalAlpha;
        orxVECTOR m_hoverColor;
        float m_hoverAlpha;
        orxVECTOR m_activeColor;
        float m_activeAlpha;
        orxVECTOR m_borderColor;
        float m_borderAlpha;
        // Cursor.
        orxVECTOR m_cursorNormalColor;
        float m_cursorNormalAlpha;
        orxVECTOR m_cursorHoverColor;
        float m_cursorHoverAlpha;
        orxVECTOR m_cursorTextNormalColor;
        float m_cursorTextNormalAlpha;
        orxVECTOR m_cursorTextHoverColor;
        float m_cursorTextHoverAlpha;
        // Text (unselected).
        orxVECTOR m_textNormalColor;
        float m_textNormalAlpha;
        orxVECTOR m_textHoverColor;
        float m_textHoverAlpha;
        orxVECTOR m_textActiveColor;
        float m_textActiveAlpha;
        // Text (selected).
        orxVECTOR m_selectedNormalColor;
        float m_selectedNormalAlpha;
        orxVECTOR m_selectedHoverColor;
        float m_selectedHoverAlpha;
        orxVECTOR m_selectedTextNormalColor;
        float m_selectedTextNormalAlpha;
        orxVECTOR m_selectedTextHoverColor;
        float m_selectedTextHoverAlpha;
        // Properties.
        float m_borderThickness;
        float m_borderRounding;
        float m_cursorSize;
        orxVECTOR m_scrollbarSize;
        orxVECTOR m_padding;
        float m_rowPadding;
    };
}
