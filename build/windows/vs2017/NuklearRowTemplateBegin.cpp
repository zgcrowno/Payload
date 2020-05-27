#include "NuklearRowTemplateBegin.h"

using namespace payload;

void NuklearRowTemplateBegin::OnCreate()
{
    NuklearLayoutRow::OnCreate();

    for (int i = 0; i < GetListCount("ElementWidths", GetModelName()); i++)
    {
        m_elementWidths.push_back(GetListFloat("ElementWidths", i, GetModelName()));
    }
    for (int i = 0; i < GetListCount("ElementMinWidths", GetModelName()); i++)
    {
        m_elementMinWidths.push_back(GetListFloat("ElementMinWidths", i, GetModelName()));
    }
}

void NuklearRowTemplateBegin::OnDelete()
{

}

orxBOOL NuklearRowTemplateBegin::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearRowTemplateBegin::Update(const orxCLOCK_INFO &_rstInfo)
{

}
