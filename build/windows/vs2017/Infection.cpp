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
    // Only cohabitate if _other isn't a Virus or Infection.
    if (dynamic_cast<Virus*>(_other) == nullptr && dynamic_cast<Infection*>(_other) == nullptr)
    {
        if (_other->m_bIsUndoing)
        {
            _other->Undo();
        }
        else
        {
            _other->Die();
        }
    }
}
