#include "utility.hpp"
#include "keys_code.h"
#include <fstream>
#include <sstream>

std::string utility::read_file( std::string_view fileLocation )
{
    std::string buffer;

    std::ifstream fileHandler;
    fileHandler.exceptions( std::ifstream::badbit | std::ifstream::failbit );

    try 
    {
        fileHandler.open( fileLocation );
        std::stringstream sstream;
        sstream << fileHandler.rdbuf();
        buffer.assign( sstream.str() );
    }
    catch (std::ifstream::failure err)
    {
        __builtin_printf("Failed to load file: %s \n", fileLocation.data());
        __builtin_printf("%s \n\n", err.what());
    }

    return buffer;
}

std::pair<size_t, size_t> utility::process_keyboard_inputs()
{
    // if (checkKeyPress( KeysCode::W ))
    //     __builtin_printf("Key W \n");
    // if (checkKeyPress( KeysCode::S ))
    //     __builtin_printf("Key S \n");
    // if (checkKeyPress( KeysCode::D ))
    //     __builtin_printf("Key D \n");
    // if (checkKeyPress( KeysCode::A ))
    //     __builtin_printf("Key A \n");
    return { 2, 2 };
}
