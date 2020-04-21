#include "Firewall.h"

using namespace payload;

void Firewall::OnCreate()
{
    TileInhabitant::OnCreate();

    m_direction = static_cast<Direction>(GetU32("Direction", GetModelName()));
    m_beam = static_cast<ScrollMod*>(GetChildByName("O-FirewallBeam"));
}

void Firewall::OnDelete()
{
    TileInhabitant::OnDelete();
}

orxBOOL Firewall::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Firewall::Update(const orxCLOCK_INFO &_rstInfo)
{
    TileInhabitant::Update(_rstInfo);
}

void Firewall::Cohabitate(TileInhabitant *_other, const bool _dueToShifting)
{
    _other->Die();

    // Ensure that in the event that two Firewalls cohabitate, both die.
    if (dynamic_cast<Firewall*>(_other) != nullptr)
    {
        Die();
    }
}

void Firewall::ExertInfluence(TileInhabitant *_other)
{
    Firewall *otherFirewall = dynamic_cast<Firewall*>(_other);
    if (otherFirewall != nullptr && otherFirewall->IsInPurview(this))
    {
        Die();
    }
    _other->Die();
}

const bool Firewall::IsInPurview(TileInhabitant *_other)
{
    Tile *otherTile = _other->m_target;

    switch (m_direction)
    {
    case Direction::Right:
    {
        switch (*otherTile->m_state)
        {
        case TileSetState::Cartesian1D:
        {
            return m_target->m_col < otherTile->m_col;
        }
        case TileSetState::Polar1D:
        {
            return m_target->m_col != otherTile->m_col;
        }
        break;
        case TileSetState::Cartesian2D:
        case TileSetState::Polar2D:
        {
            return m_target->m_row == otherTile->m_row && m_target->m_col < otherTile->m_col;
        }
        break;
        }
    }
    break;
    case Direction::Down:
    {
        switch (*otherTile->m_state)
        {
        case TileSetState::Cartesian1D:
        case TileSetState::Polar1D:
        {
            return false;
        }
        break;
        case TileSetState::Cartesian2D:
        case TileSetState::Polar2D:
        {
            return m_target->m_row < otherTile->m_row && m_target->m_col == otherTile->m_col;
        }
        break;
        }
    }
    break;
    case Direction::Left:
    {
        switch (*otherTile->m_state)
        {
        case TileSetState::Cartesian1D:
        {
            return m_target->m_col > otherTile->m_col;
        }
        break;
        case TileSetState::Polar1D:
        {
            return m_target->m_col != otherTile->m_col;
        }
        break;
        case TileSetState::Cartesian2D:
        case TileSetState::Polar2D:
        {
            return m_target->m_row == otherTile->m_row && m_target->m_col > otherTile->m_col;
        }
        break;
        }
    }
    break;
    case Direction::Up:
    {
        switch (*otherTile->m_state)
        {
        case TileSetState::Cartesian1D:
        case TileSetState::Polar1D:
        {
            return false;
        }
        break;
        case TileSetState::Cartesian2D:
        case TileSetState::Polar2D:
        {
            return m_target->m_row > otherTile->m_row && m_target->m_col == otherTile->m_col;
        }
        break;
        }
    }
    break;
    }
}
