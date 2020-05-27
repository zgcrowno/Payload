#include "NuklearRow.h"

using namespace payload;

void NuklearRow::OnCreate()
{
    NuklearLayoutRow::OnCreate();

    orxVECTOR sizeOrRatio = GetVector("SizeOrRatio", GetModelName());
    m_sizeOrRatio.push_back(sizeOrRatio.fX);
    m_sizeOrRatio.push_back(sizeOrRatio.fY);
}

void NuklearRow::OnDelete()
{

}

orxBOOL NuklearRow::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearRow::Update(const orxCLOCK_INFO &_rstInfo)
{

}
