#include "Recursive.h"

using namespace payload;

void Recursive::OnCreate()
{
    TileInhabitant::OnCreate();
}

void Recursive::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Recursive::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Recursive::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Recursive::Cohabitate(TileInhabitant *_cohabitant)
{

}
