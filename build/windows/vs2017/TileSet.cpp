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

    orxVECTOR pos = GetPosition();
    for (int i = 0; i < m_square; i++)
    {
        m_tileRows.push_back({});
        for (int j = 0; j < m_square; j++)
        {
            Tile *tile = ScrollCast<Tile*>(CreateObject("O-Tile"));
            tile->SetOwner(this);
            if (Cartesian2D())
            {
                tile->SetPosition({
                    pos.fX - m_radius + (m_radius / (float)m_square) + ((float)j * (m_radius / (float)m_halfSquare)),
                    pos.fY - m_radius + (m_radius / (float)m_square) + ((float)i * (m_radius / (float)m_halfSquare)) });
                float tileWidth = m_radius / (float)m_halfSquare;
                tile->SetScale({ tileWidth, tileWidth });
                float tileHalfWidth = tileWidth / 2.0f;
                orxVECTOR tilePos = tile->GetPosition();
                /*for (int k = 0; k < tileEdge1->m_vertices.size(); k++)
                {
                    tileEdge1->m_vertices.at(k)->SetPosition({
                        tilePos.fX - tileHalfWidth + (k * (tileWidth / (tileEdge1->m_vertices.size() - 1))),
                        tilePos.fY - tileHalfWidth });
                }
                for (int k = 0; k < tileEdge2->m_vertices.size(); k++)
                {
                    tileEdge2->m_vertices.at(k)->SetPosition({
                        tilePos.fX + tileHalfWidth,
                        tilePos.fY - tileHalfWidth + (k * (tileWidth / (tileEdge2->m_vertices.size() - 1))) });
                }
                for (int k = 0; k < tileEdge3->m_vertices.size(); k++)
                {
                    tileEdge3->m_vertices.at(k)->SetPosition({
                        tilePos.fX + tileHalfWidth - (k * (tileWidth / (tileEdge3->m_vertices.size() - 1))),
                        tilePos.fY + tileHalfWidth });
                }
                for (int k = 0; k < tileEdge4->m_vertices.size(); k++)
                {
                    tileEdge4->m_vertices.at(k)->SetPosition({
                        tilePos.fX - tileHalfWidth,
                        tilePos.fY + tileHalfWidth - (k * (tileWidth / (tileEdge4->m_vertices.size() - 1))) });
                }*/
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
                tile->m_unitDistanceFromOrigin = orxMAX(unitDistanceFromPivotX, unitDistanceFromPivotY);
                // How far away from the TileSet's pivot is this Tile?
                float radialDistance = ((1.0f / m_square) * m_radius) + ((tile->m_unitDistanceFromOrigin - 1) * ((1.0f / m_halfSquare) * m_radius));
                // In what Cartesian quadrant is this Tile?
                const int quadrant = j >= m_halfSquare && i < m_halfSquare ? 1 :
                    j < m_halfSquare && i < m_halfSquare ? 2 :
                    j < m_halfSquare && i >= m_halfSquare ? 3 :
                    4;
                // How many tiles are in this Tile's circular, polar-2D row?
                int tilesInPolarRow = (tile->m_unitDistanceFromOrigin * 4) + ((tile->m_unitDistanceFromOrigin - 1) * 4);
                // How many tiles away from the polar axis is this Tile?
                int unitDistanceFromPolarAxis = ((quadrant - 1) * (2 * tile->m_unitDistanceFromOrigin - 1));
                switch (quadrant)
                {
                case 1:
                    unitDistanceFromPolarAxis += (m_halfSquare - i) + (m_halfSquare + tile->m_unitDistanceFromOrigin - j) - 1;
                    break;
                case 2:
                    unitDistanceFromPolarAxis += (m_halfSquare - j) + ((i + 1) - (m_halfSquare - tile->m_unitDistanceFromOrigin)) - 1;
                    break;
                case 3:
                    unitDistanceFromPolarAxis += ((i + 1) - m_halfSquare) + ((j + 1) - (m_halfSquare - tile->m_unitDistanceFromOrigin)) - 1;
                    break;
                case 4:
                    unitDistanceFromPolarAxis += ((j + 1) - m_halfSquare) + (m_halfSquare + tile->m_unitDistanceFromOrigin - i) - 1;
                    break;
                }
                // At what angle is this Tile, in reference to the TileSet's pivot?
                float theta = orxMATH_KF_2_PI * ((1.0f / (tilesInPolarRow * 2.0f)) + ((unitDistanceFromPolarAxis - 1) * (1.0f / tilesInPolarRow)));

                float currentRadius = m_radius * ((float)tile->m_unitDistanceFromOrigin / (float)m_halfSquare);
                float previousRadius = m_radius * (((float)tile->m_unitDistanceFromOrigin - 1) / (float)m_halfSquare);
                float circularSegmentAngle = (orxMATH_KF_2_PI / (float)tilesInPolarRow);
                float topChordLength = 2.0f * currentRadius * sinf(circularSegmentAngle / 2.0f);
                float bottomChordLength = 2.0f * previousRadius * sinf(circularSegmentAngle / 2.0f);
                float segmentHeight = currentRadius * (1.0f - cosf(circularSegmentAngle / 2.0f));
                float previousSegmentHeight = previousRadius * (1.0f - cosf(circularSegmentAngle / 2.0f));
                orxVECTOR tileSize = tile->GetSize();
                tile->SetPolarPosition(pos, radialDistance, theta);
                tile->SetRotation(-(theta - orxMATH_KF_PI_BY_2));
                tile->SetScale({ topChordLength / tileSize.fX, ((m_radius / m_halfSquare) + previousSegmentHeight) / tileSize.fY });
                float normalizedSegmentHeight = segmentHeight / tile->GetScaledSize().fY;
                float normalizedPreviousSegmentHeight = previousSegmentHeight / tile->GetScaledSize().fY;
                float normalizedBottomChordLength = bottomChordLength / tile->GetScaledSize().fX;
                tile->m_topCenterAngle = atan2f(0.5f, normalizedSegmentHeight) * 2.0f;
                tile->m_leftEdgeTopAngle = (orxMATH_KF_PI - tile->m_topCenterAngle) / 2.0f;
                tile->m_rightEdgeTopAngle = tile->m_leftEdgeTopAngle;
                tile->m_bottomCenterAngle = atan2f(normalizedBottomChordLength / 2.0f, normalizedPreviousSegmentHeight) * 2.0f;
                tile->m_leftEdgeBottomAngle = (orxMATH_KF_PI - tile->m_bottomCenterAngle) / 2.0f;
                tile->m_rightEdgeBottomAngle = tile->m_leftEdgeBottomAngle;
                tile->m_topCenterPoint = { 0.5f, 0.0f };
                tile->m_bottomCenterPoint = { 0.5f, 1.0f - normalizedPreviousSegmentHeight };
                tile->m_leftEdgeTopPoint = { 0.0f, normalizedSegmentHeight };
                tile->m_leftEdgeBottomPoint = { 0.5f - ((bottomChordLength / topChordLength) / 2.0f), 1.0f };
                tile->m_rightEdgeTopPoint = { 1.0f, normalizedSegmentHeight };
                tile->m_rightEdgeBottomPoint = { 0.5f + ((bottomChordLength / topChordLength) / 2.0f), 1.0f };
            }
            else // Polar1D()
            {
                // How many tiles away from the TileSet's pivot is this Tile?
                int unitDistanceFromPivot = m_square - i;
                // How far away from the TileSet's pivot is this Tile?
                float radialDistance = ((1.0f / (m_square * 2.0f)) * m_radius) + ((unitDistanceFromPivot - 1) * ((1.0f / m_square) * m_radius));
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

                tile->SetPolarPosition(pos, radialDistance, theta);

                /*tileEdge1->SetPolarPosition(pos, radialDistance + (m_radius / (m_square * 2.0f)), theta);
                tileEdge2->SetPolarPosition(pos, radialDistance, theta - (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)));
                tileEdge3->SetPolarPosition(pos, radialDistance - (m_radius / (m_square * 2.0f)), theta);
                tileEdge4->SetPolarPosition(pos, radialDistance, theta + (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)));

                for (int k = 0; k < tileEdge1->m_vertices.size(); k++)
                {
                    tileEdge1->m_vertices.at(k)->SetPolarPosition(
                        pos,
                        radialDistance + (m_radius / (m_square * 2.0f)),
                        theta + (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)) - ((k / (tileEdge1->m_vertices.size() - 1.0f)) * (orxMATH_KF_2_PI / (float)tilesInPolarRow)));
                }
                for (int k = 0; k < tileEdge2->m_vertices.size(); k++)
                {
                    tileEdge2->m_vertices.at(k)->SetPolarPosition(
                        pos,
                        radialDistance + (m_radius / (m_square * 2.0f)) - ((k / (tileEdge2->m_vertices.size() - 1.0f)) * (m_radius / m_square)),
                        theta - (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)));
                }
                for (int k = 0; k < tileEdge3->m_vertices.size(); k++)
                {
                    tileEdge3->m_vertices.at(k)->SetPolarPosition(
                        pos,
                        radialDistance - (m_radius / (m_square * 2.0f)),
                        theta - (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)) + ((k / (tileEdge3->m_vertices.size() - 1.0f)) * (orxMATH_KF_2_PI / (float)tilesInPolarRow)));
                }
                for (int k = 0; k < tileEdge4->m_vertices.size(); k++)
                {
                    tileEdge4->m_vertices.at(k)->SetPolarPosition(
                        pos,
                        radialDistance - (m_radius / (m_square * 2.0f)) + ((k / (tileEdge4->m_vertices.size() - 1.0f)) * (m_radius / m_square)),
                        theta + (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)));
                }*/
            }
            m_tileRows.at(i).push_back(tile);
        }
    }
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
