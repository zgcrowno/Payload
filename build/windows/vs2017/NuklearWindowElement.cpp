#include "NuklearWindowElement.h"

using namespace payload;

void NuklearWindowElement::OnCreate()
{
    m_type = static_cast<NuklearWindowElementType>(GetU32("Type", GetModelName()));
}

void NuklearWindowElement::OnDelete()
{

}

orxBOOL NuklearWindowElement::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearWindowElement::Update(const orxCLOCK_INFO &_rstInfo)
{

}
