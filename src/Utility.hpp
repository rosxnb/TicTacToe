#pragma once

#include <memory>
#include <string_view>

struct Utility
{
    static std::unique_ptr<std::string> read_file( const std::string_view path );
};
