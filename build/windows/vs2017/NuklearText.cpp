#include "NuklearText.h"
#include <iostream>

using namespace payload;

void NuklearText::OnCreate()
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
    m_staticContent = GetString("StaticContent", GetModelName());
    m_wrap = GetBool("Wrap", GetModelName());
    m_textColor = GetVector("TextColor", GetModelName());
    m_textAlpha = GetFloat("TextAlpha", GetModelName());
    m_alignment = static_cast<NuklearTextAlignment>(GetU32("Alignment", GetModelName()));
}

void NuklearText::OnDelete()
{

}

orxBOOL NuklearText::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearText::Update(const orxCLOCK_INFO &_rstInfo)
{

}
