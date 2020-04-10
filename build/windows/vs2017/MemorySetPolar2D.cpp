#include "MemorySetPolar2D.h"

using namespace payload;

void MemorySetPolar2D::OnCreate()
{

}

void MemorySetPolar2D::OnDelete()
{

}

orxBOOL MemorySetPolar2D::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void MemorySetPolar2D::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void MemorySetPolar2D::Reconfigure(std::vector<std::vector<Tile*>> &_tileRows)
{

}
