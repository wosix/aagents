#pragma once
#include <raylib.h>
#include <array>

const std::array<Color, 8> DEFAULT_COLORS = {
    ORANGE,
    SKYBLUE,
    GREEN,
    PINK,
    RED,
    DARKBLUE,
    BEIGE,
    MAGENTA,
};

Color Brighten(Color color, float factor)
{
    return Color{
        (unsigned char)std::min(255, (int)(color.r * factor)),
        (unsigned char)std::min(255, (int)(color.g * factor)),
        (unsigned char)std::min(255, (int)(color.b * factor)),
        100};
}

Color MixColors(Color a, Color b, float ratio = 0.5f)
{
    Color result;
    result.r = a.r * (1 - ratio) + b.r * ratio;
    result.g = a.g * (1 - ratio) + b.g * ratio;
    result.b = a.b * (1 - ratio) + b.b * ratio;
    result.a = a.a * (1 - ratio) + b.a * ratio;
    return result;
}