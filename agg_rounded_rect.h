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
// Rounded rectangle vertex generator
//
//----------------------------------------------------------------------------

#ifndef AGG_ROUNDED_RECT_INCLUDED
#define AGG_ROUNDED_RECT_INCLUDED

#include "agg_basics.h"
#include "agg_arc.h"

namespace agg
{
    //------------------------------------------------------------rounded_rect
    //
    // See Implemantation agg_rounded_rect.cpp
    //
    class rounded_rect
    {
    public:
        rounded_rect() {}
        rounded_rect(float64 x1, float64 y1, float64 x2, float64 y2, float64 r);

        void rect(float64 x1, float64 y1, float64 x2, float64 y2);
        void radius(float64 r);
        void radius(float64 rx, float64 ry);
        void radius(float64 rx_bottom, float64 ry_bottom, float64 rx_top, float64 ry_top);
        void radius(float64 rx1, float64 ry1, float64 rx2, float64 ry2, 
                    float64 rx3, float64 ry3, float64 rx4, float64 ry4);
        void normalize_radius();

        void approximation_scale(float64 s) { m_arc.approximation_scale(s); }
        float64 approximation_scale() const { return m_arc.approximation_scale(); }

        void rewind(unsigned);
        unsigned vertex(float64* x, float64* y);

    private:
        float64 m_x1;
        float64 m_y1;
        float64 m_x2;
        float64 m_y2;
        float64 m_rx1;
        float64 m_ry1;
        float64 m_rx2;
        float64 m_ry2;
        float64 m_rx3;
        float64 m_ry3;
        float64 m_rx4;
        float64 m_ry4;
        unsigned m_status;
        arc      m_arc;
    };

}

#endif

