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

#ifndef AGG_GAMMA_FUNCTIONS_INCLUDED
#define AGG_GAMMA_FUNCTIONS_INCLUDED

#include <math.h>
#include "agg_basics.h"

namespace agg
{
    //===============================================================gamma_none
    struct gamma_none
    {
        float64 operator()(float64 x) const { return x; }
    };


    //==============================================================gamma_power
    class gamma_power
    {
    public:
        gamma_power() : m_gamma(1.0) {}
        gamma_power(float64 g) : m_gamma(g) {}

        void gamma(float64 g) { m_gamma = g; }
        float64 gamma() const { return m_gamma; }

        float64 operator() (float64 x) const
        {
            return pow(x, m_gamma);
        }

    private:
        float64 m_gamma;
    };


    //==========================================================gamma_threshold
    class gamma_threshold
    {
    public:
        gamma_threshold() : m_threshold(0.5) {}
        gamma_threshold(float64 t) : m_threshold(t) {}

        void threshold(float64 t) { m_threshold = t; }
        float64 threshold() const { return m_threshold; }

        float64 operator() (float64 x) const
        {
            return (x < m_threshold) ? 0.0 : 1.0;
        }

    private:
        float64 m_threshold;
    };


    //============================================================gamma_linear
    class gamma_linear
    {
    public:
        gamma_linear() : m_start(0.0), m_end(1.0) {}
        gamma_linear(float64 s, float64 e) : m_start(s), m_end(e) {}

        void set(float64 s, float64 e) { m_start = s; m_end = e; }
        void start(float64 s) { m_start = s; }
        void end(float64 e) { m_end = e; }
        float64 start() const { return m_start; }
        float64 end() const { return m_end; }

        float64 operator() (float64 x) const
        {
            if(x < m_start) return 0.0;
            if(x > m_end) return 1.0;
            return (x - m_start) / (m_end - m_start);
        }

    private:
        float64 m_start;
        float64 m_end;
    };


    //==========================================================gamma_multiply
    class gamma_multiply
    {
    public:
        gamma_multiply() : m_mul(1.0) {}
        gamma_multiply(float64 v) : m_mul(v) {}

        void value(float64 v) { m_mul = v; }
        float64 value() const { return m_mul; }

        float64 operator() (float64 x) const
        {
            float64 y = x * m_mul;
            if(y > 1.0) y = 1.0;
            return y;
        }

    private:
        float64 m_mul;
    };

    inline float64 sRGB_to_linear(float64 x)
    {
        return (x <= 0.04045) ? (x / 12.92) : pow((x + 0.055) / (1.055), 2.4);
    }

    inline float64 linear_to_sRGB(float64 x)
    {
        return (x <= 0.0031308) ? (x * 12.92) : (1.055 * pow(x, 1 / 2.4) - 0.055);
    }
}

#endif



