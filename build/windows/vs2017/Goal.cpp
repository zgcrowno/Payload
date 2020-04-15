#include "Goal.h"

using namespace payload;

void Goal::OnCreate()
{
    TileInhabitant::OnCreate();
}

void Goal::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Goal::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Goal::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Goal::Cohabitate(const bool _tileSetIs2D, const bool _dueToShifting)
{

}
