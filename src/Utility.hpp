#pragma once

#include <memory>
#include <string_view>

namespace utility
{
    std::string read_file( const std::string_view path );
    std::pair<size_t, size_t> process_keyboard_inputs();
}
