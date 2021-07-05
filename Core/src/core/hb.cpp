#include <core/hb.hpp>

namespace gsm::hb
{
hb::buffer::buffer(hb::buffer const & other) : handle(other.handle)
{
    hb_buffer_reference(handle);
}

hb::buffer::buffer(hb::buffer && other)
{
    std::swap(handle, other.handle);
}

hb::buffer & hb::buffer::operator=(hb::buffer const & other)
{
    handle = other.handle;
    hb_buffer_reference(handle);
    return *this;
}

hb::buffer & hb::buffer::operator=(hb::buffer && other)
{
    destroy();
    std::swap(handle, other.handle);
    return *this;
}

std::span<hb::glyph> hb::buffer::get_glyphs()
{
    u32 size;
    hb::glyph * data = hb_buffer_get_glyph_infos(handle, &size);
    return std::span { data, size };
}

void hb::buffer::set_content_type(u32 type)
{
    hb_buffer_set_content_type(handle, (hb_buffer_content_type_t) type);
}

void hb::buffer::set_script(u32 script)
{
    hb_buffer_set_script(handle, (hb_script_t) script);
}

void hb::buffer::assign(std::string_view text)
{
    if (handle == nullptr)
    {
        handle = hb_buffer_create();
        set_content_type(HB_BUFFER_CONTENT_TYPE_UNICODE);
        set_script(HB_SCRIPT_COMMON);
    }

    clear();
    add(text);
}

void hb::buffer::add(std::string_view text)
{
    hb_buffer_add_utf8(handle, text.data(), text.size(), 0, text.size());
}

void hb::buffer::guess_properties()
{
    hb_buffer_guess_segment_properties(handle);
}

void hb::buffer::clear()
{
    hb_buffer_clear_contents(handle);
}

void hb::buffer::destroy()
{
    hb_buffer_destroy(handle);
    handle = nullptr;
}

hb::buffer::~buffer()
{
    destroy();
}

hb::font::font(ft::face const & face)
{
    handle = hb_ft_font_create_referenced(face.handle);
}

hb::font::font(hb::font && other)
{
    std::swap(handle, other.handle);
}

hb::font & hb::font::operator=(hb::font && other)
{
    destroy();
    std::swap(handle, other.handle);
    return *this;
}

void hb::font::destroy()
{
    hb_font_destroy(handle);
    handle = nullptr;
}

hb::font::~font()
{
    destroy();
}

void shape(hb::font const & font, hb::buffer const & buffer)
{
    hb_shape(font.handle, buffer.handle, nullptr, 0);
}
}
