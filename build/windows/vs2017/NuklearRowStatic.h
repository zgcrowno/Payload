#pragma once

#include "Payload.h"
#include "NuklearLayoutRow.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearRowStatic : public NuklearLayoutRow
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
        //! The width of the NuklearRowStatic's elements.
        float m_elementWidth;
    };
}