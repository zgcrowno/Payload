#include "NuklearCombo.h"

using namespace payload;

void NuklearCombo::OnCreate()
{
    NuklearInteractable::OnCreate();

    for (int i = 0; i < GetListCount("FontList", "Nuklear"); i++)
    {
        std::string str = GetListString("FontList", i, "Nuklear");
        if (!str.compare(GetString("ComboFont", GetModelName())))
        {
            m_fontIndex = i;
            break;
        }
    }
    m_comboSize = GetVector("ComboSize", GetModelName());
    m_elementHeight = GetFloat("ElementHeight", GetModelName());
    m_selectedIndex = GetU32("SelectedIndex", GetModelName());
    for (int i = 0; i < GetListCount("Elements", GetModelName()); i++)
    {
        orxConfig_PushSection(GetModelName().c_str());
        const char *ele = orxConfig_GetListString("Elements", i);
        orxConfig_PopSection();

        m_elements.push_back(ele);
    }
    m_style = static_cast<NuklearStyleCombo*>(CreateChild(GetString("Style", GetModelName())));
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
