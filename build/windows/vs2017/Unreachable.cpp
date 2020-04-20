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

void Unreachable::Cohabitate(TileInhabitant *_other, const bool _dueToShifting)
{
    if (!_dueToShifting)
    {
        _other->Undo();
    }
    else
    {
        // TODO: Account for situation in which Unreachable is shifted onto another TileInhabitant in 1D (probably broadcast an event to which TileSet must respond).
    }
}
