#include "NuklearRowSpaceBegin.h"

using namespace payload;

void NuklearRowSpaceBegin::OnCreate()
{
    NuklearLayoutRow::OnCreate();

    for (int i = 0; i < GetListCount("ElementStartingPositions", GetModelName()); i++)
    {
        m_elementStartingPositions.push_back(GetListVector("ElementStartingPositions", i, GetModelName()));
    }
    for (int i = 0; i < GetListCount("ElementStartingSizes", GetModelName()); i++)
    {
        m_elementStartingSizes.push_back(GetListVector("ElementStartingSizes", i, GetModelName()));
    }
}

void NuklearRowSpaceBegin::OnDelete()
{

}

orxBOOL NuklearRowSpaceBegin::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearRowSpaceBegin::Update(const orxCLOCK_INFO &_rstInfo)
{

}
