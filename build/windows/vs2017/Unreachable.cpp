#include "Unreachable.h"
#include <iostream>

using namespace payload;

void Unreachable::OnCreate()
{
    TileInhabitant::OnCreate();
}

void Unreachable::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Unreachable::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Unreachable::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Unreachable::Cohabitate(const bool _tileSetIs2D, const bool _dueToShifting)
{
    for (ScrollObject *tileInhabitant : Payload::GetInstance().GetTileInhabitants())
    {
        TileInhabitant *ti = static_cast<TileInhabitant*>(tileInhabitant);
        bool cohabitating =
            _tileSetIs2D ?
            ti != this && ti->m_target->m_row == m_target->m_row && ti->m_target->m_col == m_target->m_col && ti->IsCohabitable() :
            ti != this && ti->m_target->m_col == m_target->m_col && ti->IsCohabitable();
        if (cohabitating)
        {
            // Only execute Unreachable's Cohabitate method if the interacting TileInhabitant has a lower precedence and the cohabitation is not due to shifting.
            if (ti->m_precedence < m_precedence && !_dueToShifting)
            {
                ti->Undo();
            }
            else if (ti->m_precedence < m_precedence)
            {
                // TODO: Account for situation in which Unreachable is shifted onto another TileInhabitant in 1D (probably broadcast an event to which TileSet must respond).
            }
        }
    }
}
