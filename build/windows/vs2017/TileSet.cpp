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
            TileEdge *tileEdge1 = tile->m_edges.at(0);
            TileEdge *tileEdge2 = tile->m_edges.at(1);
            TileEdge *tileEdge3 = tile->m_edges.at(2);
            TileEdge *tileEdge4 = tile->m_edges.at(3);
            tile->SetParent(this);
            if (Cartesian2D())
            {
                tile->SetParentSpacePosition({ (j - m_halfSquare) / (float)m_square + (0.5f / m_square), (i - m_halfSquare) / (float)m_square + (0.5f / m_square) });
                tile->SetParentSpaceScale({ 1.0f / m_square, 1.0f / m_square });
                tileEdge1->SetParentSpacePosition({ 0, -0.5f });
                tileEdge1->SetParentSpaceScale({ 1, 1 });
                tileEdge2->SetParentSpacePosition({ 0.5f, 0 });
                tileEdge2->SetParentSpaceScale({ 1, 1 });
                tileEdge3->SetParentSpacePosition({ 0, 0.5f });
                tileEdge3->SetParentSpaceScale({ 1, 1 });
                tileEdge4->SetParentSpacePosition({ -0.5f, 0 });
                tileEdge4->SetParentSpaceScale({ 1, 1 });

                for (int k = 0; k < tileEdge1->m_vertices.size(); k++)
                {
                    tileEdge1->m_vertices.at(k)->SetParentSpacePosition({ -0.5f + ((float)k / (tileEdge1->m_vertices.size() - 1)), 0 });
                }
                for (int k = 0; k < tileEdge2->m_vertices.size(); k++)
                {
                    tileEdge2->m_vertices.at(k)->SetParentSpacePosition({ 0, -0.5f + ((float)k / (tileEdge2->m_vertices.size() - 1)) });
                }
                for (int k = 0; k < tileEdge3->m_vertices.size(); k++)
                {
                    tileEdge3->m_vertices.at(k)->SetParentSpacePosition({ 0.5f - ((float)k / (tileEdge3->m_vertices.size() - 1)), 0 });
                }
                for (int k = 0; k < tileEdge4->m_vertices.size(); k++)
                {
                    tileEdge4->m_vertices.at(k)->SetParentSpacePosition({ 0, 0.5f - ((float)k / (tileEdge4->m_vertices.size() - 1)) });
                }
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
                int unitDistanceFromPivot = orxMAX(unitDistanceFromPivotX, unitDistanceFromPivotY);
                // How far away from the TileSet's pivot is this Tile?
                float radialDistance = ((1.0f / m_square) * m_radius) + ((unitDistanceFromPivot - 1) * ((1.0f / m_halfSquare) * m_radius));
                // In what Cartesian quadrant is this Tile?
                const int quadrant = j >= m_halfSquare && i < m_halfSquare ? 1 :
                    j < m_halfSquare && i < m_halfSquare ? 2 :
                    j < m_halfSquare && i >= m_halfSquare ? 3 :
                    4;
                // How many tiles are in this Tile's circular, polar-2D row?
                int tilesInPolarRow = (unitDistanceFromPivot * 4) + ((unitDistanceFromPivot - 1) * 4);
                // How many tiles away from the polar axis is this Tile?
                int unitDistanceFromPolarAxis = ((quadrant - 1) * (2 * unitDistanceFromPivot - 1));
                switch (quadrant)
                {
                case 1:
                    unitDistanceFromPolarAxis += (m_halfSquare - i) + (m_halfSquare + unitDistanceFromPivot - j) - 1;
                    break;
                case 2:
                    unitDistanceFromPolarAxis += (m_halfSquare - j) + ((i + 1) - (m_halfSquare - unitDistanceFromPivot)) - 1;
                    break;
                case 3:
                    unitDistanceFromPolarAxis += ((i + 1) - m_halfSquare) + ((j + 1) - (m_halfSquare - unitDistanceFromPivot)) - 1;
                    break;
                case 4:
                    unitDistanceFromPolarAxis += ((j + 1) - m_halfSquare) + (m_halfSquare + unitDistanceFromPivot - i) - 1;
                    break;
                }
                // At what angle is this Tile, in reference to the TileSet's pivot?
                float theta = orxMATH_KF_2_PI * ((1.0f / (tilesInPolarRow * 2.0f)) + ((unitDistanceFromPolarAxis - 1) * (1.0f / tilesInPolarRow)));

                tile->SetPolarPosition(pos, radialDistance, theta);

                tileEdge1->SetPolarPosition(pos, radialDistance + m_radius / m_square, theta);
                tileEdge2->SetPolarPosition(pos, radialDistance, theta - (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)));
                tileEdge3->SetPolarPosition(pos, radialDistance - m_radius / m_square, theta);
                tileEdge4->SetPolarPosition(pos, radialDistance, theta + (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)));

                for (int k = 0; k < tileEdge1->m_vertices.size(); k++)
                {
                    tileEdge1->m_vertices.at(k)->SetPolarPosition(
                        pos,
                        radialDistance + m_radius / m_square,
                        theta + (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)) - ((k / (tileEdge1->m_vertices.size() - 1.0f)) * (orxMATH_KF_2_PI / (float)tilesInPolarRow)));
                }
                for (int k = 0; k < tileEdge2->m_vertices.size(); k++)
                {
                    tileEdge2->m_vertices.at(k)->SetPolarPosition(
                        pos,
                        radialDistance + (m_radius / m_square) - ((k / (tileEdge2->m_vertices.size() - 1.0f)) * (m_radius / m_halfSquare)),
                        theta - (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)));
                }
                for (int k = 0; k < tileEdge3->m_vertices.size(); k++)
                {
                    tileEdge3->m_vertices.at(k)->SetPolarPosition(
                        pos,
                        radialDistance - m_radius / m_square,
                        theta - (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)) + ((k / (tileEdge3->m_vertices.size() - 1.0f)) * (orxMATH_KF_2_PI / (float)tilesInPolarRow)));
                }
                for (int k = 0; k < tileEdge4->m_vertices.size(); k++)
                {
                    tileEdge4->m_vertices.at(k)->SetPolarPosition(
                        pos,
                        radialDistance - (m_radius / m_square) + ((k / (tileEdge4->m_vertices.size() - 1.0f)) * (m_radius / m_halfSquare)),
                        theta + (orxMATH_KF_2_PI / (tilesInPolarRow * 2.0f)));
                }
            }
            else // Polar1D()
            {

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
