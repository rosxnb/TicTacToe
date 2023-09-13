#pragma once

#include <memory>
#include <string_view>

struct Utility
{
    static std::unique_ptr<std::string> read_file( const std::string_view path );
};

struct Point2D
{
    float x = 0.f;
    float y = 0.f;
};

struct Point3D
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct Point4D
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;
};
