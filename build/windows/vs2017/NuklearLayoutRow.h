#pragma once

#include "Payload.h"
#include "ScrollMod.h"
#include "NuklearWindowElement.h"
#include "NuklearLayoutType.h"
#include "NuklearLayoutFormat.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearLayoutRow : public ScrollMod
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
        //! What type of layout row is the NuklearLayoutRow?
        NuklearLayoutType m_type;
        //! To what type of layout format does the NuklearLayoutRow subscribe?
        NuklearLayoutFormat m_format;
        //! The height of the NuklearLayoutRow.
        float m_height;
        //! The width of the NuklearLayoutRow's elements, should the row be static.
        float m_elementWidth;
        //! The elements which comprise the NuklearLayoutRow.
        std::vector<NuklearWindowElement*> m_elements;
        //! Either the static width/height or dynamic width/height window ratio of the NuklearLayoutRow's elements.
        std::vector<float> m_sizeOrRatio;
        //! Either the static widths or dynamic width window ratios of the NuklearLayoutRow's elements.
        std::vector<float> m_widthsOrRatios;
        //! The static widths of the NuklearLayoutRow's elements.
        std::vector<float> m_elementWidths;
        //! The minimum widths of the NuklearLayoutRow's elements.
        std::vector<float> m_elementMinWidths;
        //! The starting positions of the NuklearLayoutRow's elements.
        std::vector<orxVECTOR> m_elementStartingPositions;
        //! The starting sizes of the NuklearLayoutRow's elements.
        std::vector<orxVECTOR> m_elementStartingSizes;
    };
}
