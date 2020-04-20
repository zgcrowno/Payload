#include "MemorySet.h"
#include "EventType.h"

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

void MemorySet::SetTiles()
{

}

void MemorySet::Undo()
{
    Reconfigure(true);
}

void MemorySet::Reconfigure(const bool _undoing)
{
    SetTiles();
    if (!_undoing)
    {
        orxEVENT_SEND(EVENT_TYPE_MEMORY_SET, EVENT_MEMORY_SET_RECONFIGURE, this, Payload::GetInstance().GetTileSet(), nullptr);
    }
}
