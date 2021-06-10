#pragma once

#include <unordered_map>

#include <core/image.hpp>
#include <core/log.hpp>
#include <core/misc.hpp>

namespace fnt
{
struct glyph_id
{
    // TODO@Daniel:
    //  Maybe just use weight with a fixed value?
    bool bold:    1;
    bool italic:  1;
    u16 weight:  10;
    u32 value:   21;
};

static_assert(sizeof(fnt::glyph_id) == 8, "sizeof(fnt::glyph_id) is expected to be 8 bytes, due to being used as a hash.");
}

namespace std
{
template <>
struct hash<fnt::glyph_id>
{
    u64 operator()(fnt::glyph_id id) const
    {
        return *(u64 *)&id;
    }
};
}

namespace fnt
{
struct font_cache
{
private:
    std::unordered_map<fnt::glyph_id, img::image> map;

public:
    
};
}

