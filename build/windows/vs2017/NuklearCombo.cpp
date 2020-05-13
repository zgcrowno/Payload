#include "NuklearCombo.h"

using namespace payload;

void NuklearCombo::OnCreate()
{
    NuklearWindowElement::OnCreate();
}

void NuklearCombo::OnDelete()
{

}

orxBOOL NuklearCombo::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearCombo::Update(const orxCLOCK_INFO &_rstInfo)
{

}
