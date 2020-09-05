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
// class bspline
//
//----------------------------------------------------------------------------

#ifndef AGG_BSPLINE_INCLUDED
#define AGG_BSPLINE_INCLUDED

#include "agg_array.h"

namespace agg
{
    //----------------------------------------------------------------bspline
    // A very simple class of Bi-cubic Spline interpolation.
    // First call init(num, x[], y[]) where num - number of source points, 
    // x, y - arrays of X and Y values respectively. Here Y must be a function 
    // of X. It means that all the X-coordinates must be arranged in the ascending
    // order. 
    // Then call get(x) that calculates a value Y for the respective X. 
    // The class supports extrapolation, i.e. you can call get(x) where x is
    // outside the given with init() X-range. Extrapolation is a simple linear 
    // function.
    //
    //  See Implementation agg_bspline.cpp
    //------------------------------------------------------------------------
    class bspline 
    {
    public:
        bspline();
        bspline(int num);
        bspline(int num, const float64* x, const float64* y);

        void   init(int num);
        void   add_point(float64 x, float64 y);
        void   prepare();

        void   init(int num, const float64* x, const float64* y);

        float64 get(float64 x) const;
        float64 get_stateful(float64 x) const;
    
    private:
        bspline(const bspline&);
        const bspline& operator = (const bspline&);

        static void bsearch(int n, const float64 *x, float64 x0, int *i);
        float64 extrapolation_left(float64 x) const;
        float64 extrapolation_right(float64 x) const;
        float64 interpolation(float64 x, int i) const;

        int               m_max;
        int               m_num;
        float64*           m_x;
        float64*           m_y;
        pod_array<float64> m_am;
        mutable int       m_last_idx;
    };


}

#endif
