#include "Firewall.h"

using namespace payload;

void Firewall::OnCreate()
{
    TileInhabitant::OnCreate();
}

void Firewall::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Firewall::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Firewall::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Firewall::Cohabitate(const bool _tileSetIs2D, const bool _dueToShifting)
{

}
