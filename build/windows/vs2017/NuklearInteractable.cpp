#include "NuklearInteractable.h"

using namespace payload;

void NuklearInteractable::OnCreate()
{
    NuklearWindowElement::OnCreate();
}

void NuklearInteractable::OnDelete()
{

}

orxBOOL NuklearInteractable::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearInteractable::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void NuklearInteractable::Interact()
{

}
