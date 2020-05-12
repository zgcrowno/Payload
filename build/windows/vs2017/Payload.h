#pragma once

#ifndef __ORXSCROLL_H_
#define __ORXSCROLL_H_

//! Includes
// The following define skips compilation of ScrollEd (map editor) for now
#define __NO_SCROLLED__
#include "Scroll.h"
#include <vector>

namespace payload
{
    //! OrxScroll class
    class Payload : public Scroll<Payload>
    {
    private:
        //! Event handler for user-defined events.
        static orxSTATUS orxFASTCALL EventHandler(const orxEVENT *_pstEvent);

        //! Enforce finding of config files outside of .exe folder
        virtual orxSTATUS Bootstrap() const;
        //! Initialize the program
        virtual orxSTATUS Init();
        //! Bind our scroll objects
        virtual void BindObjects();
        //! Callback called every frame
        virtual orxSTATUS Run();
        //! Exit the program
        virtual void Exit();

        //! Draws all NuklearWindows, if there are any.
        void DrawNuklearWindows();
    public:
        const int GetPayloadRow();
        ScrollObject *GetTileSet();
        std::vector<ScrollObject*> GetTileInhabitants();
        std::vector<ScrollObject*> GetFirewalls();
        std::vector<ScrollObject*> GetNuklearWindows();
    };
}

#endif // __ORXSCROLL_H_
