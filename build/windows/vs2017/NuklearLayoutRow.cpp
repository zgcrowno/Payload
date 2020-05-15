#include "NuklearLayoutRow.h"

using namespace payload;

void NuklearLayoutRow::OnCreate()
{
    m_type = static_cast<NuklearLayoutType>(GetU32("Type", GetModelName()));
    m_format = static_cast<NuklearLayoutFormat>(GetU32("Format", GetModelName()));
    m_height = GetFloat("Height", GetModelName());
    m_elementWidth = GetFloat("ElementWidth", GetModelName());
    for (ScrollObject *child = GetOwnedChild(); child != nullptr; child = child->GetOwnedSibling())
    {
        NuklearWindowElement *ele = dynamic_cast<NuklearWindowElement*>(child);
        if (ele != nullptr)
        {
            m_elements.push_back(ele);
        }
    }
    orxVECTOR sizeOrRatio = GetVector("SizeOrRatio", GetModelName());
    m_sizeOrRatio.push_back(sizeOrRatio.fX);
    m_sizeOrRatio.push_back(sizeOrRatio.fY);
    for (int i = 0; i < GetListCount("WidthsOrRatios", GetModelName()); i++)
    {
        m_widthsOrRatios.push_back(GetListFloat("WidthsOrRatios", i, GetModelName()));
    }
    for (int i = 0; i < GetListCount("ElementWidths", GetModelName()); i++)
    {
        m_elementWidths.push_back(GetListFloat("ElementWidths", i, GetModelName()));
    }
    for (int i = 0; i < GetListCount("ElementMinWidths", GetModelName()); i++)
    {
        m_elementMinWidths.push_back(GetListFloat("ElementMinWidths", i, GetModelName()));
    }
    for (int i = 0; i < GetListCount("ElementStartingPositions", GetModelName()); i++)
    {
        m_elementStartingPositions.push_back(GetListVector("ElementStartingPositions", i, GetModelName()));
    }
    for (int i = 0; i < GetListCount("ElementStartingSizes", GetModelName()); i++)
    {
        m_elementStartingSizes.push_back(GetListVector("ElementStartingSizes", i, GetModelName()));
    }
}

void NuklearLayoutRow::OnDelete()
{

}

orxBOOL NuklearLayoutRow::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearLayoutRow::Update(const orxCLOCK_INFO &_rstInfo)
{

}
