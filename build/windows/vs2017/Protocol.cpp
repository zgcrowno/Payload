#include "Protocol.h"

using namespace payload;

void Protocol::OnCreate()
{
    TileInhabitant::OnCreate();
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

void Protocol::Cohabitate(TileInhabitant *_cohabitant)
{

}
