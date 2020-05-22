#include "Moon.h"

using namespace payload;

void Moon::OnCreate()
{
    m_mutableCircleX = GetFloat("MutableCircleX", GetModelName());
    m_mutableCircleXStart = GetFloat("MutableCircleXStart", GetModelName());
    m_waning = GetBool("Waning", GetModelName());
}

void Moon::OnDelete()
{

}

orxBOOL Moon::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

orxBOOL Moon::OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload)
{
    if (!orxString_Compare(_rstPayload.zParamName, "MutableCircleX"))
    {
        _rstPayload.fValue = m_mutableCircleX;
    }
    else if (!orxString_Compare(_rstPayload.zParamName, "Waning"))
    {
        _rstPayload.fValue = m_waning;
    }

    return orxTRUE;
}

void Moon::Update(const orxCLOCK_INFO &_rstInfo)
{
    
}
