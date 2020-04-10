#include "MemorySetCartesian2D.h"

using namespace payload;

void MemorySetCartesian2D::OnCreate()
{

}

void MemorySetCartesian2D::OnDelete()
{

}

orxBOOL MemorySetCartesian2D::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void MemorySetCartesian2D::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void MemorySetCartesian2D::Reconfigure(std::vector<std::vector<Tile*>> &_tileRows)
{

}
