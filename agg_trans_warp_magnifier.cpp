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

#include <math.h>
#include "agg_trans_warp_magnifier.h"

namespace agg
{

    //------------------------------------------------------------------------
    void trans_warp_magnifier::transform(float64* x, float64* y) const
    {
        float64 dx = *x - m_xc;
        float64 dy = *y - m_yc;
        float64 r = sqrt(dx * dx + dy * dy);
        if(r < m_radius)
        {
            *x = m_xc + dx * m_magn;
            *y = m_yc + dy * m_magn;
            return;
        }

        float64 m = (r + m_radius * (m_magn - 1.0)) / r;
        *x = m_xc + dx * m;
        *y = m_yc + dy * m;
    }

    //------------------------------------------------------------------------
    void trans_warp_magnifier::inverse_transform(float64* x, float64* y) const
    {
        // New version by Andrew Skalkin
        //-----------------
        float64 dx = *x - m_xc;
        float64 dy = *y - m_yc;
        float64 r = sqrt(dx * dx + dy * dy);

        if(r < m_radius * m_magn) 
        {
            *x = m_xc + dx / m_magn;
            *y = m_yc + dy / m_magn;
        }
        else
        {
            float64 rnew = r - m_radius * (m_magn - 1.0);
            *x = m_xc + rnew * dx / r; 
            *y = m_yc + rnew * dy / r;
        }

        // Old version
        //-----------------
        //trans_warp_magnifier t(*this);
        //t.magnification(1.0 / m_magn);
        //t.radius(m_radius * m_magn);
        //t.transform(x, y);
    }


}
