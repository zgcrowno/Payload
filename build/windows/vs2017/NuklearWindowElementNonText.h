#pragma once

#include "Payload.h"
#include "NuklearWindowElement.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearWindowElementNonText : public NuklearWindowElement
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
        bool m_bUsesCustomSkin;
        int m_skinIndex;
    };
}
