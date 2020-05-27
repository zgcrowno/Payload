#include "NuklearButton.h"

using namespace payload;

void NuklearButton::OnCreate()
{
    NuklearInteractable::OnCreate();

    m_buttonText = GetString("ButtonText", GetModelName());
    m_bHasText = m_buttonText.length() > 0;
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
