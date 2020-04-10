#include "MemorySet.h"

using namespace payload;

void MemorySet::OnCreate()
{

}

void MemorySet::OnDelete()
{

}

orxBOOL MemorySet::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void MemorySet::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void MemorySet::Reconfigure(std::vector<std::vector<Tile*>> &_tileRows)
{

}
