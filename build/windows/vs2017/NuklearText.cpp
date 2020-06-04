#include "NuklearText.h"

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
    for (int i = 0; i < GetListCount("AlignmentFlags", GetModelName()); i++)
    {
        if (!orxString_Compare("Left", GetListString("AlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bIsAlignedLeft = true;
        }
        else if (!orxString_Compare("Right", GetListString("AlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bIsAlignedRight = true;
        }
        else if (!orxString_Compare("Top", GetListString("AlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bIsAlignedTop = true;
        }
        else if (!orxString_Compare("Bottom", GetListString("AlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bIsAlignedBottom = true;
        }
        else if (!orxString_Compare("CenteredHorizontal", GetListString("AlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bIsAlignedCenteredHorizontal = true;
        }
        else if (!orxString_Compare("CenteredVertical", GetListString("AlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bIsAlignedCenteredVertical = true;
        }
    }
    m_style = static_cast<NuklearStyleText*>(CreateChild(GetString("Style", GetModelName())));
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
