#include "Proxy.h"

using namespace payload;

void Proxy::OnCreate()
{
    TileInhabitant::OnCreate();
}

void Proxy::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Proxy::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Proxy::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Proxy::Cohabitate(const bool _tileSetIs2D, const bool _dueToShifting)
{

}
