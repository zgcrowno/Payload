#pragma once

#include "Payload.h"
#include "Doer.h"
#include "Goal.h"
#include "MemorySetCartesian1D.h"
#include "MemorySetCartesian2D.h"
#include "MemorySetPolar1D.h"
#include "MemorySetPolar2D.h"
#include "PlayerPayload.h"
#include "Tile.h"
#include "TileSetShiftStatus.h"
#include "TileSetState.h"
#include <vector>

#define NATIVE_TEXTURE_SIZE 1024.0f
#define NATIVE_BORDER_SIZE 5.0f
#define NORMALIZED_BORDER_SIZE (NATIVE_BORDER_SIZE / NATIVE_TEXTURE_SIZE)

namespace payload
{
    class TileSet : public Doer
    {
    protected:
        //! Called on object creation
        virtual void OnCreate();
        //! Called on object deletion
        virtual void OnDelete();
        //! Called on object collision
        virtual orxBOOL OnCollide(
            ScrollObject *_poCollider,
            const orxSTRING _zPartName,
            const orxSTRING _zColliderPartName,
            const orxVECTOR &_rvPosition,
            const orxVECTOR &_rvNormal);
        //! Called on shader event
        virtual orxBOOL OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload);
        //! Called on clock update
        virtual void Update(const orxCLOCK_INFO &_rstInfo);
    private:
        bool m_bPriorDoerActedDueToShifting;
        bool m_bInvertReconfigure;
        int m_square;
        int m_halfSquare;
        float m_normalizedTileSize;
        float m_width;
        float m_height;
        float m_radius;
        float m_timeToShift;
        float m_timeSpentShifting;
        float m_timeToReconfigure;
        float m_timeSpentReconfiguring;
        TileSetState m_state;
        TileSetState m_priorState;
        MemorySet *m_memorySetToReconfigure;
        PlayerPayload *m_payload;
        Goal *m_goal;
        std::stack<TileSetState> m_priorStates;
        std::vector<MemorySetCartesian1D*> m_memorySetsCartesian1D;
        std::vector<MemorySetCartesian2D*> m_memorySetsCartesian2D;
        std::vector<MemorySetPolar1D*> m_memorySetsPolar1D;
        std::vector<MemorySetPolar2D*> m_memorySetsPolar2D;
        std::vector<std::vector<Tile*>> m_tileRows;

        virtual void Undo();

        void SetMemorySetToReconfigure(MemorySet *_memSet);
        void Shift(TileSetShiftStatus _shiftStatus);
        void ShiftTiles();
        void Reconfigure();
        void ReconfigureTiles();
        void FinalizeTileAndInhabitantLerps();
        const bool Is2D();
        const bool PriorStateIs2D();
        const bool IsCartesian();
        const bool InputAllowed();
        const bool NeedToShiftD1Tiles();
        const int GetUnitDistanceFromPolarAxis(const int &_col);
        const int GetGreatest1DUnitDistanceOfPayloadRowFromThreshold();
        const float GetPolarTheta(const int &_unitDistanceFromPolarAxis, const int &_tilesInPolarRow, const bool _d2);
        const orxVECTOR GetNormalizedPosition(const orxVECTOR &_vec);
    public:
        TileSetShiftStatus m_shiftStatus;
        TileSetShiftStatus m_priorShiftStatus;
        // A stack of pairs of Doers and whether or not those Doers executed an action because of shifting
        // (if the second pair element is true, then the action was due to shifting).
        std::stack<std::pair<Doer*, bool>> m_priorDoers;
    };
}
