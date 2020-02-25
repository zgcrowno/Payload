#include "TileSet.h"

using namespace payload;

void TileSet::OnCreate()
{
    m_2D = GetBool("2D", GetModelName());
    m_cartesian = GetBool("Cartesian", GetModelName());
    m_numRows = GetU32("NumRows", GetModelName());
    m_numColumns = GetU32("NumColumns", GetModelName());

    int halfNumColumns = m_numColumns / 2;
    int halfNumRows = m_numRows / 2;
    orxVECTOR pos = GetPosition();
    for (int i = 0; i < m_numRows; i++)
    {
        m_tileRows.push_back({});
        for (int j = 0; j < m_numColumns; j++)
        {
            Tile *tile = ScrollCast<Tile*>(CreateObject("O-Tile"));
            TileEdge *tileEdge1 = tile->m_edges.at(0);
            TileEdge *tileEdge2 = tile->m_edges.at(1);
            TileEdge *tileEdge3 = tile->m_edges.at(2);
            TileEdge *tileEdge4 = tile->m_edges.at(3);
            tile->SetParent(this);
            if (Cartesian2D())
            {
                tile->SetParentSpacePosition({ (j - halfNumColumns) / (float)m_numColumns + (0.5f / m_numColumns), (i - halfNumRows) / (float)m_numRows + (0.5f / m_numRows) });
                tile->SetParentSpaceScale({ 1.0f / m_numColumns, 1.0f / m_numRows });
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
