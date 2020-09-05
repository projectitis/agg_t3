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
// Arc generator. Produces at most 4 consecutive cubic bezier curves, i.e., 
// 4, 7, 10, or 13 vertices.
//
//----------------------------------------------------------------------------

#ifndef AGG_BEZIER_ARC_INCLUDED
#define AGG_BEZIER_ARC_INCLUDED

#include "agg_conv_transform.h"

namespace agg
{

    //-----------------------------------------------------------------------
    void arc_to_bezier(float64 cx, float64 cy, float64 rx, float64 ry, 
                       float64 start_angle, float64 sweep_angle,
                       float64* curve);


    //==============================================================bezier_arc
    // 
    // See implemantaion agg_bezier_arc.cpp
    //
    class bezier_arc
    {
    public:
        //--------------------------------------------------------------------
        bezier_arc() : m_vertex(26), m_num_vertices(0), m_cmd(path_cmd_line_to) {}
        bezier_arc(float64 x,  float64 y, 
                   float64 rx, float64 ry, 
                   float64 start_angle, 
                   float64 sweep_angle)
        {
            init(x, y, rx, ry, start_angle, sweep_angle);
        }

        //--------------------------------------------------------------------
        void init(float64 x,  float64 y, 
                  float64 rx, float64 ry, 
                  float64 start_angle, 
                  float64 sweep_angle);

        //--------------------------------------------------------------------
        void rewind(unsigned)
        {
            m_vertex = 0;
        }

        //--------------------------------------------------------------------
        unsigned vertex(float64* x, float64* y)
        {
            if(m_vertex >= m_num_vertices) return path_cmd_stop;
            *x = m_vertices[m_vertex];
            *y = m_vertices[m_vertex + 1];
            m_vertex += 2;
            return (m_vertex == 2) ? unsigned(path_cmd_move_to) : m_cmd;
        }

        // Supplemantary functions. num_vertices() actually returns float64d 
        // number of vertices. That is, for 1 vertex it returns 2.
        //--------------------------------------------------------------------
        unsigned  num_vertices() const { return m_num_vertices; }
        const float64* vertices() const { return m_vertices;     }
        float64*       vertices()       { return m_vertices;     }
 
    private:
        unsigned m_vertex;
        unsigned m_num_vertices;
        float64   m_vertices[26];
        unsigned m_cmd;
    };



    //==========================================================bezier_arc_svg
    // Compute an SVG-style bezier arc. 
    //
    // Computes an elliptical arc from (x1, y1) to (x2, y2). The size and 
    // orientation of the ellipse are defined by two radii (rx, ry) 
    // and an x-axis-rotation, which indicates how the ellipse as a whole 
    // is rotated relative to the current coordinate system. The center 
    // (cx, cy) of the ellipse is calculated automatically to satisfy the 
    // constraints imposed by the other parameters. 
    // large-arc-flag and sweep-flag contribute to the automatic calculations 
    // and help determine how the arc is drawn.
    class bezier_arc_svg
    {
    public:
        //--------------------------------------------------------------------
        bezier_arc_svg() : m_arc(), m_radii_ok(false) {}

        bezier_arc_svg(float64 x1, float64 y1, 
                       float64 rx, float64 ry, 
                       float64 angle,
                       bool large_arc_flag,
                       bool sweep_flag,
                       float64 x2, float64 y2) : 
            m_arc(), m_radii_ok(false)
        {
            init(x1, y1, rx, ry, angle, large_arc_flag, sweep_flag, x2, y2);
        }

        //--------------------------------------------------------------------
        void init(float64 x1, float64 y1, 
                  float64 rx, float64 ry, 
                  float64 angle,
                  bool large_arc_flag,
                  bool sweep_flag,
                  float64 x2, float64 y2);

        //--------------------------------------------------------------------
        bool radii_ok() const { return m_radii_ok; }

        //--------------------------------------------------------------------
        void rewind(unsigned)
        {
            m_arc.rewind(0);
        }

        //--------------------------------------------------------------------
        unsigned vertex(float64* x, float64* y)
        {
            return m_arc.vertex(x, y);
        }

        // Supplemantary functions. num_vertices() actually returns float64d 
        // number of vertices. That is, for 1 vertex it returns 2.
        //--------------------------------------------------------------------
        unsigned  num_vertices() const { return m_arc.num_vertices(); }
        const float64* vertices() const { return m_arc.vertices();     }
        float64*       vertices()       { return m_arc.vertices();     }

    private:
        bezier_arc m_arc;
        bool       m_radii_ok;
    };




}


#endif
