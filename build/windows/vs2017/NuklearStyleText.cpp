#include "NuklearStyleText.h"

using namespace payload;

void NuklearStyleText::OnCreate()
{
    m_textColor = GetVector("TextColor", GetModelName());
    m_textAlpha = GetU32("TextAlpha", GetModelName());
    m_padding = GetVector("Padding", GetModelName());
}

void NuklearStyleText::OnDelete()
{

}

orxBOOL NuklearStyleText::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearStyleText::Update(const orxCLOCK_INFO &_rstInfo)
{

}
