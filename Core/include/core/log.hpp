#pragma once

#include <cstdlib>

#include <array>
#include <string>
#include <string_view>

namespace log
{

#define FORE(r, g, b) "\e[38;2;" #r ";" #g ";" #b "m"
#define BACK(r, g, b) "\e[48;2;" #r ";" #g ";" #b "m"

enum severity
{
    ERROR,
    WARNING,
    INFO,
    DEBUG
};

void print(severity s, std::string const & format, auto const & ... args)
{
    static constexpr std::array colors =
    {
        FORE(255, 0,   0  ),
        FORE(255, 127, 63 ),
        FORE(127, 255, 255),
        FORE(0,   195, 255),
    };

    static constexpr std::array tags =
    {
        "ERROR",
        "WARNING",
        "INFO",
        "DEBUG"
    };

    std::string_view color = colors[s];
    std::string_view tag   = tags[s];

    printf(
        "[%.*s%.*s\e[39;m] ",
        (int) color.size(), color.data(),
        (int) tag.size(),   tag.data()
    );

    auto argify = [](auto const & arg) -> decltype(auto)
    {
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>)
        {
            return arg.c_str();
        }
        else
        {
            return arg;
        }
    };

    // NOTE@Daniel:
    //  When no other args have been provided
    //  Prevents warning -Wformat-security
    if constexpr (sizeof...(args) == 0)
    {
        printf("%.*s", (int) format.size(), format.data());
    }
    else
    {
        printf(format.c_str(), argify(args)...);
    }

    puts("");
}

void error(auto const & ... args)
{
    print(ERROR, args...);
}

void warning(auto const & ... args)
{
    print(WARNING, args...);
}

void info(auto const & ... args)
{
    print(INFO, args...);
}

void debug(auto const & ... args)
{
    print(DEBUG, args...);
}

#undef FORE
#undef BACK

}
