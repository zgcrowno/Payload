#include "Bypass.h"

using namespace payload;

void Bypass::OnCreate()
{
    TileInhabitant::OnCreate();
}

void Bypass::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Bypass::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Bypass::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Bypass::Cohabitate(TileInhabitant *_other, const bool _dueToShifting)
{
    Tile *otherDest = m_target->GetTileInDirection(2, _other->m_movementDirection);
    if (otherDest != nullptr)
    {
        _other->SetTarget(otherDest, _other->m_movementDirection);
    }
    else
    {
        // TODO: If I implement out-of-bounds Tiles that destroy their associated TileInhabitants,
        // I'll need to change this behavior.
        Tile *alternateOtherDest = m_target->GetTileInDirection(1, _other->m_movementDirection);
        if (alternateOtherDest != nullptr)
        {
            _other->SetTarget(alternateOtherDest, _other->m_movementDirection);
        }
    }
}
