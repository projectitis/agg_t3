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
//
// Arc vertex generator
//
//----------------------------------------------------------------------------

#ifndef AGG_ARC_INCLUDED
#define AGG_ARC_INCLUDED

#include <math.h>
#include "agg_basics.h"

namespace agg
{

    //=====================================================================arc
    //
    // See Implementation agg_arc.cpp 
    //
    class arc
    {
    public:
        arc() : m_scale(1.0), m_initialized(false) {}
        arc(float64 x,  float64 y, 
            float64 rx, float64 ry, 
            float64 a1, float64 a2, 
            bool ccw=true);

        void init(float64 x,  float64 y, 
                  float64 rx, float64 ry, 
                  float64 a1, float64 a2, 
                  bool ccw=true);

        void approximation_scale(float64 s);
        float64 approximation_scale() const { return m_scale;  }

        void rewind(unsigned);
        unsigned vertex(float64* x, float64* y);

    private:
        void normalize(float64 a1, float64 a2, bool ccw);

        float64   m_x;
        float64   m_y;
        float64   m_rx;
        float64   m_ry;
        float64   m_angle;
        float64   m_start;
        float64   m_end;
        float64   m_scale;
        float64   m_da;
        bool     m_ccw;
        bool     m_initialized;
        unsigned m_path_cmd;
    };


}


#endif
