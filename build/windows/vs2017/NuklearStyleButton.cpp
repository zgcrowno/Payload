#include "NuklearStyleButton.h"

using namespace payload;

void NuklearStyleButton::OnCreate()
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
    // Text attributes.
    m_textBackgroundColor = GetVector("TextBackgroundColor", GetModelName());
    m_textBackgroundAlpha = GetU32("TextBackgroundAlpha", GetModelName());
    m_textNormalColor = GetVector("TextNormalColor", GetModelName());
    m_textNormalAlpha = GetU32("TextNormalAlpha", GetModelName());
    m_textHoverColor = GetVector("TextHoverColor", GetModelName());
    m_textHoverAlpha = GetU32("TextHoverAlpha", GetModelName());
    m_textActiveColor = GetVector("TextActiveColor", GetModelName());
    m_textActiveAlpha = GetU32("TextActiveAlpha", GetModelName());
    // Properties.
    m_borderThickness = GetFloat("BorderThickness", GetModelName());
    m_borderRounding = GetFloat("BorderRounding", GetModelName());
    m_padding = GetVector("Padding", GetModelName());
    m_imagePadding = GetVector("ImagePadding", GetModelName());
    m_touchPadding = GetVector("TouchPadding", GetModelName());
}

void NuklearStyleButton::OnDelete()
{

}

orxBOOL NuklearStyleButton::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearStyleButton::Update(const orxCLOCK_INFO &_rstInfo)
{

}
