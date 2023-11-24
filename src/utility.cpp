#include "utility.hpp"
#include <fstream>
#include <sstream>

std::string Utility::read_file( std::string_view fileLocation )
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
