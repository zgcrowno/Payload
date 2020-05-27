#include "NuklearRowBegin.h"

using namespace payload;

void NuklearRowBegin::OnCreate()
{
    NuklearLayoutRow::OnCreate();

    for (int i = 0; i < GetListCount("WidthsOrRatios", GetModelName()); i++)
    {
        m_widthsOrRatios.push_back(GetListFloat("WidthsOrRatios", i, GetModelName()));
    }
}

void NuklearRowBegin::OnDelete()
{

}

orxBOOL NuklearRowBegin::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearRowBegin::Update(const orxCLOCK_INFO &_rstInfo)
{

}
