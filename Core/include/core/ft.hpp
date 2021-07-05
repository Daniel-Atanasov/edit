#pragma once

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <core/misc.hpp>
#include <core/image.hpp>

namespace gsm::ft
{
struct metrics
{
    img::vec<u16> em;
    img::vec<long> scale;

    i64 ascender    = 0;
    i64 descender   = 0;
    i64 height      = 0;
    i64 max_advance = 0;
};

struct glyph
{
private:
    FT_GlyphSlot handle = nullptr;

    friend struct face;

    operator FT_GlyphSlot()
    {
        return handle;
    }

public:
    glyph() = default;

    glyph(ft::glyph const & other) = delete;
    glyph(ft::glyph && other);

    ft::glyph operator=(ft::glyph const & other) = delete;
    ft::glyph operator=(ft::glyph && other);

    i32 get_left();
    i32 get_top();

    img::vec<long> get_advance();

    img::image get_image();

    void render();
};

struct face
{
// TODO@Daniel:
//  Change to private once harfbuzz wrapper is done
public:
    FT_Face handle = nullptr;

public:
    face() = default;
    face(std::string const & filename);

    face(ft::face const & other) = delete;
    face(ft::face && other);

    ft::face & operator=(ft::face const & other) = delete;
    ft::face & operator=(ft::face && other);

    bool has_kerning();

    std::string get_family_name();
    std::string get_style_name();

    u32 get_index(u32 cp);
    img::vec<long> get_kerning(u32 last, u32 next);
    ft::metrics get_metrics();
    ft::glyph get_glyph();

    void set_charmap(u32 charmap);
    void set_size(u32 width, u32 height, u32 hdpi, u32 vdpi);

    void load(u32 cp);

    void destroy();

    ~face();
};

struct freetype
{
private:
    FT_Library handle = nullptr;

    friend struct face;

    operator FT_Library()
    {
        return handle;
    }

public:
    freetype() = default;

    freetype(ft::freetype const & other) = delete;
    freetype(ft::freetype && other);

    ft::freetype & operator=(ft::freetype const & other) = delete;
    ft::freetype & operator=(ft::freetype && other);

    static ft::freetype init();

    void destroy();

    ~freetype();
};
}
