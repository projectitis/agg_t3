//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------

#ifndef AGG_VPGEN_SEGMENTATOR_INCLUDED
#define AGG_VPGEN_SEGMENTATOR_INCLUDED

#include <math.h>
#include "agg_basics.h"

namespace agg
{

    //=======================================================vpgen_segmentator
    // 
    // See Implementation agg_vpgen_segmentator.cpp
    //
    class vpgen_segmentator
    {
    public:
        vpgen_segmentator() : m_approximation_scale(1.0) {}

        void approximation_scale(float64 s) { m_approximation_scale = s;     }
        float64 approximation_scale() const { return m_approximation_scale;  }

        static bool auto_close()   { return false; }
        static bool auto_unclose() { return false; }

        void reset() { m_cmd = path_cmd_stop; }
        void move_to(float64 x, float64 y);
        void line_to(float64 x, float64 y);
        unsigned vertex(float64* x, float64* y);

    private:
        float64   m_approximation_scale;
        float64   m_x1;
        float64   m_y1;
        float64   m_dx;
        float64   m_dy;
        float64   m_dl;
        float64   m_ddl;
        unsigned m_cmd;
    };



}

#endif

