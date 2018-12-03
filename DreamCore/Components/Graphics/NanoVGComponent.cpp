/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "NanoVGComponent.h"
#include "../Window/IWindowComponent.h"
#include "../../Scene/SceneRuntime.h"
#include "../Scripting/ScriptInstance.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#define NANOVG_GL3_IMPLEMENTATION
extern "C" {
    #include "../../deps/nanovg/nanovg_gl.h"
}
namespace Dream
{

    NanoVGComponent::NanoVGComponent(IWindowComponent* windowComponentHandle)
        : IComponent (),
          mWindowComponentHandle(windowComponentHandle)
    {
        setLogClassName("NanoVGComponent");
        auto log = getLog();
        log->trace("Constructing");
    }

    NanoVGComponent::~NanoVGComponent()
    {
        auto log = getLog();
        log->trace("Destructing");
        nvgDeleteGL3(mContext);
        mContext = nullptr;
    }

    bool NanoVGComponent::init()
    {
        auto log = getLog();
        log->info("Creating NanoVG Context");
        mContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        return mContext != nullptr;
    }

    void NanoVGComponent::updateComponent(SceneRuntime*)
    {

    }

    bool
    NanoVGComponent::render
    (SceneRuntime* sr)
    {
        auto log = getLog();
        log->info( "UpdateNanoVG Called" );
        auto scriptObj = sr->getNanoVGScript();
        if (sr)
        {
            BeginFrame();
            scriptObj->executeOnNanoVG(this,sr);
            EndFrame();
            return true;
        }
        return false;
    }

    void NanoVGComponent::BeginFrame()
    {
        BeginFrame(mWindowComponentHandle->getWidth(), mWindowComponentHandle->getHeight(), 1.0f);
    }

    void NanoVGComponent::BeginFrame(float windowWidth, float windowHeight, float devicePixelRatio)
    {
        nvgBeginFrame(mContext,windowWidth,windowHeight,devicePixelRatio);
    }

    void NanoVGComponent::CancelFrame()
    {
        nvgCancelFrame(mContext);
    }

    void NanoVGComponent::EndFrame()
    {
        nvgEndFrame(mContext);
    }

    void NanoVGComponent::GlobalCompositeOperation(int op)
    {
        nvgGlobalCompositeOperation(mContext,op);
    }

    void NanoVGComponent::GlobalCompositeBlendFunc(int sfactor, int dfactor)
    {
        nvgGlobalCompositeBlendFunc(mContext,sfactor,dfactor);
    }

    void NanoVGComponent::GlobalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha)
    {
        nvgGlobalCompositeBlendFuncSeparate(mContext,srcRGB,dstRGB,srcAlpha,dstAlpha);
    }

    NVGcolor NanoVGComponent::_RGB(unsigned char r, unsigned char g, unsigned char b)
    {
        return nvgRGB(r,g,b);
    }

    NVGcolor NanoVGComponent::RGBf(float r, float g, float b)
    {
        return nvgRGBf(r,g,b);
    }

    NVGcolor NanoVGComponent::RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        return nvgRGBA(r,g,b,a);
    }

    NVGcolor NanoVGComponent::RGBAf(float r, float g, float b, float a)
    {
        return nvgRGBAf(r,g,b,a);
    }

    NVGcolor NanoVGComponent::LerpRGBA(NVGcolor c0, NVGcolor c1, float u)
    {
        return nvgLerpRGBA(c0,c1,u);
    }

    NVGcolor NanoVGComponent::TransRGBA(NVGcolor c0, unsigned char a)
    {
        return nvgTransRGBA(c0,a);
    }

    NVGcolor NanoVGComponent::TransRGBAf(NVGcolor c0, float a)
    {
        return nvgTransRGBAf(c0,a);
    }

    NVGcolor NanoVGComponent::HSL(float h, float s, float l)
    {
        return nvgHSL(h,s,l);
    }

    NVGcolor NanoVGComponent::HSLA(float h, float s, float l, unsigned char a)
    {
        return nvgHSLA(h,s,l,a);
    }

    void NanoVGComponent::Save()
    {
        nvgSave(mContext);
    }

    void NanoVGComponent::Restore()
    {
        nvgRestore(mContext);
    }

    void NanoVGComponent::Reset()
    {
        nvgReset(mContext);
    }

    void NanoVGComponent::ShapeAntiAlias(int enabled)
    {
        nvgShapeAntiAlias(mContext,enabled);
    }

    void NanoVGComponent::StrokeColor(NVGcolor color)
    {
        nvgStrokeColor(mContext,color);
    }

    void NanoVGComponent::StrokePaint(NVGpaint paint)
    {
        nvgStrokePaint(mContext, paint);
    }

    void NanoVGComponent::FillColor(NVGcolor color)
    {
        nvgFillColor(mContext,color);
    }

    void NanoVGComponent::FillPaint(NVGpaint paint)
    {
        nvgFillPaint(mContext,paint);
    }

    void NanoVGComponent::MiterLimit(float limit)
    {
        nvgMiterLimit(mContext,limit);
    }

    void NanoVGComponent::StrokeWidth(float size)
    {
        nvgStrokeWidth(mContext,size);
    }

    void NanoVGComponent::LineCap(int cap)
    {
        nvgLineCap(mContext,cap);
    }

    void NanoVGComponent::LineJoin(int join)
    {
        nvgLineJoin(mContext,join);
    }

    void NanoVGComponent::GlobalAlpha(float alpha)
    {
        nvgGlobalAlpha(mContext,alpha);
    }

    void NanoVGComponent::ResetTransform()
    {
        nvgResetTransform(mContext);
    }

    void NanoVGComponent::Transform(float a, float b, float c, float d, float e, float f)
    {
        nvgTransform(mContext,a,b,c,d,e,f);
    }

    void NanoVGComponent::Translate(float x, float y)
    {
        nvgTranslate(mContext,x,y);
    }

    void NanoVGComponent::Rotate(float angle)
    {
        nvgRotate(mContext,angle);
    }

    void NanoVGComponent::SkewX(float angle)
    {
        nvgSkewX(mContext,angle);
    }

    void NanoVGComponent::SkewY(float angle)
    {
        nvgSkewY(mContext,angle);
    }

    void NanoVGComponent::Scale(float x, float y)
    {
        nvgScale(mContext,x,y);
    }

    void NanoVGComponent::CurrentTransform(float* xform)
    {
        nvgCurrentTransform(mContext,xform);
    }

    void NanoVGComponent::TransformIdentity(float* dst)
    {
        nvgTransformIdentity(dst);
    }

    void NanoVGComponent::TransformTranslate(float* dst, float tx, float ty)
    {
        nvgTransformTranslate(dst,tx,ty);
    }

    void NanoVGComponent::TransformScale(float* dst, float sx, float sy)
    {
        nvgTransformScale(dst,sx,sy);
    }

    void NanoVGComponent::TransformRotate(float* dst, float a)
    {
        nvgTransformRotate(dst,a);
    }

    void NanoVGComponent::TransformSkewX(float* dst, float a)
    {
        nvgTransformSkewX(dst,a);
    }

    void NanoVGComponent::TransformSkewY(float* dst, float a)
    {
        nvgTransformSkewY(dst,a);
    }

    void NanoVGComponent::TransformMultiply(float* dst, const float* src)
    {
        nvgTransformMultiply(dst,src);
    }

    void NanoVGComponent::TransformPremultiply(float* dst, const float* src)
    {
        nvgTransformPremultiply(dst,src);
    }

    int NanoVGComponent::TransformInverse(float* dst, const float* src)
    {
        return nvgTransformInverse(dst,src);
    }

    void NanoVGComponent::TransformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy)
    {
        nvgTransformPoint(dstx,dsty,xform,srcx,srcy);
    }

    float NanoVGComponent::DegToRad(float deg)
    {
        return nvgDegToRad(deg);
    }

    float NanoVGComponent::RadToDeg(float rad)
    {

        return nvgRadToDeg(rad);
    }

    int NanoVGComponent::CreateImage(const char* filename, int imageFlags)
    {
        return nvgCreateImage(mContext,filename,imageFlags);
    }

    int NanoVGComponent::CreateImageMem(int imageFlags, unsigned char* data, int ndata)
    {
        return nvgCreateImageMem(mContext,imageFlags,data,ndata);
    }

    int NanoVGComponent::CreateImageRGBA(int w, int h, int imageFlags, const unsigned char* data)
    {
        return nvgCreateImageRGBA(mContext,w,h,imageFlags,data);
    }

    void NanoVGComponent::UpdateImage(int image, const unsigned char* data)
    {
        nvgUpdateImage(mContext,image,data);
    }

    void NanoVGComponent::ImageSize(int image, int* w, int* h)
    {
        nvgImageSize(mContext,image,w,h);
    }

    void NanoVGComponent::DeleteImage(int image)
    {
        nvgDeleteImage(mContext,image);
    }

    NVGpaint NanoVGComponent::LinearGradient(float sx, float sy, float ex, float ey, NVGcolor icol, NVGcolor ocol)
    {

        return nvgLinearGradient(mContext,sx,sy,ex,ey,icol,ocol);
    }

    NVGpaint NanoVGComponent::BoxGradient(float x, float y, float w, float h, float r, float f, NVGcolor icol, NVGcolor ocol)
    {
        return nvgBoxGradient(mContext,x,y,w,h,r,f,icol,ocol);
    }

    NVGpaint NanoVGComponent::RadialGradient(float cx, float cy, float inr, float outr, NVGcolor icol, NVGcolor ocol)
    {
        return nvgRadialGradient(mContext,cx,cy,inr,outr,icol,ocol);
    }

    NVGpaint NanoVGComponent::ImagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha)
    {
        return nvgImagePattern(mContext,ox,oy,ex,ey,angle,image,alpha);
    }

    void NanoVGComponent::Scissor(float x, float y, float w, float h)
    {
        nvgScissor(mContext,x,y,w,h);
    }

    void NanoVGComponent::IntersectScissor(float x, float y, float w, float h)
    {
        nvgIntersectScissor(mContext,x,y,w,h);
    }

    void NanoVGComponent::ResetScissor()
    {
        nvgResetScissor(mContext);
    }

    void NanoVGComponent::BeginPath()
    {
        nvgBeginPath(mContext);
    }

    void NanoVGComponent::MoveTo(float x, float y)
    {
        nvgMoveTo(mContext,x,y);
    }

    void NanoVGComponent::LineTo(float x, float y)
    {
        nvgLineTo(mContext,x,y);
    }

    void NanoVGComponent::BezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y)
    {
        nvgBezierTo(mContext,c1x,c1y,c2x,c2y,x,y);
    }

    void NanoVGComponent::QuadTo(float cx, float cy, float x, float y)
    {

        nvgQuadTo(mContext,cx,cy,x,y);
    }

    void NanoVGComponent::ArcTo(float x1, float y1, float x2, float y2, float radius)
    {
        nvgArcTo(mContext,x1,y1,x2,y2,radius);
    }

    void NanoVGComponent::ClosePath()
    {
        nvgClosePath(mContext);
    }

    void NanoVGComponent::PathWinding(int dir)
    {
        nvgPathWinding(mContext,dir);
    }

    void NanoVGComponent::Arc(float cx, float cy, float r, float a0, float a1, int dir)
    {
        nvgArc(mContext,cx,cy,r,a0,a1,dir);
    }

    void NanoVGComponent::Rect(float x, float y, float w, float h)
    {
        nvgRect(mContext,x,y,w,h);
    }

    void NanoVGComponent::RoundedRect(float x, float y, float w, float h, float r)
    {
        nvgRoundedRect(mContext,x,y,w,h,r);
    }

    void NanoVGComponent::RoundedRectVarying
    (float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft)
    {
        nvgRoundedRectVarying(mContext,x,y,w,h,radTopLeft,radTopRight,radBottomLeft,radBottomRight);
    }

    void NanoVGComponent::Ellipse(float cx, float cy, float rx, float ry)
    {
        nvgEllipse(mContext,cx,cy,rx,ry);
    }

    void NanoVGComponent::Circle(float cx, float cy, float r)
    {
        nvgCircle(mContext,cx,cy,r);
    }

    void NanoVGComponent::Fill()
    {
        nvgFill(mContext);
    }

    void NanoVGComponent::Stroke()
    {
        nvgStroke(mContext);
    }

    int NanoVGComponent::CreateFont(const char* name, const char* filename)
    {
        return nvgCreateFont(mContext,name,filename);
    }

    int NanoVGComponent::CreateFontMem
    (const char* name, unsigned char* data, int ndata, int freeData)
    {
        return nvgCreateFontMem(mContext,name,data,ndata,freeData);
    }

    int NanoVGComponent::FindFont(const char* name)
    {
        return nvgFindFont(mContext,name);
    }

    int NanoVGComponent::AddFallbackFontId(int baseFont, int fallbackFont)
    {
        return nvgAddFallbackFontId(mContext,baseFont,fallbackFont);
    }

    int NanoVGComponent::AddFallbackFont(const char* baseFont, const char* fallbackFont)
    {
        return nvgAddFallbackFont(mContext,baseFont,fallbackFont);
    }

    void NanoVGComponent::FontSize(float size)
    {
        nvgFontSize(mContext,size);
    }

    void NanoVGComponent::FontBlur(float blur)
    {
        nvgFontBlur(mContext,blur);
    }

    void NanoVGComponent::TextLetterSpacing(float spacing)
    {
        nvgTextLetterSpacing(mContext,spacing);
    }

    void NanoVGComponent::TextLineHeight(float lineHeight)
    {
        nvgTextLineHeight(mContext,lineHeight);
    }

    void NanoVGComponent::TextAlign(int align)
    {
        nvgTextAlign(mContext,align);
    }

    void NanoVGComponent::FontFaceId(int font)
    {
        nvgFontFaceId(mContext,font);
    }

    void NanoVGComponent::FontFace(const char* font)
    {
        nvgFontFace(mContext,font);
    }

    float NanoVGComponent::Text(float x, float y, const char* string)
    {
        return nvgText(mContext,x,y,string,nullptr);
    }


    float NanoVGComponent::Text(float x, float y, const char* string, const char* end)
    {
        return nvgText(mContext,x,y,string,end);
    }

    void NanoVGComponent::TextBox(float x, float y, float breakRowWidth, const char* string, const char* end)
    {
        nvgTextBox(mContext,x,y,breakRowWidth,string,end);
    }

    float NanoVGComponent::TextBounds(float x, float y, const char* string, const char* end, float* bounds)
    {
        return nvgTextBounds(mContext,x,y,string,end,bounds);
    }

    void NanoVGComponent::TextBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds)
    {
        nvgTextBoxBounds(mContext,x,y,breakRowWidth,string,end,bounds);
    }

    int NanoVGComponent::TextGlyphPositions
    (float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions)
    {
        return nvgTextGlyphPositions(mContext,x,y,string,end,positions,maxPositions);
    }

    void NanoVGComponent::NanoVGComponent::TextMetrics
    (float* ascender, float* descender, float* lineh)
    {
        nvgTextMetrics(mContext,ascender,descender,lineh);
    }

    int NanoVGComponent::NanoVGComponent::TextBreakLines
    (const char* string, const char* end, float breakRowWidth, NVGtextRow* rows, int maxRows)
    {
        return nvgTextBreakLines(mContext,string,end,breakRowWidth,rows,maxRows);
    }
}
