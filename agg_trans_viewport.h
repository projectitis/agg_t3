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
// Viewport transformer - simple orthogonal conversions from world coordinates
//                        to screen (device) ones.
//
//----------------------------------------------------------------------------

#ifndef AGG_TRANS_VIEWPORT_INCLUDED
#define AGG_TRANS_VIEWPORT_INCLUDED

#include <string.h>
#include "agg_trans_affine.h"


namespace agg
{

    enum aspect_ratio_e
    {
        aspect_ratio_stretch,
        aspect_ratio_meet,
        aspect_ratio_slice
    };


    //----------------------------------------------------------trans_viewport
    class trans_viewport
    {
    public:
        //-------------------------------------------------------------------
        trans_viewport() : 
            m_world_x1(0.0),
            m_world_y1(0.0),
            m_world_x2(1.0),
            m_world_y2(1.0),
            m_device_x1(0.0),
            m_device_y1(0.0),
            m_device_x2(1.0),
            m_device_y2(1.0),
            m_aspect(aspect_ratio_stretch),
            m_is_valid(true),
            m_align_x(0.5),
            m_align_y(0.5),
            m_wx1(0.0),
            m_wy1(0.0),
            m_wx2(1.0),
            m_wy2(1.0),
            m_dx1(0.0),
            m_dy1(0.0),
            m_kx(1.0),
            m_ky(1.0)
        {}

        //-------------------------------------------------------------------
        void preserve_aspect_ratio(float64 alignx, 
                                   float64 aligny, 
                                   aspect_ratio_e aspect)
        {
            m_align_x = alignx;
            m_align_y = aligny;
            m_aspect  = aspect;
            update();
        }

        //-------------------------------------------------------------------
        void device_viewport(float64 x1, float64 y1, float64 x2, float64 y2)
        {
            m_device_x1 = x1;
            m_device_y1 = y1;
            m_device_x2 = x2;
            m_device_y2 = y2;
            update();
        }

        //-------------------------------------------------------------------
        void world_viewport(float64 x1, float64 y1, float64 x2, float64 y2)
        {
            m_world_x1 = x1;
            m_world_y1 = y1;
            m_world_x2 = x2;
            m_world_y2 = y2;
            update();
        }

        //-------------------------------------------------------------------
        void device_viewport(float64* x1, float64* y1, float64* x2, float64* y2) const
        {
            *x1 = m_device_x1;
            *y1 = m_device_y1;
            *x2 = m_device_x2;
            *y2 = m_device_y2;
        }

        //-------------------------------------------------------------------
        void world_viewport(float64* x1, float64* y1, float64* x2, float64* y2) const
        {
            *x1 = m_world_x1;
            *y1 = m_world_y1;
            *x2 = m_world_x2;
            *y2 = m_world_y2;
        }

        //-------------------------------------------------------------------
        void world_viewport_actual(float64* x1, float64* y1, 
                                   float64* x2, float64* y2) const
        {
            *x1 = m_wx1;
            *y1 = m_wy1;
            *x2 = m_wx2;
            *y2 = m_wy2;
        }

        //-------------------------------------------------------------------
        bool   is_valid()             const { return m_is_valid; }
        float64 align_x()              const { return m_align_x; }
        float64 align_y()              const { return m_align_y; }
        aspect_ratio_e aspect_ratio() const { return m_aspect; }

        //-------------------------------------------------------------------
        void transform(float64* x, float64* y) const
        {
            *x = (*x - m_wx1) * m_kx + m_dx1;
            *y = (*y - m_wy1) * m_ky + m_dy1;
        }

        //-------------------------------------------------------------------
        void transform_scale_only(float64* x, float64* y) const
        {
            *x *= m_kx;
            *y *= m_ky;
        }

        //-------------------------------------------------------------------
        void inverse_transform(float64* x, float64* y) const
        {
            *x = (*x - m_dx1) / m_kx + m_wx1;
            *y = (*y - m_dy1) / m_ky + m_wy1;
        }

        //-------------------------------------------------------------------
        void inverse_transform_scale_only(float64* x, float64* y) const
        {
            *x /= m_kx;
            *y /= m_ky;
        }

        //-------------------------------------------------------------------
        float64 device_dx() const { return m_dx1 - m_wx1 * m_kx; }
        float64 device_dy() const { return m_dy1 - m_wy1 * m_ky; }

        //-------------------------------------------------------------------
        float64 scale_x() const
        {
            return m_kx;
        }

        //-------------------------------------------------------------------
        float64 scale_y() const
        {
            return m_ky;
        }

        //-------------------------------------------------------------------
        float64 scale() const
        {
            return (m_kx + m_ky) * 0.5;
        }

        //-------------------------------------------------------------------
        trans_affine to_affine() const
        {
            trans_affine mtx = trans_affine_translation(-m_wx1, -m_wy1);
            mtx *= trans_affine_scaling(m_kx, m_ky);
            mtx *= trans_affine_translation(m_dx1, m_dy1);
            return mtx;
        }

        //-------------------------------------------------------------------
        trans_affine to_affine_scale_only() const
        {
            return trans_affine_scaling(m_kx, m_ky);
        }

        //-------------------------------------------------------------------
        unsigned byte_size() const
        {
            return sizeof(*this);
        }

        void serialize(int8u* ptr) const
        {
            memcpy(ptr, this, sizeof(*this)); 
        }

        void deserialize(const int8u* ptr)
        {
            memcpy(this,  ptr, sizeof(*this));
        }

    private:
        void update();

        float64         m_world_x1;
        float64         m_world_y1;
        float64         m_world_x2;
        float64         m_world_y2;
        float64         m_device_x1;
        float64         m_device_y1;
        float64         m_device_x2;
        float64         m_device_y2;
        aspect_ratio_e m_aspect;
        bool           m_is_valid;
        float64         m_align_x;
        float64         m_align_y;
        float64         m_wx1;
        float64         m_wy1;
        float64         m_wx2;
        float64         m_wy2;
        float64         m_dx1;
        float64         m_dy1;
        float64         m_kx;
        float64         m_ky;
    };



    //-----------------------------------------------------------------------
    inline void trans_viewport::update()
    {
        const float64 epsilon = 1e-30;
        if(fabs(m_world_x1  - m_world_x2)  < epsilon ||
           fabs(m_world_y1  - m_world_y2)  < epsilon ||
           fabs(m_device_x1 - m_device_x2) < epsilon ||
           fabs(m_device_y1 - m_device_y2) < epsilon)
        {
            m_wx1 = m_world_x1;
            m_wy1 = m_world_y1;
            m_wx2 = m_world_x1 + 1.0;
            m_wy2 = m_world_y2 + 1.0;
            m_dx1 = m_device_x1;
            m_dy1 = m_device_y1;
            m_kx  = 1.0;
            m_ky  = 1.0;
            m_is_valid = false;
            return;
        }

        float64 world_x1  = m_world_x1;
        float64 world_y1  = m_world_y1;
        float64 world_x2  = m_world_x2;
        float64 world_y2  = m_world_y2;
        float64 device_x1 = m_device_x1;
        float64 device_y1 = m_device_y1;
        float64 device_x2 = m_device_x2;
        float64 device_y2 = m_device_y2;
        if(m_aspect != aspect_ratio_stretch)
        {
            float64 d;
            m_kx = (device_x2 - device_x1) / (world_x2 - world_x1);
            m_ky = (device_y2 - device_y1) / (world_y2 - world_y1);

            if((m_aspect == aspect_ratio_meet) == (m_kx < m_ky))
            {
                d         = (world_y2 - world_y1) * m_ky / m_kx;
                world_y1 += (world_y2 - world_y1 - d) * m_align_y;
                world_y2  =  world_y1 + d;
            }
            else
            {
                d         = (world_x2 - world_x1) * m_kx / m_ky;
                world_x1 += (world_x2 - world_x1 - d) * m_align_x;
                world_x2  =  world_x1 + d;
            }
        }
        m_wx1 = world_x1;
        m_wy1 = world_y1;
        m_wx2 = world_x2;
        m_wy2 = world_y2;
        m_dx1 = device_x1;
        m_dy1 = device_y1;
        m_kx  = (device_x2 - device_x1) / (world_x2 - world_x1);
        m_ky  = (device_y2 - device_y1) / (world_y2 - world_y1);
        m_is_valid = true;
    }


}


#endif
