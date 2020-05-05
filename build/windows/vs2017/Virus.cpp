#include "Virus.h"

using namespace payload;

void Virus::OnCreate()
{
    TileInhabitant::OnCreate();
}

void Virus::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Virus::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Virus::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Virus::Cohabitate(TileInhabitant *_other, const bool _dueToShifting)
{
    // Only cohabitate if _other isn't already infected.
    if (!_other->m_bIsInfected)
    {
        _other->m_bIsPendingInfection = true;
    }
}
