#include "NuklearText.h"

using namespace payload;

void NuklearText::OnCreate()
{
    NuklearWindowElement::OnCreate();

    m_staticContent = GetString("StaticContent", GetModelName());
    m_wrap = GetBool("Wrap", GetModelName());
    m_textColor = GetVector("TextColor", GetModelName());
    m_textAlpha = GetFloat("TextAlpha", GetModelName());
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
