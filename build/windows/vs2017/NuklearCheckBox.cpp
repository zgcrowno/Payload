#include "NuklearCheckBox.h"

using namespace payload;

void NuklearCheckBox::OnCreate()
{
    NuklearInteractable::OnCreate();

    m_active = GetBool("Active", GetModelName());
    m_label = GetString("Label", GetModelName());
}

void NuklearCheckBox::OnDelete()
{

}

orxBOOL NuklearCheckBox::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearCheckBox::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void NuklearCheckBox::Interact()
{

}
