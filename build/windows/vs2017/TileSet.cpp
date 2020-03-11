#include "TileSet.h"

using namespace payload;

void TileSet::OnCreate()
{
    m_2D = GetBool("2D", GetModelName());
    m_cartesian = GetBool("Cartesian", GetModelName());
    m_square = GetU32("Square", GetModelName());
    m_halfSquare = m_square / 2;
    orxVECTOR parentCameraScale = GetVector("Scale", GetModelName());
    m_width = GetScaledSize().fX;
    m_height = GetScaledSize().fY;
    m_radius = m_width / 2.0f;
    m_payloadOrigin = GetVector("PayloadOrigin", GetModelName());
    m_payload = static_cast<PlayerPayload*>(CreateObject("O-PlayerPayload"));
    m_payload->SetOwner(this);

    orxVECTOR pos = GetPosition();
    orxVECTOR scale = GetScale();
    for (int i = 0; i < m_square; i++)
    {
        m_tileRows.push_back({});
        for (int j = 0; j < m_square; j++)
        {
            Tile *tile = ScrollCast<Tile*>(CreateObject("O-Tile"));
            tile->SetOwner(this);
            if (Cartesian2D())
            {
                tile->SetRotation(0);
                orxVECTOR tileSize = tile->GetSize();
                tile->SetPolarPosition(pos, 0, 0);
                tile->SetScale({ scale.fX / tileSize.fX, scale.fY / tileSize.fY });
                tile->m_bCartesian = m_cartesian;
                tile->m_leftEdgeTopPoint = {
                    (float)j / (float)m_square,
                    (float)i / (float)m_square };
                tile->m_leftEdgeBottomPoint = {
                    (float)j / (float)m_square,
                    ((float)i + 1.0f) / (float)m_square };
                tile->m_rightEdgeTopPoint = {
                    ((float)j + 1.0f) / (float)m_square,
                    (float)i / (float)m_square };
                tile->m_rightEdgeBottomPoint = {
                    ((float)j + 1.0f) / (float)m_square,
                    ((float)i + 1.0f) / (float)m_square };
                tile->m_visualCenter = {
                    pos.fX - m_radius + (m_radius / (float)m_square) + (j * (m_radius / (float)m_halfSquare)),
                    pos.fY - m_radius + (m_radius / (float)m_square) + (i * (m_radius / (float)m_halfSquare)) };
            }
            else if (Cartesian1D())
            {

            }
            else if (Polar2D())
            {
                // The imaginary value of an x or y point between halfNumColumns and halfNumColumns + 1 (used for determining unit distance from center of TileSet).
                float imaginaryPivotValue = m_halfSquare + 0.5f;
                // How many tiles away from the TileSet's pivot (on the X-axis) is this Tile?
                int unitDistanceFromPivotX = abs(imaginaryPivotValue - (j + 1)) + 1;
                // How many tiles away from the TileSet's pivot (on the Y-axis) is this Tile?
                int unitDistanceFromPivotY = abs(imaginaryPivotValue - (i + 1)) + 1;
                // How many tiles away from the TileSet's pivot is this Tile?
                int unitDistanceFromOrigin = orxMAX(unitDistanceFromPivotX, unitDistanceFromPivotY);
                // How far away from the TileSet's pivot is this Tile's visual center?
                float radialDistance = ((1.0f / m_square) * m_radius) + ((unitDistanceFromOrigin - 1) * ((1.0f / m_halfSquare) * m_radius));
                // In what Cartesian quadrant is this Tile?
                const int quadrant = j >= m_halfSquare && i < m_halfSquare ? 1 :
                    j < m_halfSquare && i < m_halfSquare ? 2 :
                    j < m_halfSquare && i >= m_halfSquare ? 3 :
                    4;
                // How many tiles are in this Tile's circular, polar-2D row?
                int tilesInPolarRow = (unitDistanceFromOrigin * 4) + ((unitDistanceFromOrigin - 1) * 4);
                // How many tiles away from the polar axis is this Tile?
                int unitDistanceFromPolarAxis = ((quadrant - 1) * (2 * unitDistanceFromOrigin - 1));
                switch (quadrant)
                {
                case 1:
                    unitDistanceFromPolarAxis += (m_halfSquare - i) + (m_halfSquare + unitDistanceFromOrigin - j) - 1;
                    break;
                case 2:
                    unitDistanceFromPolarAxis += (m_halfSquare - j) + ((i + 1) - (m_halfSquare - unitDistanceFromOrigin)) - 1;
                    break;
                case 3:
                    unitDistanceFromPolarAxis += ((i + 1) - m_halfSquare) + ((j + 1) - (m_halfSquare - unitDistanceFromOrigin)) - 1;
                    break;
                case 4:
                    unitDistanceFromPolarAxis += ((j + 1) - m_halfSquare) + (m_halfSquare + unitDistanceFromOrigin - i) - 1;
                    break;
                }
                // At what angle is this Tile, in reference to the TileSet's pivot?
                float theta = orxMATH_KF_2_PI * ((1.0f / (tilesInPolarRow * 2.0f)) + ((unitDistanceFromPolarAxis - 1) * (1.0f / tilesInPolarRow)));

                tile->SetRotation(-(theta - orxMATH_KF_PI_BY_2));
                orxVECTOR tileSize = tile->GetSize();
                tile->SetPolarPosition(pos, 0, theta);
                tile->SetScale({ scale.fX / tileSize.fX, scale.fY / tileSize.fY });
                float currentRadius = m_radius * ((float)unitDistanceFromOrigin / (float)m_halfSquare);
                float previousRadius = m_radius * (((float)unitDistanceFromOrigin - 1) / (float)m_halfSquare);
                float circularSegmentAngle = (orxMATH_KF_2_PI / (float)tilesInPolarRow);
                float topChordLength = 2.0f * currentRadius * sinf(circularSegmentAngle / 2.0f);
                float bottomChordLength = 2.0f * previousRadius * sinf(circularSegmentAngle / 2.0f);
                float segmentHeight = currentRadius * (1.0f - cosf(circularSegmentAngle / 2.0f));
                float previousSegmentHeight = previousRadius * (1.0f - cosf(circularSegmentAngle / 2.0f));
                float normalizedSegmentHeight = segmentHeight / tile->GetScaledSize().fY;
                float normalizedPreviousSegmentHeight = previousSegmentHeight / tile->GetScaledSize().fY;
                float normalizedTopChordLength = topChordLength / tile->GetScaledSize().fX;
                float normalizedBottomChordLength = bottomChordLength / tile->GetScaledSize().fX;
                tile->m_bCartesian = m_cartesian;
                tile->m_leftEdgeTopPoint = {
                    0.5f - (normalizedTopChordLength / 2.0f),
                    (m_halfSquare - unitDistanceFromOrigin) * (1.0f / (float)m_square) + normalizedSegmentHeight };
                tile->m_leftEdgeBottomPoint = {
                    0.5f - (normalizedBottomChordLength / 2.0f),
                    (m_halfSquare + 1 - unitDistanceFromOrigin) * (1.0f / (float)m_square) + normalizedPreviousSegmentHeight };
                tile->m_rightEdgeTopPoint = {
                    0.5f + (normalizedTopChordLength / 2.0f),
                    (m_halfSquare - unitDistanceFromOrigin) * (1.0f / (float)m_square) + normalizedSegmentHeight };
                tile->m_rightEdgeBottomPoint = {
                    0.5f + (normalizedBottomChordLength / 2.0f),
                    (m_halfSquare + 1 - unitDistanceFromOrigin) * (1.0f / (float)m_square) + normalizedPreviousSegmentHeight };
                tile->m_topRadius = ((float)unitDistanceFromOrigin / m_halfSquare) * 0.5f;
                tile->m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / m_halfSquare) * 0.5f;
                SetPolarPosition(tile->m_visualCenter, pos, radialDistance, theta);
            }
            else // Polar1D()
            {
                // How many tiles away from the TileSet's pivot is this Tile?
                int unitDistanceFromOrigin = m_square - i;
                // How many tiles are in this Tile's circular, polar-2D row?
                int tilesInPolarRow = m_square;
                // How many tiles away from the polar axis is this Tile?
                int unitDistanceFromPolarAxis = ((3 * m_square) / 4) - j;
                if (unitDistanceFromPolarAxis <= 0)
                {
                    unitDistanceFromPolarAxis += m_square;
                }
                if (unitDistanceFromPolarAxis == m_square && m_halfSquare % 2 != 0)
                {
                    unitDistanceFromPolarAxis = 0;
                }
                // How far away from the TileSet's pivot is this Tile's visual center?
                float radialDistance = ((1.0f / (m_square * 2.0f)) * m_radius) + ((unitDistanceFromOrigin - 1) * ((1.0f / m_square) * m_radius));
                // At what angle is this Tile, in reference to the TileSet's pivot?
                float theta = orxMATH_KF_2_PI;
                if (unitDistanceFromPolarAxis == 0)
                {
                    theta = 0;
                }
                else
                {
                    if (m_halfSquare % 2 != 0)
                    {
                        theta *= (unitDistanceFromPolarAxis * (1.0f / tilesInPolarRow));
                    }
                    else
                    {
                        theta *= ((1.0f / (tilesInPolarRow * 2.0f)) + ((unitDistanceFromPolarAxis - 1) * (1.0f / tilesInPolarRow)));
                    }
                }

                tile->SetRotation(-(theta - orxMATH_KF_PI_BY_2));
                orxVECTOR tileSize = tile->GetSize();
                tile->SetPolarPosition(pos, 0, theta);
                tile->SetScale({ scale.fX / tileSize.fX, scale.fY / tileSize.fY });
                float currentRadius = m_radius * ((float)unitDistanceFromOrigin / (float)m_square);
                float previousRadius = m_radius * (((float)unitDistanceFromOrigin - 1) / (float)m_square);
                float circularSegmentAngle = (orxMATH_KF_2_PI / (float)tilesInPolarRow);
                float topChordLength = 2.0f * currentRadius * sinf(circularSegmentAngle / 2.0f);
                float bottomChordLength = 2.0f * previousRadius * sinf(circularSegmentAngle / 2.0f);
                float segmentHeight = currentRadius * (1.0f - cosf(circularSegmentAngle / 2.0f));
                float previousSegmentHeight = previousRadius * (1.0f - cosf(circularSegmentAngle / 2.0f));
                float normalizedSegmentHeight = segmentHeight / tile->GetScaledSize().fY;
                float normalizedPreviousSegmentHeight = previousSegmentHeight / tile->GetScaledSize().fY;
                float normalizedTopChordLength = topChordLength / tile->GetScaledSize().fX;
                float normalizedBottomChordLength = bottomChordLength / tile->GetScaledSize().fX;
                tile->m_bCartesian = m_cartesian;
                tile->m_leftEdgeTopPoint = {
                    0.5f - (normalizedTopChordLength / 2.0f),
                    (m_square - unitDistanceFromOrigin) * (1.0f / ((float)m_square * 2.0f)) + normalizedSegmentHeight };
                tile->m_leftEdgeBottomPoint = {
                    0.5f - (normalizedBottomChordLength / 2.0f),
                    (m_square + 1 - unitDistanceFromOrigin) * (1.0f / ((float)m_square * 2.0f)) + normalizedPreviousSegmentHeight };
                tile->m_rightEdgeTopPoint = {
                    0.5f + (normalizedTopChordLength / 2.0f),
                    (m_square - unitDistanceFromOrigin) * (1.0f / ((float)m_square * 2.0f)) + normalizedSegmentHeight };
                tile->m_rightEdgeBottomPoint = {
                    0.5f + (normalizedBottomChordLength / 2.0f),
                    (m_square + 1 - unitDistanceFromOrigin) * (1.0f / ((float)m_square * 2.0f)) + normalizedPreviousSegmentHeight };
                tile->m_topRadius = ((float)unitDistanceFromOrigin / m_square) * 0.5f;
                tile->m_bottomRadius = ((unitDistanceFromOrigin - 1.0f) / m_square) * 0.5f;
                SetPolarPosition(tile->m_visualCenter, pos, radialDistance, theta);
            }
            m_tileRows.at(i).push_back(tile);
        }
    }
    m_payload->SetPosition(m_tileRows.at(m_payloadOrigin.fX).at(m_payloadOrigin.fY)->m_visualCenter);
}

void TileSet::OnDelete()
{

}

orxBOOL TileSet::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void TileSet::Update(const orxCLOCK_INFO &_rstInfo)
{
    
}

const bool TileSet::Cartesian2D()
{
    return m_cartesian && m_2D;
}

const bool TileSet::Cartesian1D()
{
    return m_cartesian && !m_2D;
}

const bool TileSet::Polar2D()
{
    return !m_cartesian && m_2D;
}

const bool TileSet::Polar1D()
{
    return !m_cartesian && !m_2D;
}
