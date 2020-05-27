#include "NuklearRowDynamic.h"

using namespace payload;

void NuklearRowDynamic::OnCreate()
{
    NuklearLayoutRow::OnCreate();
}

void NuklearRowDynamic::OnDelete()
{

}

orxBOOL NuklearRowDynamic::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearRowDynamic::Update(const orxCLOCK_INFO &_rstInfo)
{

}
