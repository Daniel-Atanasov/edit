#include <core/ft.hpp>

#include <algorithm>

#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_LCD_FILTER_H
#include FT_BITMAP_H

#include <core/log.hpp>

namespace ft
{
namespace
{
    ft::freetype ft = ft::freetype::init();
}

i32 ft::glyph::get_left()
{
    return handle->bitmap_left;
}

i32 ft::glyph::get_top()
{
    return handle->bitmap_top;
}

img::vec<i64> ft::glyph::get_advance()
{
    return img::vec { handle->advance.x, handle->advance.y };
}

img::image ft::glyph::get_image()
{
    FT_Bitmap bitmap = handle->bitmap;

    img::image image(bitmap.width / 3, bitmap.rows);

    for (i32 y = 0; y < image.height(); y++)
    {
        img::color * data = (img::color *) &bitmap.buffer[y * bitmap.pitch];
        std:copy_n(data, image.width(), std::begin(image[y]));
    }

    return image;
}

void ft::glyph::render()
{
    FT_Render_Glyph(handle, FT_RENDER_MODE_LCD);
}

ft::face::face(std::string const & filename)
{
    FT_New_Face(ft, filename.c_str(), 0, &handle);
    set_charmap(FT_ENCODING_UNICODE);
}

ft::face::face(ft::face && other)
{
    std::swap(handle, other.handle);
}

ft::face & ft::face::operator=(ft::face && other)
{
    destroy();
    std::swap(handle, other.handle);
    return *this;
}

bool ft::face::has_kerning()
{
    return FT_HAS_KERNING(handle);
}

std::string ft::face::get_family_name()
{
    return handle->family_name;
}

std::string ft::face::get_style_name()
{
    return handle->style_name;
}

u32 ft::face::get_index(u32 cp)
{
    return FT_Get_Char_Index(handle, cp);
}

img::vec<i64> ft::face::get_kerning(u32 last, u32 next)
{
    FT_Vector delta;
    FT_Get_Kerning(handle, last, next, FT_KERNING_DEFAULT, &delta);
    return img::vec(delta.x, delta.y);
}

ft::metrics ft::face::get_metrics()
{
    FT_Size_Metrics metrics = handle->size->metrics;
    return ft::metrics
    {
        img::vec { metrics.x_ppem,  metrics.y_ppem  },
        img::vec { metrics.x_scale, metrics.y_scale },
        metrics.ascender,
        metrics.descender,
        metrics.height,
        metrics.max_advance
     };
}

ft::glyph ft::face::get_glyph()
{
    ft::glyph glyph;
    glyph.handle = handle->glyph;
    return glyph;
}

void ft::face::set_charmap(u32 charmap)
{
    FT_Select_Charmap(handle, (FT_Encoding) charmap);
}

void ft::face::set_size(u32 width, u32 height, u32 hdpi, u32 vdpi)
{
    FT_Set_Char_Size(handle, width, height, hdpi, vdpi);
}

void ft::face::load(u32 cp)
{
    FT_Load_Glyph(handle, cp, FT_LOAD_COLOR);
}

void ft::face::destroy()
{
    FT_Done_Face(handle);
    handle = nullptr;
}

ft::face::~face()
{
    destroy();
}

ft::freetype::freetype(ft::freetype && other)
{
    std::swap(handle, other.handle);
}

ft::freetype & ft::freetype::operator=(ft::freetype && other)
{
    destroy();
    std::swap(handle, other.handle);
    return *this;
}

ft::freetype ft::freetype::init()
{
    ft::freetype ft;
    FT_Init_FreeType(&ft.handle);
    return ft;
}

void ft::freetype::destroy()
{
    FT_Done_FreeType(handle);
    handle = nullptr;
}

ft::freetype::~freetype()
{
    destroy();
}
}
