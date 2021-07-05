#pragma once

#include <span>
#include <string_view>

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include <core/ft.hpp>

namespace gsm::hb
{
using glyph = hb_glyph_info_t;

struct buffer;
struct font;

struct buffer
{
private:
    hb_buffer_t * handle = nullptr;

    friend void shape(hb::font const & font, hb::buffer const & buffer);

public:
    buffer() = default;
    buffer(hb::buffer const & other);
    buffer(hb::buffer && other);

    buffer(std::string_view text);

    hb::buffer & operator=(hb::buffer const & other);
    hb::buffer & operator=(hb::buffer && other);

    std::span<hb::glyph> get_glyphs();

    void set_content_type(u32 type);
    void set_script(u32 script);

    void assign(std::string_view text);

    void add(std::string_view text);

    void guess_properties();

    void clear();

    void destroy();

    ~buffer();
};

struct font
{
private:
    hb_font_t * handle = nullptr;

    friend void shape(hb::font const & font, hb::buffer const & buffer);

public:
    font() = default;
    font(ft::face const & face);

    font(hb::font const & other) = delete;
    font(hb::font && other);

    hb::font & operator=(hb::font const & other) = delete;
    hb::font & operator=(hb::font && other);

    void destroy();

    ~font();
};

void shape(hb::font const & font, hb::buffer const & buffer);
}
