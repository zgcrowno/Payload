#include "NuklearRowStatic.h"

using namespace payload;

void NuklearRowStatic::OnCreate()
{
    NuklearLayoutRow::OnCreate();

    m_elementWidth = GetFloat("ElementWidth", GetModelName());
}

void NuklearRowStatic::OnDelete()
{

}

orxBOOL NuklearRowStatic::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearRowStatic::Update(const orxCLOCK_INFO &_rstInfo)
{

}
