#include "PlayerPayload.h"

using namespace payload;

void PlayerPayload::OnCreate()
{
    TileInhabitant::OnCreate();
}

void PlayerPayload::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL PlayerPayload::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void PlayerPayload::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void PlayerPayload::Cohabitate(TileInhabitant *_other, const bool _dueToShifting)
{

}
