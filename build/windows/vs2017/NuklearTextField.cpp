#include "NuklearTextField.h"

using namespace payload;

void NuklearTextField::OnCreate()
{
    NuklearWindowElement::OnCreate();

    for (int i = 0; i < GetListCount("FontList", "Nuklear"); i++)
    {
        std::string str = GetListString("FontList", i, "Nuklear");
        if (!str.compare(GetString("Font", GetModelName())))
        {
            m_fontIndex = i;
            break;
        }
    }
    m_content = GetString("Content", GetModelName());
    m_maxLength = GetU32("MaxLength", GetModelName());
    m_filter = static_cast<NuklearTextFieldFilter>(GetU32("Filter", GetModelName()));
    for (int i = 0; i < GetListCount("FieldFlags", GetModelName()); i++)
    {
        if (!orxString_Compare("ReadOnly", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bReadOnly = true;
        }
        else if (!orxString_Compare("AutoSelect", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bAutoSelect = true;
        }
        else if (!orxString_Compare("SigEnter", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bSigEnter = true;
        }
        else if (!orxString_Compare("AllowTab", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bAllowTab = true;
        }
        else if (!orxString_Compare("NoCursor", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bNoCursor = true;
        }
        else if (!orxString_Compare("Selectable", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bSelectable = true;
        }
        else if (!orxString_Compare("Clipboard", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bClipboard = true;
        }
        else if (!orxString_Compare("ControlEnterNewline", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bControlEnterNewline = true;
        }
        else if (!orxString_Compare("NoHorizontalScroll", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bNoHorizontalScroll = true;
        }
        else if (!orxString_Compare("AlwaysInsertMode", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bAlwaysInsertMode = true;
        }
        else if (!orxString_Compare("Multiline", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bMultiline = true;
        }
        else if (!orxString_Compare("GoToEndOnActivate", GetListString("FieldFlags", i, GetModelName()).c_str()))
        {
            m_bGoToEndOnActivate = true;
        }
    }
    m_style = static_cast<NuklearStyleTextField*>(CreateChild(GetString("Style", GetModelName())));
}

void NuklearTextField::OnDelete()
{

}

orxBOOL NuklearTextField::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearTextField::Update(const orxCLOCK_INFO &_rstInfo)
{

}
