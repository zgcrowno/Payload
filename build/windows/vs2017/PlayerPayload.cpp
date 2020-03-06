#include "PlayerPayload.h"

using namespace payload;

void PlayerPayload::OnCreate()
{

}

void PlayerPayload::OnDelete()
{

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

}
