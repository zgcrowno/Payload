#include "Protocol.h"

using namespace payload;

void Protocol::OnCreate()
{
    TileInhabitant::OnCreate();

    m_direction = static_cast<Direction>(GetU32("Direction", GetModelName()));
}

void Protocol::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Protocol::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Protocol::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Protocol::Cohabitate(TileInhabitant *_other, const bool _dueToShifting)
{
    Tile *otherDest = _other->m_target->GetTileInDirection(1, m_direction);
    if (otherDest != nullptr)
    {
        _other->SetTarget(otherDest, m_direction);
    }
    else
    {
        // TODO: Determine what to do if otherDest is null.
    }
}
