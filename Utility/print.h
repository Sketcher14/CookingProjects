#pragma once

#include <format>

template<typename... Args>
constexpr void print(const std::string_view str_fmt, Args&&... args)
{
    fputs(std::vformat(str_fmt, std::make_format_args(args...)).c_str(), stdout);
}

template<typename... Args>
constexpr void println(const std::string_view str_fmt, Args&&... args)
{
    puts(std::vformat(str_fmt, std::make_format_args(args...)).c_str());
}
