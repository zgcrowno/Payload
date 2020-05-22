#include "NuklearWindow.h"

using namespace payload;

void NuklearWindow::OnCreate()
{
    NuklearWindowElement::OnCreate();

    m_title = GetString("Title", GetModelName());
    m_backgroundColor = GetVector("BackgroundColor", GetModelName());
    m_backgroundAlpha = GetU32("BackgroundAlpha", GetModelName());
    m_borderColor = GetVector("BorderColor", GetModelName());
    m_borderAlpha = GetU32("BorderAlpha", GetModelName());
    m_bIsBordered = GetBool("IsBordered", GetModelName());
    m_bIsMovable = GetBool("IsMovable", GetModelName());
    m_bIsScalable = GetBool("IsScalable", GetModelName());
    m_bIsClosable = GetBool("IsClosable", GetModelName());
    m_bIsMinimizable = GetBool("IsMinimizable", GetModelName());
    m_bHasNoScrollbar = GetBool("HasNoScrollbar", GetModelName());
    m_bHasTitle = GetBool("HasTitle", GetModelName());
    m_bAutoHidesScrollbar = GetBool("AutoHidesScrollbar", GetModelName());
    m_bIsKeptInBackground = GetBool("IsKeptInBackground", GetModelName());
    m_bIsScaledLeft = GetBool("IsScaledLeft", GetModelName());
    m_bDisallowsInput = GetBool("DisallowsInput", GetModelName());
    for (ScrollObject *child = GetOwnedChild(); child != nullptr; child = child->GetOwnedSibling())
    {
        NuklearLayoutRow *layoutRow = dynamic_cast<NuklearLayoutRow*>(child);
        if (layoutRow != nullptr)
        {
            m_layoutRows.push_back(layoutRow);
        }
    }
}

void NuklearWindow::OnDelete()
{
    
}

orxBOOL NuklearWindow::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearWindow::Update(const orxCLOCK_INFO &_rstInfo)
{
    
}
