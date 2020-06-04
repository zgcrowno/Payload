#pragma once

#include "Payload.h"
#include "ScrollMod.h"
#include "NuklearStyleButton.h"
#include "NuklearSymbolType.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearStyleCombo : public ScrollMod
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
        // Label attributes.
        orxVECTOR m_labelNormalColor;
        int m_labelNormalAlpha;
        orxVECTOR m_labelHoverColor;
        int m_labelHoverAlpha;
        orxVECTOR m_labelActiveColor;
        int m_labelActiveAlpha;
        // Symbol attributes.
        orxVECTOR m_symbolNormalColor;
        int m_symbolNormalAlpha;
        orxVECTOR m_symbolHoverColor;
        int m_symbolHoverAlpha;
        orxVECTOR m_symbolActiveColor;
        int m_symbolActiveAlpha;
        // Button attributes.
        NuklearStyleButton *m_buttonStyle;
        NuklearSymbolType m_symbolNormal;
        NuklearSymbolType m_symbolHover;
        NuklearSymbolType m_symbolActive;
        // Properties.
        float m_borderThickness;
        float m_borderRounding;
        orxVECTOR m_contentPadding;
        orxVECTOR m_buttonPadding;
        orxVECTOR m_spacing;
    };
}
