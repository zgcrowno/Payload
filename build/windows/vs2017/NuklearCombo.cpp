#include "NuklearCombo.h"

using namespace payload;

void NuklearCombo::OnCreate()
{
    NuklearInteractable::OnCreate();

    m_comboSize = GetVector("ComboSize", GetModelName());
    m_elementHeight = GetFloat("ElementHeight", GetModelName());
    m_selectedIndex = GetU32("SelectedIndex", GetModelName());
    for (int i = 0; i < GetListCount("Elements", GetModelName()); i++)
    {
        const char *ele = GetListString("Elements", i, GetModelName()).c_str();
        m_elements.push_back(ele);
    }
}

void NuklearCombo::OnDelete()
{

}

orxBOOL NuklearCombo::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearCombo::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void NuklearCombo::Interact()
{

}
