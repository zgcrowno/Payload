#pragma once

#include "Payload.h"
#include "NuklearInteractable.h"
#include "NuklearSymbolType.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearCombo : public NuklearInteractable
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
        //! The size of the NuklearCombo.
        orxVECTOR m_comboSize;
        //! The height of each element.
        int m_elementHeight;
        //! The index of the currently selected element.
        int m_selectedIndex;
        NuklearSymbolType m_symbolNormal;
        NuklearSymbolType m_symbolHover;
        NuklearSymbolType m_symbolActive;
        orxVECTOR m_symbolBackgroundColorNormal;
        int m_symbolBackgroundAlphaNormal;
        orxVECTOR m_symbolBackgroundColorHover;
        int m_symbolBackgroundAlphaHover;
        orxVECTOR m_symbolBackgroundColorActive;
        int m_symbolBackgroundAlphaActive;
        orxVECTOR m_backgroundColorNormal;
        int m_backgroundAlphaNormal;
        orxVECTOR m_backgroundColorHover;
        int m_backgroundAlphaHover;
        orxVECTOR m_backgroundColorActive;
        int m_backgroundAlphaActive;
        orxVECTOR m_textColorNormal;
        int m_textAlphaNormal;
        orxVECTOR m_textColorHover;
        int m_textAlphaHover;
        orxVECTOR m_textColorActive;
        int m_textAlphaActive;
        //! The NuklearCombo's elements (strings).
        std::vector<const char*> m_elements;

        virtual void Interact();
    };
}
