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

void Bypass::Cohabitate(const bool _tileSetIs2D, const bool _dueToShifting)
{

}
