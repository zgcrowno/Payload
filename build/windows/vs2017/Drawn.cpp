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
    //orxThread_RunTask(&ExecuteDraw, nullptr, nullptr, this);
    Draw();
}

void Drawn::Draw()
{
    
}

//orxSTATUS orxFASTCALL Drawn::ExecuteDraw(void *_context)
//{
//    Drawn *drawn = static_cast<Drawn*>(_context);
//    drawn->Draw();
//
//    return orxSTATUS_SUCCESS;
//}
