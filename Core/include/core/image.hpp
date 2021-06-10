#pragma once

#include <cstdint>

#include <span>
#include <vector>

#include <algorithm>

#include <core/misc.hpp>

namespace img
{
template <std::integral T = i64>
struct vec
{
    T x = 0;
    T y = 0;

    constexpr vec() = default;

    constexpr vec(T x, T y) : x(x), y(y)
    {
    }
};

struct color
{
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;

    constexpr color() = default;
    constexpr color(img::color const & other) = default;
    constexpr color(img::color && other) = default;

    constexpr color(u8 r, u8 g, u8 b) :
        r(r), g(g), b(b)
    {
    }

    constexpr color(u32 v) :
        color(v >> 16, v >> 8, v)
    {
    }

    constexpr img::color & operator=(img::color const & other) = default;
    constexpr img::color & operator=(img::color && other) = default;

    constexpr u32 value() const
    {
        return (r << 16) | (g << 8) | b;
    }
};

template <std::integral T>
constexpr T blend(T b, T f, T a)
{
    double alpha = a / 255.0;
    return alpha * f + (1.0 - alpha) * b;
}

constexpr img::color blend(img::color d, img::color s, img::color a)
{
    return color(
        img::blend(d.r, s.r, a.r),
        img::blend(d.g, s.g, a.g),
        img::blend(d.b, s.b, a.b)
    );
}

struct fu
{
private:
    i64 value = 0;

public:
    constexpr fu() = default;
    constexpr fu(img::fu const & other) = default;
    constexpr fu(img::fu && other) = default;

    constexpr fu(std::integral auto value) : value(value * 64)
    {
    }

    constexpr img::fu & operator=(img::fu const & other) = default;
    constexpr img::fu & operator=(img::fu && other) = default;

    operator std::integral auto()
    {
        return value / 64;
    }
};

struct image
{
private:
    std::vector<img::color> buffer;
    int w;
    int h;

public:
    image() = default;
    image(img::image const & other) = default;
    image(img::image && other) = default;

    image(int w, int h) : w(w), h(h)
    {
        buffer.resize(w * h);
    }

    image(color * d, int w, int h) : image(w, h)
    {
        std::copy_n(d, w * h, buffer.data());
    }

    img::image & operator=(img::image const & other) = default;
    img::image & operator=(img::image && other) = default;

    int width()  const { return w; }
    int height() const { return h; }

    auto operator[](int row) const
    {
        return std::span { buffer.data() + row * w, (u32) w };
    }

    auto operator[](int row)
    {
        return std::span { buffer.data() + row * w, (u32) w };
    }
};
}sudo ldconfig -p | grep -i gl.so
