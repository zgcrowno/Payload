#include "NuklearWindow.h"

using namespace payload;

void NuklearWindow::OnCreate()
{
    NuklearWindowElementNonText::OnCreate();

    m_title = GetString("Title", GetModelName());
    for (int i = 0; i < GetListCount("WindowFlags", GetModelName()); i++)
    {
        if (!orxString_Compare("IsBordered", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bIsBordered = true;
        }
        else if (!orxString_Compare("IsMovable", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bIsMovable = true;
        }
        else if (!orxString_Compare("IsScalable", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bIsScalable = true;
        }
        else if (!orxString_Compare("IsClosable", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bIsClosable = true;
        }
        else if (!orxString_Compare("IsMinimizable", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bIsMinimizable = true;
        }
        else if (!orxString_Compare("HasNoScrollbar", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bHasNoScrollbar = true;
        }
        else if (!orxString_Compare("HasTitle", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bHasTitle = true;
        }
        else if (!orxString_Compare("AutoHidesScrollbar", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bAutoHidesScrollbar = true;
        }
        else if (!orxString_Compare("IsKeptInBackground", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bIsKeptInBackground = true;
        }
        else if (!orxString_Compare("IsScaledLeft", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bIsScaledLeft = true;
        }
        else if (!orxString_Compare("DisallowsInput", GetListString("WindowFlags", i, GetModelName()).c_str()))
        {
            m_bDisallowsInput = true;
        }
    }
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
