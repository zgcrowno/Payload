#include "NuklearStyleCombo.h"

using namespace payload;

void NuklearStyleCombo::OnCreate()
{
    // Background attributes.
    m_normalColor = GetVector("NormalColor", GetModelName());
    m_normalAlpha = GetU32("NormalAlpha", GetModelName());
    m_hoverColor = GetVector("HoverColor", GetModelName());
    m_hoverAlpha = GetU32("HoverAlpha", GetModelName());
    m_activeColor = GetVector("ActiveColor", GetModelName());
    m_activeAlpha = GetU32("ActiveAlpha", GetModelName());
    m_borderColor = GetVector("BorderColor", GetModelName());
    m_borderAlpha = GetU32("BorderAlpha", GetModelName());
    // Label attributes.
    m_labelNormalColor = GetVector("LabelNormalColor", GetModelName());
    m_labelNormalAlpha = GetU32("LabelNormalAlpha", GetModelName());
    m_labelHoverColor = GetVector("LabelHoverColor", GetModelName());
    m_labelHoverAlpha = GetU32("LabelHoverAlpha", GetModelName());
    m_labelActiveColor = GetVector("LabelActiveColor", GetModelName());
    m_labelActiveAlpha = GetU32("LabelActiveAlpha", GetModelName());
    // Symbol attributes.
    m_symbolNormalColor = GetVector("SymbolNormalColor", GetModelName());
    m_symbolNormalAlpha = GetU32("SymbolNormalAlpha", GetModelName());
    m_symbolHoverColor = GetVector("SymbolHoverColor", GetModelName());
    m_symbolHoverAlpha = GetU32("SymbolHoverAlpha", GetModelName());
    m_symbolActiveColor = GetVector("SymbolActiveColor", GetModelName());
    m_symbolActiveAlpha = GetU32("SymbolActiveAlpha", GetModelName());
    // Button attributes.
    m_buttonStyle = static_cast<NuklearStyleButton*>(CreateChild(GetString("ButtonStyle")));
    m_symbolNormal = static_cast<NuklearSymbolType>(GetU32("SymbolNormal", GetModelName()));
    m_symbolHover = static_cast<NuklearSymbolType>(GetU32("SymbolHover", GetModelName()));
    m_symbolActive = static_cast<NuklearSymbolType>(GetU32("SymbolActive", GetModelName()));
    // Properties.
    m_borderThickness = GetFloat("BorderThickness", GetModelName());
    m_borderRounding = GetFloat("BorderRounding", GetModelName());
    m_contentPadding = GetVector("ContentPadding", GetModelName());
    m_buttonPadding = GetVector("ButtonPadding", GetModelName());
    m_spacing = GetVector("Spacing", GetModelName());
}

void NuklearStyleCombo::OnDelete()
{

}

orxBOOL NuklearStyleCombo::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearStyleCombo::Update(const orxCLOCK_INFO &_rstInfo)
{

}
