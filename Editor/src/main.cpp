#include <cmath>

#include <concepts>
#include <vector>
#include <span>
#include <string>
#include <string_view>

#include <SDL2/SDL.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_LCD_FILTER_H
#include FT_BITMAP_H

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include <core/log.hpp>
#include <core/image.hpp>
#include <core/font.hpp>

#include <core/ft.hpp>
#include <core/hb.hpp>

auto to_font_units(auto value)
{
    return value * 64;
}

auto from_font_units(auto value)
{
    return value / 64;
}

std::string filename = "/home/daniel/.local/share/fonts/CascadiaCodePLItalic-BoldItalic.ttf";

struct EditorData
{
    int width;
    int height;
    int point_size;
    bool is_running;
};

int on_event(EditorData * data, SDL_Event event)
{
    switch (event.type)
    {
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            data->width  = event.window.data1;
            data->height = event.window.data2;
            log::info("EventWatch: Window size changed: %dx%d", data->width, data->height);
        }
        break;
    case SDL_MOUSEWHEEL:
        log::info("Mouse wheel event");
        if (event.wheel.y > 0)
        {
            data->point_size = std::min(70, std::max(6, data->point_size + 1));
        }
        else
        {
            data->point_size = std::min(70, std::max(6, data->point_size - 1));
        }
        log::info("EventWatch: Mouse wheel event: %d", event.wheel.y);
        break;
    case SDL_KEYDOWN:
        log::debug("Key down");
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            data->is_running = false;
        }
        break;
    case SDL_QUIT:
        data->is_running = false;
        break;
    }
    return 1;
}

int main(int argc, char * argv[])
{
    std::vector<std::string_view> lines
    {
        "1: | Line == 1",
        "2: | Line == 2",
        "3: | Line == 3",
        "4: | Line == 4",
        "5: | Line == 5"
    };

    f32 hdpi;
    f32 vdpi;
    SDL_GetDisplayDPI(0, NULL, &hdpi, &vdpi);

    ft::face face(filename);
    hb::font font(face);

    log::info("Loaded face: %s, %s", face.get_family_name(), face.get_style_name());

    SDL_Init(SDL_INIT_EVERYTHING);

    EditorData data = {
        .width = 1280,
        .height = 720,
        .point_size = 12,
        .is_running = true
    };

    SDL_Window * window = SDL_CreateWindow(
        "Editor",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        data.width, data.height,
        SDL_WINDOW_RESIZABLE
    );

    SDL_AddEventWatch((SDL_EventFilter) on_event, &data);

    hb::buffer buffer;
    while (data.is_running)
    {
        face.set_size(0, to_font_units(data.point_size), hdpi, vdpi);

        img::color color(38, 50, 56);

        SDL_Surface * surface = SDL_GetWindowSurface(window);
        SDL_FillRect(surface, NULL, color.value());

        int cursor_y = 0;
        for (std::string_view text : lines)
        {
            /* Layout the text */
            buffer.assign(text);
            buffer.guess_properties();

            hb::shape(font, buffer);

            int cursor_x = 0;
            int previous = 0;

            ft::metrics metrics = face.get_metrics();

            for (hb::glyph const & info : buffer.get_glyphs())
            {
                if (cursor_x >= surface->w)
                {
                    continue;
                }

                u32 codepoint   = info.codepoint;
                u32 glyph_index = face.get_index(codepoint);

                face.load(codepoint);

                if (face.has_kerning() && previous && glyph_index)
                {
                    cursor_x += face.get_kerning(previous, glyph_index).x;
                }

                ft::glyph glyph = face.get_glyph();

                glyph.render();

                i32 glyph_x = cursor_x + glyph.get_left();
                i32 glyph_y = cursor_y - glyph.get_top() + from_font_units(metrics.ascender);

                img::image image = glyph.get_image();
                if (glyph_x + image.width() < 0)
                {
                    continue;
                }

                std::span<u32> pixels((u32 *) surface->pixels, surface->h * surface->w);
                for (i32 row = 0; row < image.height(); row++)
                {
                    for (i32 col = 0; col < image.width(); col++)
                    {
                        int x = glyph_x + col;
                        int y = glyph_y + row;

                        if (x < 0 || x >= surface->w || y < 0 || y >= surface->h)
                        {
                            continue;
                        }

                        u32 & dst = pixels[y * surface->w + x];
                        img::color src(0xFF, 0xCC, 0x00);

                        dst = img::blend(dst, src, image[row][col]).value();
                    }
                }

                img::vec<i64> advance = glyph.get_advance();
                cursor_x += from_font_units(advance.x);
                cursor_y -= from_font_units(advance.y);

                previous = glyph_index;
            }

            cursor_y += from_font_units(metrics.height);
        }

        SDL_UpdateWindowSurface(window);

        SDL_PollEvent(NULL);

        SDL_Delay(90);
    }

    SDL_Quit();

    return 0;
}
