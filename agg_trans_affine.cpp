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
// Affine transformations
//
//----------------------------------------------------------------------------
#include "agg_trans_affine.h"



namespace agg
{

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::parl_to_parl(const float64* src, 
                                                   const float64* dst)
    {
        sx  = src[2] - src[0];
        shy = src[3] - src[1];
        shx = src[4] - src[0];
        sy  = src[5] - src[1];
        tx  = src[0];
        ty  = src[1];
        invert();
        multiply(trans_affine(dst[2] - dst[0], dst[3] - dst[1], 
                              dst[4] - dst[0], dst[5] - dst[1],
                              dst[0], dst[1]));
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::rect_to_parl(float64 x1, float64 y1, 
                                                   float64 x2, float64 y2, 
                                                   const float64* parl)
    {
        float64 src[6];
        src[0] = x1; src[1] = y1;
        src[2] = x2; src[3] = y1;
        src[4] = x2; src[5] = y2;
        parl_to_parl(src, parl);
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::parl_to_rect(const float64* parl, 
                                                   float64 x1, float64 y1, 
                                                   float64 x2, float64 y2)
    {
        float64 dst[6];
        dst[0] = x1; dst[1] = y1;
        dst[2] = x2; dst[3] = y1;
        dst[4] = x2; dst[5] = y2;
        parl_to_parl(parl, dst);
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::multiply(const trans_affine& m)
    {
        float64 t0 = sx  * m.sx + shy * m.shx;
        float64 t2 = shx * m.sx + sy  * m.shx;
        float64 t4 = tx  * m.sx + ty  * m.shx + m.tx;
        shy = sx  * m.shy + shy * m.sy;
        sy  = shx * m.shy + sy  * m.sy;
        ty  = tx  * m.shy + ty  * m.sy + m.ty;
        sx  = t0;
        shx = t2;
        tx  = t4;
        return *this;
    }


    //------------------------------------------------------------------------
    const trans_affine& trans_affine::invert()
    {
        float64 d  = determinant_reciprocal();

        float64 t0  =  sy  * d;
               sy  =  sx  * d;
               shy = -shy * d;
               shx = -shx * d;

        float64 t4 = -tx * t0  - ty * shx;
               ty = -tx * shy - ty * sy;

        sx = t0;
        tx = t4;
        return *this;
    }


   //------------------------------------------------------------------------
    const trans_affine& trans_affine::flip_x()
    {
        sx  = -sx;
        shy = -shy;
        tx  = -tx;
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::flip_y()
    {
        shx = -shx;
        sy  = -sy;
        ty  = -ty;
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::reset()
    {
        sx  = sy  = 1.0; 
        shy = shx = tx = ty = 0.0;
        return *this;
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_identity(float64 epsilon) const
    {
        return is_equal_eps(sx,  (float64)1.0, epsilon) &&
               is_equal_eps(shy, (float64)0.0, epsilon) &&
               is_equal_eps(shx, (float64)0.0, epsilon) && 
               is_equal_eps(sy,  (float64)1.0, epsilon) &&
               is_equal_eps(tx,  (float64)0.0, epsilon) &&
               is_equal_eps(ty,  (float64)0.0, epsilon);
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_valid(float64 epsilon) const
    {
        return fabs(sx) > epsilon && fabs(sy) > epsilon;
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_equal(const trans_affine& m, float64 epsilon) const
    {
        return is_equal_eps(sx,  m.sx,  epsilon) &&
               is_equal_eps(shy, m.shy, epsilon) &&
               is_equal_eps(shx, m.shx, epsilon) && 
               is_equal_eps(sy,  m.sy,  epsilon) &&
               is_equal_eps(tx,  m.tx,  epsilon) &&
               is_equal_eps(ty,  m.ty,  epsilon);
    }

    //------------------------------------------------------------------------
    float64 trans_affine::rotation() const
    {
        float64 x1 = 0.0;
        float64 y1 = 0.0;
        float64 x2 = 1.0;
        float64 y2 = 0.0;
        transform(&x1, &y1);
        transform(&x2, &y2);
        return atan2(y2-y1, x2-x1);
    }

    //------------------------------------------------------------------------
    void trans_affine::translation(float64* dx, float64* dy) const
    {
        *dx = tx;
        *dy = ty;
    }

    //------------------------------------------------------------------------
    void trans_affine::scaling(float64* x, float64* y) const
    {
        float64 x1 = 0.0;
        float64 y1 = 0.0;
        float64 x2 = 1.0;
        float64 y2 = 1.0;
        trans_affine t(*this);
        t *= trans_affine_rotation(-rotation());
        t.transform(&x1, &y1);
        t.transform(&x2, &y2);
        *x = x2 - x1;
        *y = y2 - y1;
    }


}

