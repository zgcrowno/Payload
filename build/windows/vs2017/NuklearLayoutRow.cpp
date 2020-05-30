#include "NuklearLayoutRow.h"

using namespace payload;

void NuklearLayoutRow::OnCreate()
{
    NuklearWindowElement::OnCreate();

    m_format = static_cast<NuklearLayoutFormat>(GetU32("Format", GetModelName()));
    m_height = GetFloat("Height", GetModelName());
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
