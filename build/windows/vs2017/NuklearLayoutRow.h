#pragma once

#include "Payload.h"
#include "ScrollMod.h"
#include "NuklearWindowElement.h"

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
        //! Is the NuklearLayoutRow dynamic? (If not, it's static).
        bool m_bIsDynamic;
        //! The height of the NuklearLayoutRow.
        float m_height;
        //! The width of the NuklearLayoutRow's elements, should the row be static.
        float m_elementWidth;
        //! The number of columns contained in the NuklearLayoutRow.
        int m_numCols;
        //! The elements which comprise the NuklearLayoutRow.
        std::vector<NuklearWindowElement*> m_elements;
    };
}
