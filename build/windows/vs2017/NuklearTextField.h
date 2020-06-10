#pragma once

#include "Payload.h"
#include "NuklearWindowElement.h"
#include "NuklearTextFieldFilter.h"
#include "NuklearStyleTextField.h"

namespace payload
{
    // A UI window within which rows of UI elements are displayed.
    class NuklearTextField : public NuklearWindowElement
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
        std::string m_content;
        int m_maxLength;
        NuklearTextFieldFilter m_filter;
        //! Text field flags.
        bool m_bReadOnly;
        bool m_bAutoSelect;
        bool m_bSigEnter;
        bool m_bAllowTab;
        bool m_bNoCursor;
        bool m_bSelectable;
        bool m_bClipboard;
        bool m_bControlEnterNewline;
        bool m_bNoHorizontalScroll;
        bool m_bAlwaysInsertMode;
        bool m_bMultiline;
        bool m_bGoToEndOnActivate;
        //! The NuklearTextField's style attribute.
        NuklearStyleTextField *m_style;
    };
}
