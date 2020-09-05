//----------------------------------------------------------------------------
// Agg2D - Version 1.0
// Based on Anti-Grain Geometry
// Copyright (C) 2005 Maxim Shemanarev (http://www.antigrain.com)
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
//----------------------------------------------------------------------------
//
//	25 Jan 2007 - Ported to AGG 2.4 Jerry Evans (jerry@novadsp.com)
//
//----------------------------------------------------------------------------

#ifndef AGG2D_INCLUDED
#define AGG2D_INCLUDED

// With this define uncommented you can use FreeType font engine
#define AGG2D_USE_FREETYPE

// With this define uncommented you can use floating-point pixel format
//#define AGG2D_USE_FLOAT_FORMAT

#include "agg_basics.h"
#include "agg_trans_affine.h"
#include "agg_trans_viewport.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"
#include "agg_conv_transform.h"
#include "agg_conv_curve.h"
#include "agg_rendering_buffer.h"
#include "agg_renderer_base.h"
#include "agg_renderer_scanline.h"
#include "agg_span_gradient.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_allocator.h"
#include "agg_span_converter.h"
#include "agg_span_interpolator_linear.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_gamma_functions.h"
#include "agg_scanline_u.h"
#include "agg_bezier_arc.h"
#include "agg_rounded_rect.h"
#include "agg_font_cache_manager.h"

#ifdef AGG2D_USE_FREETYPE
#include "agg_font_freetype.h"
#else
#include "agg_font_win32_tt.h"
#endif

#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_rgb_packed.h"
#include "agg_image_accessors.h"

class Agg2D
{
#ifdef AGG2D_USE_FLOAT_FORMAT
    typedef agg::rgba32 ColorType;
#else
    typedef agg::rgba8  ColorType;
#endif
    typedef agg::order_rgba 										 ComponentOrder; // Platform dependent!
    typedef agg::blender_rgba<ColorType, ComponentOrder>             Blender;
    typedef agg::comp_op_adaptor_rgba<ColorType, ComponentOrder>     BlenderComp;
    typedef agg::blender_rgba_pre<ColorType, ComponentOrder>         BlenderPre;
    typedef agg::comp_op_adaptor_rgba_pre<ColorType, ComponentOrder> BlenderCompPre;

    typedef agg::pixfmt_alpha_blend_rgba<Blender, agg::rendering_buffer>         PixFormat;
    typedef agg::pixfmt_custom_blend_rgba<BlenderComp, agg::rendering_buffer>    PixFormatComp;
    typedef agg::pixfmt_alpha_blend_rgba<BlenderPre, agg::rendering_buffer>      PixFormatPre;
    typedef agg::pixfmt_custom_blend_rgba<BlenderCompPre, agg::rendering_buffer> PixFormatCompPre;

    typedef agg::renderer_base<PixFormat>        RendererBase;
    typedef agg::renderer_base<PixFormatComp>    RendererBaseComp;
    typedef agg::renderer_base<PixFormatPre>     RendererBasePre;
    typedef agg::renderer_base<PixFormatCompPre> RendererBaseCompPre;

    typedef agg::renderer_scanline_aa_solid<RendererBase>     RendererSolid;
    typedef agg::renderer_scanline_aa_solid<RendererBaseComp> RendererSolidComp;

    typedef agg::span_allocator<ColorType> SpanAllocator;
    typedef agg::pod_auto_array<ColorType, 256> GradientArray;

    typedef agg::span_gradient<ColorType, agg::span_interpolator_linear<>, agg::gradient_x,      GradientArray> LinearGradientSpan;
    typedef agg::span_gradient<ColorType, agg::span_interpolator_linear<>, agg::gradient_circle, GradientArray> RadialGradientSpan;

#ifdef AGG2D_USE_FREETYPE
    typedef agg::font_engine_freetype_int32       FontEngine;
#else
    typedef agg::font_engine_win32_tt_int32       FontEngine;
#endif
    typedef agg::font_cache_manager<FontEngine>   FontCacheManager;
    typedef FontCacheManager::gray8_adaptor_type  FontRasterizer;
    typedef FontCacheManager::gray8_scanline_type FontScanline;

    typedef agg::conv_curve<agg::path_storage>    ConvCurve;
    typedef agg::conv_stroke<ConvCurve>           ConvStroke;
    typedef agg::conv_transform<ConvCurve>        PathTransform;
    typedef agg::conv_transform<ConvStroke>       StrokeTransform;

    enum Gradient
    {
        Solid,
        Linear,
        Radial
    };

public:
    friend class Agg2DRenderer;

    // Use srgba8 as the "user" color type, even though the underlying color type 
    // might be something else, such as rgba32. This allows code based on 
    // 8-bit sRGB values to carry on working as before.
    typedef agg::srgba8       Color;
    typedef agg::rect_i       Rect;
    typedef agg::rect_f       RectF;
    typedef agg::trans_affine Affine;

    enum LineJoin
    {
        JoinMiter = agg::miter_join,
        JoinRound = agg::round_join,
        JoinBevel = agg::bevel_join
    };

    enum LineCap
    {
        CapButt   = agg::butt_cap,
        CapSquare = agg::square_cap,
        CapRound  = agg::round_cap
    };

    enum TextAlignment
    {
        AlignLeft,
        AlignRight,
        AlignCenter,
        AlignTop = AlignRight,
        AlignBottom = AlignLeft
    };


    enum DrawPathFlag
    {
        FillOnly,
        StrokeOnly,
        FillAndStroke,
        FillWithLineColor
    };

    enum ViewportOption
    {
        Anisotropic,
        XMinYMin,
        XMidYMin,
        XMaxYMin,
        XMinYMid,
        XMidYMid,
        XMaxYMid,
        XMinYMax,
        XMidYMax,
        XMaxYMax
    };

    struct Transformations
    {
        agg::float64 affineMatrix[6];
    };


    struct Image
    {
        agg::rendering_buffer renBuf;

        Image() {}
        Image(unsigned char* buf, unsigned width, unsigned height, int stride) :
            renBuf(buf, width, height, stride) {}
        void attach(unsigned char* buf, unsigned width, unsigned height, int stride)
        {
            renBuf.attach(buf, width, height, stride);
        }
        int width()  const { return renBuf.width(); }
        int height() const { return renBuf.height(); }
        void premultiply();
        void demultiply();
    };

    enum ImageFilter
    {
        NoFilter,
        Bilinear,
        Hanning,
        Hermite,
        Quadric,
        Bicubic,
        Catrom,
        Spline16,
        Spline36,
        Blackman144
    };

    enum ImageResample
    {
        NoResample,
        ResampleAlways,
        ResampleOnZoomOut
    };

    enum FontCacheType
    {
        RasterFontCache,
        VectorFontCache
    };

    enum BlendMode
    {
        BlendAlpha      = agg::end_of_comp_op_e,
        BlendClear      = agg::comp_op_clear,
        BlendSrc        = agg::comp_op_src,
        BlendDst        = agg::comp_op_dst,
        BlendSrcOver    = agg::comp_op_src_over,
        BlendDstOver    = agg::comp_op_dst_over,
        BlendSrcIn      = agg::comp_op_src_in,
        BlendDstIn      = agg::comp_op_dst_in,
        BlendSrcOut     = agg::comp_op_src_out,
        BlendDstOut     = agg::comp_op_dst_out,
        BlendSrcAtop    = agg::comp_op_src_atop,
        BlendDstAtop    = agg::comp_op_dst_atop,
        BlendXor        = agg::comp_op_xor,
        BlendAdd        = agg::comp_op_plus,
        BlendMultiply   = agg::comp_op_multiply,
        BlendScreen     = agg::comp_op_screen,
        BlendOverlay    = agg::comp_op_overlay,
        BlendDarken     = agg::comp_op_darken,
        BlendLighten    = agg::comp_op_lighten,
        BlendColorDodge = agg::comp_op_color_dodge,
        BlendColorBurn  = agg::comp_op_color_burn,
        BlendHardLight  = agg::comp_op_hard_light,
        BlendSoftLight  = agg::comp_op_soft_light,
        BlendDifference = agg::comp_op_difference,
        BlendExclusion  = agg::comp_op_exclusion,
    };

    enum Direction
    {
        CW, CCW
    };

    ~Agg2D();
    Agg2D();

    // Setup
    //-----------------------
    void  attach(unsigned char* buf, unsigned width, unsigned height, int stride);
    void  attach(Image& img);

    void  clipBox(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2);
    RectF clipBox() const;

    void  clearAll(Color c);
    void  clearAll(unsigned r, unsigned g, unsigned b, unsigned a = 255);

    void  clearClipBox(Color c);
    void  clearClipBox(unsigned r, unsigned g, unsigned b, unsigned a = 255);

    // Conversions
    //-----------------------
    void   worldToScreen(agg::float64& x, agg::float64& y) const;
    void   screenToWorld(agg::float64& x, agg::float64& y) const;
    agg::float64 worldToScreen(agg::float64 scalar) const;
    agg::float64 screenToWorld(agg::float64 scalar) const;
    void   alignPoint(agg::float64& x, agg::float64& y) const;
    bool   inBox(agg::float64 worldX, agg::float64 worldY) const;

    // General Attributes
    //-----------------------
    void blendMode(BlendMode m);
    BlendMode blendMode() const;

    void imageBlendMode(BlendMode m);
    BlendMode imageBlendMode() const;

    void imageBlendColor(Color c);
    void imageBlendColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    Color imageBlendColor() const;

    void masterAlpha(agg::float64 a);
    agg::float64 masterAlpha() const;

    void antiAliasGamma(agg::float64 g);
    agg::float64 antiAliasGamma() const;

    void fillColor(Color c);
    void fillColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    void noFill();

    void lineColor(Color c);
    void lineColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);
    void noLine();

    Color fillColor() const;
    Color lineColor() const;

    void fillLinearGradient(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2, Color c1, Color c2, agg::float64 profile=1.0);
    void lineLinearGradient(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2, Color c1, Color c2, agg::float64 profile=1.0);

    void fillRadialGradient(agg::float64 x, agg::float64 y, agg::float64 r, Color c1, Color c2, agg::float64 profile=1.0);
    void lineRadialGradient(agg::float64 x, agg::float64 y, agg::float64 r, Color c1, Color c2, agg::float64 profile=1.0);

    void fillRadialGradient(agg::float64 x, agg::float64 y, agg::float64 r, Color c1, Color c2, Color c3);
    void lineRadialGradient(agg::float64 x, agg::float64 y, agg::float64 r, Color c1, Color c2, Color c3);

    void fillRadialGradient(agg::float64 x, agg::float64 y, agg::float64 r);
    void lineRadialGradient(agg::float64 x, agg::float64 y, agg::float64 r);

    void lineWidth(agg::float64 w);
    agg::float64 lineWidth() const;
	
    void lineCap(LineCap cap);
    LineCap lineCap() const;

    void lineJoin(LineJoin join);
    LineJoin lineJoin() const;

    void fillEvenOdd(bool evenOddFlag);
    bool fillEvenOdd() const;

    // Transformations
    //-----------------------
    Transformations transformations() const;
    void transformations(const Transformations& tr);
    void resetTransformations();
    void affine(const Affine& tr);
    void affine(const Transformations& tr);
    void rotate(agg::float64 angle);
    void scale(agg::float64 sx, agg::float64 sy);
    void skew(agg::float64 sx, agg::float64 sy);
    void translate(agg::float64 x, agg::float64 y);
    void parallelogram(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2, const agg::float64* para);
    void viewport(agg::float64 worldX1,  agg::float64 worldY1,  agg::float64 worldX2,  agg::float64 worldY2,
                  agg::float64 screenX1, agg::float64 screenY1, agg::float64 screenX2, agg::float64 screenY2,
                  ViewportOption opt=XMidYMid);

    // Basic Shapes
    //-----------------------
    void line(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2);
    void triangle(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2, agg::float64 x3, agg::float64 y3);
    void rectangle(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2);
    void roundedRect(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2, agg::float64 r);
    void roundedRect(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2, agg::float64 rx, agg::float64 ry);
    void roundedRect(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2,
                     agg::float64 rxBottom, agg::float64 ryBottom,
                     agg::float64 rxTop,    agg::float64 ryTop);
    void ellipse(agg::float64 cx, agg::float64 cy, agg::float64 rx, agg::float64 ry);
    void arc(agg::float64 cx, agg::float64 cy, agg::float64 rx, agg::float64 ry, agg::float64 start, agg::float64 sweep);
    void star(agg::float64 cx, agg::float64 cy, agg::float64 r1, agg::float64 r2, agg::float64 startAngle, int numRays);
    void curve(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2, agg::float64 x3, agg::float64 y3);
    void curve(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2, agg::float64 x3, agg::float64 y3, agg::float64 x4, agg::float64 y4);
    void polygon(agg::float64* xy, int numPoints);
    void polyline(agg::float64* xy, int numPoints);


    // Text
    //-----------------------
    void   flipText(bool flip);
    void   font(const char* fileName, agg::float64 height,
                bool bold = false,
                bool italic = false,
                FontCacheType ch = RasterFontCache,
                agg::float64 angle = 0.0);
    agg::float64 fontHeight() const;
    void   textAlignment(TextAlignment alignX, TextAlignment alignY);
    bool   textHints() const;
    void   textHints(bool hints);
    agg::float64 textWidth(const char* str);
    void   text(agg::float64 x, agg::float64 y, const char* str, bool roundOff=false, agg::float64 dx=0.0, agg::float64 dy=0.0);

    // Path commands
    //-----------------------
    void resetPath();

    void moveTo(agg::float64 x, agg::float64 y);
    void moveRel(agg::float64 dx, agg::float64 dy);

    void lineTo(agg::float64 x, agg::float64 y);
    void lineRel(agg::float64 dx, agg::float64 dy);

    void horLineTo(agg::float64 x);
    void horLineRel(agg::float64 dx);

    void verLineTo(agg::float64 y);
    void verLineRel(agg::float64 dy);

    void arcTo(agg::float64 rx, agg::float64 ry,
               agg::float64 angle,
               bool largeArcFlag,
               bool sweepFlag,
               agg::float64 x, agg::float64 y);

    void arcRel(agg::float64 rx, agg::float64 ry,
                agg::float64 angle,
                bool largeArcFlag,
                bool sweepFlag,
                agg::float64 dx, agg::float64 dy);

    void quadricCurveTo(agg::float64 xCtrl, agg::float64 yCtrl,
                         agg::float64 xTo,   agg::float64 yTo);
    void quadricCurveRel(agg::float64 dxCtrl, agg::float64 dyCtrl,
                         agg::float64 dxTo,   agg::float64 dyTo);
    void quadricCurveTo(agg::float64 xTo, agg::float64 yTo);
    void quadricCurveRel(agg::float64 dxTo, agg::float64 dyTo);

    void cubicCurveTo(agg::float64 xCtrl1, agg::float64 yCtrl1,
                      agg::float64 xCtrl2, agg::float64 yCtrl2,
                      agg::float64 xTo,    agg::float64 yTo);

    void cubicCurveRel(agg::float64 dxCtrl1, agg::float64 dyCtrl1,
                       agg::float64 dxCtrl2, agg::float64 dyCtrl2,
                       agg::float64 dxTo,    agg::float64 dyTo);

    void cubicCurveTo(agg::float64 xCtrl2, agg::float64 yCtrl2,
                      agg::float64 xTo,    agg::float64 yTo);

    void cubicCurveRel(agg::float64 xCtrl2, agg::float64 yCtrl2,
                       agg::float64 xTo,    agg::float64 yTo);

    void addEllipse(agg::float64 cx, agg::float64 cy, agg::float64 rx, agg::float64 ry, Direction dir);
    void closePolygon();

    void drawPath(DrawPathFlag flag = FillAndStroke);
    void drawPathNoTransform(DrawPathFlag flag = FillAndStroke);


    // Image Transformations
    //-----------------------
    void imageFilter(ImageFilter f);
    ImageFilter imageFilter() const;

    void imageResample(ImageResample f);
    ImageResample imageResample() const;

    void transformImage(const Image& img,
                           int imgX1,    int imgY1,    int imgX2,    int imgY2,
                        agg::float64 dstX1, agg::float64 dstY1, agg::float64 dstX2, agg::float64 dstY2);

    void transformImage(const Image& img,
                        agg::float64 dstX1, agg::float64 dstY1, agg::float64 dstX2, agg::float64 dstY2);

    void transformImage(const Image& img,
                        int imgX1, int imgY1, int imgX2, int imgY2,
                        const agg::float64* parallelogram);

    void transformImage(const Image& img, const agg::float64* parallelogram);


    void transformImagePath(const Image& img,
                               int imgX1,    int imgY1,    int imgX2,    int imgY2,
                            agg::float64 dstX1, agg::float64 dstY1, agg::float64 dstX2, agg::float64 dstY2);

    void transformImagePath(const Image& img,
                            agg::float64 dstX1, agg::float64 dstY1, agg::float64 dstX2, agg::float64 dstY2);

    void transformImagePath(const Image& img,
                            int imgX1, int imgY1, int imgX2, int imgY2,
                            const agg::float64* parallelogram);

    void transformImagePath(const Image& img, const agg::float64* parallelogram);


    // Image Blending (no transformations available)
    void blendImage(Image& img,
                    int imgX1, int imgY1, int imgX2, int imgY2,
                    agg::float64 dstX, agg::float64 dstY, unsigned alpha=255);
    void blendImage(Image& img, agg::float64 dstX, agg::float64 dstY, unsigned alpha=255);


    // Copy image directly, together with alpha-channel
    void copyImage(Image& img,
                   int imgX1, int imgY1, int imgX2, int imgY2,
                   agg::float64 dstX, agg::float64 dstY);
    void copyImage(Image& img, agg::float64 dstX, agg::float64 dstY);


    // Auxiliary
    //-----------------------
    static agg::float64 pi() { return agg::pi; }
    static agg::float64 deg2Rad(agg::float64 v) { return v * agg::pi / 180.0; }
    static agg::float64 rad2Deg(agg::float64 v) { return v * 180.0 / agg::pi; }

private:
    void render(bool fillColor);
    void render(FontRasterizer& ras, FontScanline& sl);

    void addLine(agg::float64 x1, agg::float64 y1, agg::float64 x2, agg::float64 y2);
    void updateRasterizerGamma();
    void renderImage(const Image& img, int x1, int y1, int x2, int y2, const agg::float64* parl);

    agg::rendering_buffer           m_rbuf;
    PixFormat                       m_pixFormat;
    PixFormatComp                   m_pixFormatComp;
    PixFormatPre                    m_pixFormatPre;
    PixFormatCompPre                m_pixFormatCompPre;
    RendererBase                    m_renBase;
    RendererBaseComp                m_renBaseComp;
    RendererBasePre                 m_renBasePre;
    RendererBaseCompPre             m_renBaseCompPre;
    RendererSolid                   m_renSolid;
    RendererSolidComp               m_renSolidComp;

    SpanAllocator                   m_allocator;
    RectF                           m_clipBox;

    BlendMode                       m_blendMode;
    BlendMode                       m_imageBlendMode;
    Color                           m_imageBlendColor;

    agg::scanline_u8                m_scanline;
    agg::rasterizer_scanline_aa<>   m_rasterizer;

    agg::float64                          m_masterAlpha;
    agg::float64                          m_antiAliasGamma;

    Color                           m_fillColor;
    Color                           m_lineColor;
    GradientArray                   m_fillGradient;
    GradientArray                   m_lineGradient;

    LineCap                         m_lineCap;
    LineJoin                        m_lineJoin;

    Gradient                        m_fillGradientFlag;
    Gradient                        m_lineGradientFlag;
    agg::trans_affine               m_fillGradientMatrix;
    agg::trans_affine               m_lineGradientMatrix;
    agg::float64                          m_fillGradientD1;
    agg::float64                          m_lineGradientD1;
    agg::float64                          m_fillGradientD2;
    agg::float64                          m_lineGradientD2;

    agg::float64                          m_textAngle;
    TextAlignment                   m_textAlignX;
    TextAlignment                   m_textAlignY;
    bool                            m_textHints;
    agg::float64                          m_fontHeight;
    agg::float64                          m_fontAscent;
    agg::float64                          m_fontDescent;
    FontCacheType                   m_fontCacheType;

    ImageFilter                     m_imageFilter;
    ImageResample                   m_imageResample;
    agg::image_filter_lut           m_imageFilterLut;

    agg::span_interpolator_linear<> m_fillGradientInterpolator;
    agg::span_interpolator_linear<> m_lineGradientInterpolator;

    agg::gradient_x                 m_linearGradientFunction;
    agg::gradient_circle            m_radialGradientFunction;

    agg::float64                          m_lineWidth;
    bool                            m_evenOddFlag;

    agg::path_storage               m_path;
    agg::trans_affine               m_transform;

    ConvCurve                       m_convCurve;
    ConvStroke                      m_convStroke;

    PathTransform                   m_pathTransform;
    StrokeTransform                 m_strokeTransform;

#ifndef AGG2D_USE_FREETYPE
    HDC                             m_fontDC;
#endif
    FontEngine                      m_fontEngine;
    FontCacheManager                m_fontCacheManager;
};


inline bool operator == (const Agg2D::Color& c1, const Agg2D::Color& c2)
{
   return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

inline bool operator != (const Agg2D::Color& c1, const Agg2D::Color& c2)
{
   return !(c1 == c2);
}


#endif


