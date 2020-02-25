#include "TileEdge.h"

using namespace payload;

void TileEdge::OnCreate()
{
    orxVECTOR rgb = GetVector("FillColor", GetModelName());
    float fillAlpha = GetFloat("FillAlpha", GetModelName());
    orxCOLOR fillColor = { rgb.fR, rgb.fG, rgb.fB, fillAlpha };
    m_fillColor = orxColor_ToRGBA(&fillColor);

    int numVertices = GetU32("NumVertices", GetModelName());
    for (int i = 0; i < numVertices; i++)
    {
        ScrollMod *tileEdgeVertex = ScrollCast<ScrollMod*>(CreateObject("O-TileEdgeVertex"));
        tileEdgeVertex->SetParent(this);
        m_vertices.push_back(tileEdgeVertex);
    }
}

void TileEdge::OnDelete()
{

}

orxBOOL TileEdge::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void TileEdge::Update(const orxCLOCK_INFO &_rstInfo)
{
    Drawn::Update(_rstInfo);
}

void TileEdge::Draw()
{
    
}
