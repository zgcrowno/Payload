#include "NuklearStyleTextField.h"
#include <iostream>

using namespace payload;

void NuklearStyleTextField::OnCreate()
{
    // Background.
    m_normalColor = GetVector("NormalColor", GetModelName());
    m_normalAlpha = GetFloat("NormalAlpha", GetModelName());
    m_hoverColor = GetVector("HoverColor", GetModelName());
    m_hoverAlpha = GetFloat("HoverAlpha", GetModelName());
    m_activeColor = GetVector("ActiveColor", GetModelName());
    m_activeAlpha = GetFloat("ActiveAlpha", GetModelName());
    m_borderColor = GetVector("BorderColor", GetModelName());
    m_borderAlpha = GetFloat("BorderAlpha", GetModelName());
    // Cursor.
    m_cursorNormalColor = GetVector("CursorNormalColor", GetModelName());
    m_cursorNormalAlpha = GetFloat("CursorNormalAlpha", GetModelName());
    m_cursorHoverColor = GetVector("CursorHoverColor", GetModelName());
    m_cursorHoverAlpha = GetFloat("CursorHoverAlpha", GetModelName());
    m_cursorTextNormalColor = GetVector("CursorTextNormalColor", GetModelName());
    m_cursorTextNormalAlpha = GetFloat("CursorTextNormalAlpha", GetModelName());
    m_cursorTextHoverColor = GetVector("CursorTextHoverColor", GetModelName());
    m_cursorTextHoverAlpha = GetFloat("CursorTextHoverAlpha", GetModelName());
    // Text (unselected).
    m_textNormalColor = GetVector("TextNormalColor", GetModelName());
    m_textNormalAlpha = GetFloat("TextNormalAlpha", GetModelName());
    m_textHoverColor = GetVector("TextHoverColor", GetModelName());
    m_textHoverAlpha = GetFloat("TextHoverAlpha", GetModelName());
    m_textActiveColor = GetVector("TextActiveColor", GetModelName());
    m_textActiveAlpha = GetFloat("TextActiveAlpha", GetModelName());
    // Text (selected).
    m_selectedNormalColor = GetVector("SelectedNormalColor", GetModelName());
    m_selectedNormalAlpha = GetFloat("SelectedNormalAlpha", GetModelName());
    m_selectedHoverColor = GetVector("SelectedHoverColor", GetModelName());
    m_selectedHoverAlpha = GetFloat("SelectedHoverAlpha", GetModelName());
    m_selectedTextNormalColor = GetVector("SelectedTextNormalColor", GetModelName());
    m_selectedTextNormalAlpha = GetFloat("SelectedTextNormalAlpha", GetModelName());
    m_selectedTextHoverColor = GetVector("SelectedTextHoverColor", GetModelName());
    m_selectedTextHoverAlpha = GetFloat("SelectedTextHoverAlpha", GetModelName());
    // Properties.
    m_borderThickness = GetFloat("BorderThickness", GetModelName());
    m_borderRounding = GetFloat("BorderRounding", GetModelName());
    m_cursorSize = GetFloat("CursorSize", GetModelName());
    m_scrollbarSize = GetVector("ScrollbarSize", GetModelName());
    m_padding = GetVector("Padding", GetModelName());
    m_rowPadding = GetFloat("RowPadding", GetModelName());
    std::cout << m_textNormalColor.fR << std::endl;
}

void NuklearStyleTextField::OnDelete()
{

}

orxBOOL NuklearStyleTextField::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearStyleTextField::Update(const orxCLOCK_INFO &_rstInfo)
{

}
