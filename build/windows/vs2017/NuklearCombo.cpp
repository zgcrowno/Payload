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
    m_symbolNormal = static_cast<NuklearSymbolType>(GetU32("SymbolNormal", GetModelName()));
    m_symbolHover = static_cast<NuklearSymbolType>(GetU32("SymbolHover", GetModelName()));
    m_symbolActive = static_cast<NuklearSymbolType>(GetU32("SymbolActive", GetModelName()));
    m_symbolBackgroundColorNormal = GetVector("SymbolBackgroundColorNormal", GetModelName());
    m_symbolBackgroundAlphaNormal = GetU32("SymbolBackgroundAlphaNormal", GetModelName());
    m_symbolBackgroundColorHover = GetVector("SymbolBackgroundColorHover", GetModelName());
    m_symbolBackgroundAlphaHover = GetU32("SymbolBackgroundAlphaHover", GetModelName());
    m_symbolBackgroundColorActive = GetVector("SymbolBackgroundColorActive", GetModelName());
    m_symbolBackgroundAlphaActive = GetU32("SymbolBackgroundAlphaActive", GetModelName());
    m_backgroundColorNormal = GetVector("BackgroundColorNormal", GetModelName());
    m_backgroundAlphaNormal = GetU32("BackgroundAlphaNormal", GetModelName());
    m_backgroundColorHover = GetVector("BackgroundColorHover", GetModelName());
    m_backgroundAlphaHover = GetU32("BackgroundAlphaHover", GetModelName());
    m_backgroundColorActive = GetVector("BackgroundColorActive", GetModelName());
    m_backgroundAlphaActive = GetU32("BackgroundAlphaActive", GetModelName());
    m_textColorNormal = GetVector("TextColorNormal", GetModelName());
    m_textAlphaNormal = GetU32("TextAlphaNormal", GetModelName());
    m_textColorHover = GetVector("TextColorHover", GetModelName());
    m_textAlphaHover = GetU32("TextAlphaHover", GetModelName());
    m_textColorActive = GetVector("TextColorActive", GetModelName());
    m_textAlphaActive = GetU32("TextAlphaActive", GetModelName());
    for (int i = 0; i < GetListCount("Elements", GetModelName()); i++)
    {
        orxConfig_PushSection(GetModelName().c_str());
        const char *ele = orxConfig_GetListString("Elements", i);
        orxConfig_PopSection();

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
