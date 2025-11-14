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