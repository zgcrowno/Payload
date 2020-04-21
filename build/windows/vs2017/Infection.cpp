#include "Infection.h"
#include "Virus.h"

using namespace payload;

void Infection::OnCreate()
{
    TileInhabitant::OnCreate();
}

void Infection::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Infection::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Infection::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Infection::Cohabitate(TileInhabitant *_other, const bool _dueToShifting)
{
    if (dynamic_cast<Virus*>(_other) == nullptr && dynamic_cast<Infection*>(_other) == nullptr)
    {
        _other->Die();
    }
}
