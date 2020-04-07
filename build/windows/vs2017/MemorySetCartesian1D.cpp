#include "MemorySetCartesian1D.h"

using namespace payload;

void MemorySetCartesian1D::OnCreate()
{

}

void MemorySetCartesian1D::OnDelete()
{

}

orxBOOL MemorySetCartesian1D::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void MemorySetCartesian1D::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void MemorySetCartesian1D::Reconfigure()
{

}
