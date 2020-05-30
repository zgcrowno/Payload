#include "NuklearButton.h"

using namespace payload;

void NuklearButton::OnCreate()
{
    NuklearInteractable::OnCreate();

    for (int i = 0; i < GetListCount("FontList", "Nuklear"); i++)
    {
        std::string str = GetListString("FontList", i, "Nuklear");
        if (!str.compare(GetString("ButtonFont", GetModelName())))
        {
            m_fontIndex = i;
            break;
        }
    }
    m_buttonText = GetListString("ButtonTextAndAlignmentFlags", 0, GetModelName());
    m_bHasText = m_buttonText.length() > 0;
    for (int i = 1; i < GetListCount("ButtonTextAndAlignmentFlags", GetModelName()); i++)
    {
        if (!orxString_Compare("Left", GetListString("ButtonTextAndAlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bTextIsAlignedLeft = true;
        }
        else if (!orxString_Compare("Right", GetListString("ButtonTextAndAlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bTextIsAlignedRight = true;
        }
        else if (!orxString_Compare("Top", GetListString("ButtonTextAndAlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bTextIsAlignedTop = true;
        }
        else if (!orxString_Compare("Bottom", GetListString("ButtonTextAndAlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bTextIsAlignedBottom = true;
        }
        else if (!orxString_Compare("CenteredHorizontal", GetListString("ButtonTextAndAlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bTextIsAlignedCenteredHorizontal = true;
        }
        else if (!orxString_Compare("CenteredVertical", GetListString("ButtonTextAndAlignmentFlags", i, GetModelName()).c_str()))
        {
            m_bTextIsAlignedCenteredVertical = true;
        }
    }
}

void NuklearButton::OnDelete()
{

}

orxBOOL NuklearButton::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearButton::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void NuklearButton::Interact()
{

}
