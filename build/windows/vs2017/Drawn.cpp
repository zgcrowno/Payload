#include "Drawn.h"

using namespace payload;

void Drawn::OnCreate()
{
    
}

void Drawn::OnDelete()
{

}

orxBOOL Drawn::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Drawn::Update(const orxCLOCK_INFO &_rstInfo)
{
    Draw();
}

void Drawn::Draw()
{
    
}
