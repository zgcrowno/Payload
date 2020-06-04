#include "NuklearWindowElementNonText.h"

using namespace payload;

void NuklearWindowElementNonText::OnCreate()
{
    NuklearWindowElement::OnCreate();

    for (int i = 0; i < GetListCount("SkinList", "Nuklear"); i++)
    {
        std::string str = GetListString("SkinList", i, "Nuklear");
        if (!str.compare(GetString("Skin", GetModelName())))
        {
            m_bUsesCustomSkin = true;
            m_skinIndex = i;
            break;
        }
    }
}

void NuklearWindowElementNonText::OnDelete()
{

}

orxBOOL NuklearWindowElementNonText::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearWindowElementNonText::Update(const orxCLOCK_INFO &_rstInfo)
{

}
