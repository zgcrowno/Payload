#include "NuklearLayoutRow.h"

using namespace payload;

void NuklearLayoutRow::OnCreate()
{
    m_bIsDynamic = GetBool("IsDynamic", GetModelName());
    m_height = GetFloat("Height", GetModelName());
    m_elementWidth = GetFloat("ElementWidth", GetModelName());
    m_numCols = GetU32("NumCols", GetModelName());
    for (ScrollObject *child = GetOwnedChild(); child != nullptr; child = child->GetOwnedSibling())
    {
        NuklearWindowElement *ele = dynamic_cast<NuklearWindowElement*>(child);
        if (ele != nullptr)
        {
            m_elements.push_back(ele);
        }
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
