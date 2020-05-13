#include "NuklearButton.h"

using namespace payload;

void NuklearButton::OnCreate()
{
    NuklearWindowElement::OnCreate();
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
