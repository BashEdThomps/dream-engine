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


#pragma once

#include "../Component.h"
#include <glm/vec4.hpp>
extern "C" {
    #include "../../deps/nanovg/nanovg.h"
}

#ifdef CreateFont
#undef CreateFont
#endif

using glm::vec4;
typedef struct NVGcontext NVGcontext;

namespace Dream
{
    class WindowComponent;
    class SceneRuntime;

    class NanoVGComponent : public Component
    {
    public:
        NanoVGComponent(ProjectRuntime* pr, WindowComponent* windowComponent);
        ~NanoVGComponent() override;

        // Begin drawing a new frame
        // Calls to nanovg drawing API should be wrapped in BeginFrame() & EndFrame()
        // BeginFrame() defines the size of the window to render to in relation currently
        // set viewport (i.e. glViewport on GL backends). Device pixel ration allows to
        // control the rendering on Hi-DPI devices.
        // For example, GLFW returns two dimension for an opened window: window size and
        // frame buffer size. In that case you would set windowWidth/Height to the window size
        // devicePixelRatio to: frameBufferWidth / windowWidth.
        void BeginFrame(float windowWidth, float windowHeight, float devicePixelRatio);

        // Cancels drawing the current frame.
        void CancelFrame();

        // Ends drawing flushing remaining render state.
        void EndFrame();

        //
        // Composite operation
        //
        // The composite operations in NanoVG are modeled after HTML Canvas API, and
        // the blend func is based on OpenGL (see corresponding manuals for more info).
        // The colors in the blending state have premultiplied alpha.

        // Sets the composite operation. The op parameter should be one of NVGcompositeOperation.
        void GlobalCompositeOperation(int op);

        // Sets the composite operation with custom pixel arithmetic. The parameters should be one of NVGblendFactor.
        void GlobalCompositeBlendFunc(int sfactor, int dfactor);

        // Sets the composite operation with custom pixel arithmetic for RGB and alpha components separately. The parameters should be one of NVGblendFactor.
        void GlobalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha);

        //
        // Color utils
        //
        // Colors in NanoVG are stored as unsigned ints in ABGR format.

        // Returns a color value from red, green, blue values. Alpha will be set to 255 (1.0f).
        NVGcolor _RGB(unsigned char r, unsigned char g, unsigned char b);

        // Returns a color value from red, green, blue values. Alpha will be set to 1.0f.
        NVGcolor RGBf(float r, float g, float b);


        // Returns a color value from red, green, blue and alpha values.
        NVGcolor RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

        // Returns a color value from red, green, blue and alpha values.
        NVGcolor RGBAf(float r, float g, float b, float a);


        // Linearly interpolates from color c0 to c1, and returns resulting color value.
        NVGcolor LerpRGBA(NVGcolor c0, NVGcolor c1, float u);

        // Sets transparency of a color value.
        NVGcolor TransRGBA(NVGcolor c0, unsigned char a);

        // Sets transparency of a color value.
        NVGcolor TransRGBAf(NVGcolor c0, float a);

        // Returns color value specified by hue, saturation and lightness.
        // HSL values are all in range [0..1], alpha will be set to 255.
        NVGcolor HSL(float h, float s, float l);

        // Returns color value specified by hue, saturation and lightness and alpha.
        // HSL values are all in range [0..1], alpha in range [0..255]
        NVGcolor HSLA(float h, float s, float l, unsigned char a);

        //
        // State Handling
        //
        // NanoVG contains state which represents how paths will be rendered.
        // The state contains transform, fill and stroke styles, text and font styles,
        // and scissor clipping.

        // Pushes and saves the current render state into a state stack.
        // A matching Restore() must be used to restore the state.
        void Save();

        // Pops and restores current render state.
        void Restore();

        // Resets current render state to default values. Does not affect the render state stack.
        void Reset();

        //
        // Render styles
        //
        // Fill and stroke render style can be either a solid color or a paint which is a gradient or a pattern.
        // Solid color is simply defined as a color value, different kinds of paints can be created
        // using LinearGradient(), BoxGradient(), RadialGradient() and ImagePattern().
        //
        // Current render style can be saved and restored using Save() and Restore().

        // Sets whether to draw antialias for Stroke() and Fill(). It's enabled by default.
        void ShapeAntiAlias(int enabled);

        // Sets current stroke style to a solid color.
        void StrokeColor(NVGcolor color);

        // Sets current stroke style to a paint, which can be a one of the gradients or a pattern.
        void StrokePaint(NVGpaint paint);

        // Sets current fill style to a solid color.
        void FillColor(NVGcolor color);

        // Sets current fill style to a paint, which can be a one of the gradients or a pattern.
        void FillPaint(NVGpaint paint);

        // Sets the miter limit of the stroke style.
        // Miter limit controls when a sharp corner is beveled.
        void MiterLimit(float limit);

        // Sets the stroke width of the stroke style.
        void StrokeWidth(float size);

        // Sets how the end of the line (cap) is drawn,
        // Can be one of: NVG_BUTT (default), NVG_ROUND, NVG_SQUARE.
        void LineCap(int cap);

        // Sets how sharp path corners are drawn.
        // Can be one of NVG_MITER (default), NVG_ROUND, NVG_BEVEL.
        void LineJoin(int join);

        // Sets the transparency applied to all rendered shapes.
        // Already transparent paths will get proportionally more transparent as well.
        void GlobalAlpha(float alpha);

        //
        // Transforms
        //
        // The paths, gradients, patterns and scissor region are transformed by an transformation
        // matrix at the time when they are passed to the API.
        // The current transformation matrix is a affine matrix:
        //   [sx kx tx]
        //   [ky sy ty]
        //   [ 0  0  1]
        // Where: sx,sy define scaling, kx,ky skewing, and tx,ty translation.
        // The last row is assumed to be 0,0,1 and is not stored.
        //
        // Apart from ResetTransform(), each transformation function first creates
        // specific transformation matrix and pre-multiplies the current transformation by it.
        //
        // Current coordinate system (transformation) can be saved and restored using Save() and Restore().

        // Resets current transform to a identity matrix.
        void ResetTransform();

        // Premultiplies current coordinate system by specified matrix.
        // The parameters are interpreted as matrix as follows:
        //   [a c e]
        //   [b d f]
        //   [0 0 1]
        void Transform(float a, float b, float c, float d, float e, float f);

        // Translates current coordinate system.
        void Translate(float x, float y);

        // Rotates current coordinate system. Angle is specified in radians.
        void Rotate(float angle);

        // Skews the current coordinate system along X axis. Angle is specified in radians.
        void SkewX(float angle);

        // Skews the current coordinate system along Y axis. Angle is specified in radians.
        void SkewY(float angle);

        // Scales the current coordinate system.
        void Scale(float x, float y);

        // Stores the top part (a-f) of the current transformation matrix in to the specified buffer.
        //   [a c e]
        //   [b d f]
        //   [0 0 1]
        // There should be space for 6 floats in the return buffer for the values a-f.
        void CurrentTransform(float* xform);


        // The following functions can be used to make calculations on 2x3 transformation matrices.
        // A 2x3 matrix is represented as float[6].

        // Sets the transform to identity matrix.
        void TransformIdentity(float* dst);

        // Sets the transform to translation matrix matrix.
        void TransformTranslate(float* dst, float tx, float ty);

        // Sets the transform to scale matrix.
        void TransformScale(float* dst, float sx, float sy);

        // Sets the transform to rotate matrix. Angle is specified in radians.
        void TransformRotate(float* dst, float a);

        // Sets the transform to skew-x matrix. Angle is specified in radians.
        void TransformSkewX(float* dst, float a);

        // Sets the transform to skew-y matrix. Angle is specified in radians.
        void TransformSkewY(float* dst, float a);

        // Sets the transform to the result of multiplication of two transforms, of A = A*B.
        void TransformMultiply(float* dst, const float* src);

        // Sets the transform to the result of multiplication of two transforms, of A = B*A.
        void TransformPremultiply(float* dst, const float* src);

        // Sets the destination to inverse of specified transform.
        // Returns 1 if the inverse could be calculated, else 0.
        int TransformInverse(float* dst, const float* src);

        // Transform a point by given transform.
        void TransformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy);

        // Converts degrees to radians and vice versa.
        float DegToRad(float deg);
        float RadToDeg(float rad);

        //
        // Images
        //
        // NanoVG allows you to load jpg, png, psd, tga, pic and gif files to be used for rendering.
        // In addition you can upload your own image. The image loading is provided by stb_image.
        // The parameter imageFlags is combination of flags defined in NVGimageFlags.

        // Creates image by loading it from the disk from specified file name.
        // Returns handle to the image.
        int CreateImage(const char* filename, int imageFlags);

        // Creates image by loading it from the specified chunk of memory.
        // Returns handle to the image.
        int CreateImageMem(int imageFlags, unsigned char* data, int ndata);

        // Creates image from specified image data.
        // Returns handle to the image.
        int CreateImageRGBA(int w, int h, int imageFlags, const unsigned char* data);

        // Updates image data specified by image handle.
        void UpdateImage(int image, const unsigned char* data);

        // Returns the dimensions of a created image.
        void ImageSize(int image, int* w, int* h);

        // Deletes created image.
        void DeleteImage(int image);

        //
        // Paints
        //
        // NanoVG supports four types of paints: linear gradient, box gradient, radial gradient and image pattern.
        // These can be used as paints for strokes and fills.

        // Creates and returns a linear gradient. Parameters (sx,sy)-(ex,ey) specify the start and end coordinates
        // of the linear gradient, icol specifies the start color and ocol the end color.
        // The gradient is transformed by the current transform when it is passed to FillPaint() or StrokePaint().
        NVGpaint LinearGradient(float sx, float sy, float ex, float ey,
                                   NVGcolor icol, NVGcolor ocol);

        // Creates and returns a box gradient. Box gradient is a feathered rounded rectangle, it is useful for rendering
        // drop shadows or highlights for boxes. Parameters (x,y) define the top-left corner of the rectangle,
        // (w,h) define the size of the rectangle, r defines the corner radius, and f feather. Feather defines how blurry
        // the border of the rectangle is. Parameter icol specifies the inner color and ocol the outer color of the gradient.
        // The gradient is transformed by the current transform when it is passed to FillPaint() or StrokePaint().
        NVGpaint BoxGradient(float x, float y, float w, float h,
                                float r, float f, NVGcolor icol, NVGcolor ocol);

        // Creates and returns a radial gradient. Parameters (cx,cy) specify the center, inr and outr specify
        // the inner and outer radius of the gradient, icol specifies the start color and ocol the end color.
        // The gradient is transformed by the current transform when it is passed to FillPaint() or StrokePaint().
        NVGpaint RadialGradient(float cx, float cy, float inr, float outr,
                                   NVGcolor icol, NVGcolor ocol);

        // Creates and returns an image patter. Parameters (ox,oy) specify the left-top location of the image pattern,
        // (ex,ey) the size of one image, angle rotation around the top-left corner, image is handle to the image to render.
        // The gradient is transformed by the current transform when it is passed to FillPaint() or StrokePaint().
        NVGpaint ImagePattern(float ox, float oy, float ex, float ey,
                                 float angle, int image, float alpha);

        //
        // Scissoring
        //
        // Scissoring allows you to clip the rendering into a rectangle. This is useful for various
        // user interface cases like rendering a text edit or a timeline.

        // Sets the current scissor rectangle.
        // The scissor rectangle is transformed by the current transform.
        void Scissor(float x, float y, float w, float h);

        // Intersects current scissor rectangle with the specified rectangle.
        // The scissor rectangle is transformed by the current transform.
        // Note: in case the rotation of previous scissor rect differs from
        // the current one, the intersection will be done between the specified
        // rectangle and the previous scissor rectangle transformed in the current
        // transform space. The resulting shape is always rectangle.
        void IntersectScissor(float x, float y, float w, float h);

        // Reset and disables scissoring.
        void ResetScissor();

        //
        // Paths
        //
        // Drawing a new shape starts with BeginPath(), it clears all the currently defined paths.
        // Then you define one or more paths and sub-paths which describe the shape. The are functions
        // to draw common shapes like rectangles and circles, and lower level step-by-step functions,
        // which allow to define a path curve by curve.
        //
        // NanoVG uses even-odd fill rule to draw the shapes. Solid shapes should have counter clockwise
        // winding and holes should have counter clockwise order. To specify winding of a path you can
        // call PathWinding(). This is useful especially for the common shapes, which are drawn CCW.
        //
        // Finally you can fill the path using current fill style by calling Fill(), and stroke it
        // with current stroke style by calling Stroke().
        //
        // The curve segments and sub-paths are transformed by the current transform.

        // Clears the current path and sub-paths.
        void BeginPath();

        // Starts new sub-path with specified point as first point.
        void MoveTo(float x, float y);

        // Adds line segment from the last point in the path to the specified point.
        void LineTo(float x, float y);

        // Adds cubic bezier segment from last point in the path via two control points to the specified point.
        void BezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y);

        // Adds quadratic bezier segment from last point in the path via a control point to the specified point.
        void QuadTo(float cx, float cy, float x, float y);

        // Adds an arc segment at the corner defined by the last path point, and two specified points.
        void ArcTo(float x1, float y1, float x2, float y2, float radius);

        // Closes current sub-path with a line segment.
        void ClosePath();

        // Sets the current sub-path winding, see NVGwinding and NVGsolidity.
        void PathWinding(int dir);

        // Creates new circle arc shaped sub-path. The arc center is at cx,cy, the arc radius is r,
        // and the arc is drawn from angle a0 to a1, and swept in direction dir (NVG_CCW, or NVG_CW).
        // Angles are specified in radians.
        void Arc(float cx, float cy, float r, float a0, float a1, int dir);

        // Creates new rectangle shaped sub-path.
        void Rect(float x, float y, float w, float h);

        // Creates new rounded rectangle shaped sub-path.
        void RoundedRect(float x, float y, float w, float h, float r);

        // Creates new rounded rectangle shaped sub-path with varying radii for each corner.
        void RoundedRectVarying(float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft);

        // Creates new ellipse shaped sub-path.
        void Ellipse(float cx, float cy, float rx, float ry);

        // Creates new circle shaped sub-path.
        void Circle(float cx, float cy, float r);

        // Fills the current path with current fill style.
        void Fill();

        // Fills the current path with current stroke style.
        void Stroke();

        //
        // Text
        //
        // NanoVG allows you to load .ttf files and use the font to render text.
        //
        // The appearance of the text can be defined by setting the current text style
        // and by specifying the fill color. Common text and font settings such as
        // font size, letter spacing and text align are supported. Font blur allows you
        // to create simple text effects such as drop shadows.
        //
        // At render time the font face can be set based on the font handles or name.
        //
        // Font measure functions return values in local space, the calculations are
        // carried in the same resolution as the final rendering. This is done because
        // the text glyph positions are snapped to the nearest pixels sharp rendering.
        //
        // The local space means that values are not rotated or scale as per the current
        // transformation. For example if you set font size to 12, which would mean that
        // line height is 16, then regardless of the current scaling and rotation, the
        // returned line height is always 16. Some measures may vary because of the scaling
        // since aforementioned pixel snapping.
        //
        // While this may sound a little odd, the setup allows you to always render the
        // same way regardless of scaling. I.e. following works regardless of scaling:
        //
        //		const char* txt = "Text me up.";
        //		TextBounds(vg, x,y, txt, NULL, bounds);
        //		BeginPath(vg);
        //		RoundedRect(vg, bounds[0],bounds[1], bounds[2]-bounds[0], bounds[3]-bounds[1]);
        //		Fill(vg);
        //
        // Note: currently only solid color fill is supported for text.

        // Creates font by loading it from the disk from specified file name.
        // Returns handle to the font.
        int CreateFont(const char* name, const char* filename);

        // Creates font by loading it from the specified memory chunk.
        // Returns handle to the font.
        int CreateFontMem(const char* name, unsigned char* data, int ndata, int freeData);

        // Finds a loaded font of specified name, and returns handle to it, or -1 if the font is not found.
        int FindFont(const char* name);

        // Adds a fallback font by handle.
        int AddFallbackFontId(int baseFont, int fallbackFont);

        // Adds a fallback font by name.
        int AddFallbackFont(const char* baseFont, const char* fallbackFont);

        // Sets the font size of current text style.
        void FontSize(float size);

        // Sets the blur of current text style.
        void FontBlur(float blur);

        // Sets the letter spacing of current text style.
        void TextLetterSpacing(float spacing);

        // Sets the proportional line height of current text style. The line height is specified as multiple of font size.
        void TextLineHeight(float lineHeight);

        // Sets the text align of current text style, see NVGalign for options.
        void TextAlign(int align);

        // Sets the font face based on specified id of current text style.
        void FontFaceId(int font);

        // Sets the font face based on specified name of current text style.
        void FontFace(const char* font);

        // Draws text string at specified location. If end is specified only the sub-string up to the end is drawn.
        float Text(float x, float y, const char* string, const char* end);
        float Text(float x, float y, const char* string);

        // Draws multi-line text string at specified location wrapped at the specified width. If end is specified only the sub-string up to the end is drawn.
        // White space is stripped at the beginning of the rows, the text is split at word boundaries or when new-line characters are encountered.
        // Words longer than the max width are slit at nearest character (i.e. no hyphenation).
        void TextBox(float x, float y, float breakRowWidth, const char* string, const char* end);
        void TextBox(float x, float y, float breakRowWidth, const char* string);

        // Measures the specified text string. Parameter bounds should be a pointer to float[4],
        // if the bounding box of the text should be returned. The bounds value are [xmin,ymin, xmax,ymax]
        // Returns the horizontal advance of the measured text (i.e. where the next character should drawn).
        // Measured values are returned in local coordinate space.
        float TextBounds(float x, float y, const char* string, const char* end, float* bounds);

        // Measures the specified multi-text string. Parameter bounds should be a pointer to float[4],
        // if the bounding box of the text should be returned. The bounds value are [xmin,ymin, xmax,ymax]
        // Measured values are returned in local coordinate space.
        void TextBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds);
        vec4 TextBoxBounds(float x, float y, float breakRowWidth, const char* string);

        // Calculates the glyph x positions of the specified text. If end is specified only the sub-string will be used.
        // Measured values are returned in local coordinate space.
        int TextGlyphPositions(float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions);

        // Returns the vertical metrics based on the current text style.
        // Measured values are returned in local coordinate space.
        void TextMetrics(float* ascender, float* descender, float* lineh);

        // Breaks the specified text into lines. If end is specified only the sub-string will be used.
        // White space is stripped at the beginning of the rows, the text is split at word boundaries or when new-line characters are encountered.
        // Words longer than the max width are slit at nearest character (i.e. no hyphenation).
        int TextBreakLines(const char* string, const char* end, float breakRowWidth, NVGtextRow* rows, int maxRows);


        bool init() override;

        void updateComponent(SceneRuntime*) override;
        void BeginFrame();
        bool render(SceneRuntime*);

    protected:
            NVGcontext* mContext;
            WindowComponent* mWindowComponent;
    };
}
