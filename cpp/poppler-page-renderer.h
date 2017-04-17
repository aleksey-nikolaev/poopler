/*
 * Copyright (C) 2010, Pino Toscano <pino@kde.org>
 * Copyright (C) 2018, Zsombor Hollay-Horvath <hollay.horvath@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef POPPLER_PAGE_RENDERER_H
#define POPPLER_PAGE_RENDERER_H

#include "poppler-global.h"
#include "poppler-image.h"
#include "poppler-extern-draw.h"

namespace poppler {

typedef unsigned int argb;

class page;
class page_renderer_private;

class POPPLER_CPP_EXPORT page_renderer : public poppler::noncopyable
{
public:
    enum render_hint
    {
        antialiasing = 0x00000001,
        text_antialiasing = 0x00000002,
        text_hinting = 0x00000004
    };

    enum line_mode_enum {
        line_default,
        line_solid,
        line_shape
    };

    enum class splash_color_mode
    {
        // 1 bit per component, 8 pixels per byte,   MSbit is on the left
        Mono1,
        // 1 byte per component, 1 byte per pixel
        Mono8,
        // 1 byte per component, 3 bytes per pixel:   RGBRGB...
        RGB8,
        // 1 byte per component, 3 bytes per pixel:   BGRBGR...
        BGR8,
        // 1 byte per component, 4 bytes per pixel:   XBGRXBGR...
        XBGR8,
        // 1 byte per component, 4 bytes per pixel:   CMYKCMYK...
        CMYK8,
        // 1 byte per component, 
        // 4 bytes + n bytes spot colors per pixel:
        // CMYKSSSSCMYKSSSS... so n == 4, isn't it?
        DeviceN8,
        NotSet = -1
    };
    page_renderer();
    ~page_renderer();

    argb paper_color() const;
    void set_paper_color(argb c);

    unsigned int render_hints() const;
    void set_render_hint(render_hint hint, bool on = true);
    void set_render_hints(unsigned int hints);

    image::format_enum image_format() const;
    void set_image_format(image::format_enum format);

    line_mode_enum line_mode() const;
    void set_line_mode(line_mode_enum mode);

    image render_page(const page *p,
                      double xres = 72.0, double yres = 72.0,
                      int x = -1, int y = -1, int w = -1, int h = -1,
                      rotation_enum rotate = rotate_0) const;

    struct SplashBitmapResult : public ProcessStep
    {
        StepInformation stepInformation() override
        {
            return static_cast<StepInformation>(ProcessStep::Extra + 1);
        }
        ImageData imageData;
    };

    ProcessStepStore extract_process(const page *p,
                                     double xres = 72.0, double yres = 72.0, splash_color_mode splashMode = splash_color_mode::NotSet) const;

    static bool can_render();

private:
    page_renderer_private *d;
    friend class page_renderer_private;
};

}

#endif
