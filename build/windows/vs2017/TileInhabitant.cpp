#include "TileInhabitant.h"

using namespace payload;

void TileInhabitant::OnCreate()
{
    m_precedence = GetU32("Precedence", GetModelName());
    m_timeToMove = GetFloat("TimeToMove", GetModelName());
    m_tileRatio = GetFloat("TileRatio", GetModelName());
}

void TileInhabitant::OnDelete()
{

}

orxBOOL TileInhabitant::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void TileInhabitant::Update(const orxCLOCK_INFO &_rstInfo)
{
    if (m_target != nullptr)
    {
        // TARGET SCALING
        float minAxis = orxMIN(m_target->m_visualScale.fX, m_target->m_visualScale.fY);
        float scaleRatio = minAxis * m_tileRatio;
        orxVECTOR targetScale = m_target->GetScale();
        SetScale({ targetScale.fX * scaleRatio, targetScale.fY * scaleRatio });
        // TARGET MOVEMENT
        if (m_bIsMoving)
        {
            // Increment the time spent moving.
            m_timeSpentMoving += _rstInfo.fDT;
            float lerpWeight = m_timeSpentMoving / m_timeToMove;

            if (m_timeSpentMoving < m_timeToMove)
            {
                orxVECTOR pos;

                if (m_target->m_bCartesian)
                {
                    orxVector_Lerp(&pos, &m_priorPos, &m_target->GetPosition(), lerpWeight);
                    SetPosition(pos);
                }
                else
                {
                    orxVECTOR targetPos = m_target->GetPosition();
                    float radialDistance = orxLERP(
                        orxVector_GetDistance(&m_priorPos, &m_tileSetPos),
                        orxVector_GetDistance(&m_tileSetPos, &targetPos),
                        lerpWeight);
                    float theta = LerpAngle(
                        CartesianToPolar(m_priorPos, m_tileSetPos).fY,
                        CartesianToPolar(targetPos, m_tileSetPos).fY,
                        lerpWeight);
                    SetPolarPosition(m_tileSetPos, radialDistance, theta);
                }
            }
            else
            {
                m_bIsMoving = false;
                m_timeSpentMoving = 0.0f;
                SetPosition(m_target->GetPosition());
                m_priorPos = GetPosition();
            }
        }
    }
}

void TileInhabitant::Cohabitate(TileInhabitant *_cohabitant)
{

}

void TileInhabitant::SetTarget(Tile *_target)
{
    m_target = _target;
    m_bIsMoving = true;
}
